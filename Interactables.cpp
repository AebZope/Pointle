#include "Interactables.h"
Aperture::CollisionInfo Aperture::Collision::chellCollisionX(Map& map,Chell& chell){
    // Sidene av karakteren "Chell" sine koordinater i x og y
    double chellLeft = chell.location.x; 
    double chellRight = chell.location.x + chell_width;
    double chellTop = chell.location.y;
    double chellBottom = chell.location.y + chell_height;

    std::vector<collisionGrid> collisionGridData; // Full av structs som beskriver posisjonen til hjørnene og typen blokk
    for(int row = 0; row < map.mapGrid.size(); row++){
        for(int col = 0; col < map.mapGrid.at(row).size(); col++){
            if(map.mapGrid.at(row).at(col) != Aperture::BlockType::Air){
                double left = col * map.block_width;
                double top = row * map.block_height;
                double right = left + map.block_width;
                double bottom = top + map.block_height;

                collisionGridData.push_back({
                    left, top,   //Top left x,y
                    right, top,   // tilsvarende for resten
                    left, bottom,  
                    right, bottom, 
                    row, col, //Indeksering av blokken, sånn at jeg kan finne tilbake og endre den
                    map.mapGrid.at(row).at(col) //BlockType av kollisjonen
                });
            }
        }
    }

    for(const auto& cd : collisionGridData){
        double biggerHitbox = 0;
        if(cd.type == BlockType::OrangePortal || cd.type == BlockType::BluePortal){
            biggerHitbox = 10;
        }

        bool verticalOverlap = (chellBottom > cd.topLeftY) && (chellTop < cd.bottomLeftY);
        bool horizontalOverlapLeft = (chellRight > cd.topLeftX-biggerHitbox) && (chellLeft < cd.topLeftX+biggerHitbox);
        bool horizontalOverlapRight = (chellLeft < cd.topRightX+biggerHitbox) && (chellRight > cd.topRightX-biggerHitbox);

        if(chell.velocity.x > 0){
            if(verticalOverlap && horizontalOverlapLeft){
                return {true, Direction::Left, cd.type, cd.row, cd.col}; 
            }
        }

        if(chell.velocity.x < 0 && verticalOverlap && horizontalOverlapRight){
            return {true, Direction::Right, cd.type, cd.row, cd.col}; 
        }
    }
    //Randbetingelser for å gjøre grendsen av window til "vegger"
    if(chellLeft <= 0){
        return {true, Direction::Right, Aperture::BlockType::Air};
    }

    if(chellRight >= 1440){
        return {true, Direction::Left, Aperture::BlockType::Air};
    }

    return {false, Direction::None, Aperture::BlockType::Air}; //Ingen kollisjon
};

Aperture::CollisionInfo Aperture::Collision::chellCollisionY(Map& map,Chell& chell){
    double chellLeft = chell.location.x; // Chell venstre og høyre koordinater
    double chellRight = chell.location.x + chell_width;
    double chellTop = chell.location.y;
    double chellBottom = chell.location.y + chell_height;

    std::vector<collisionGrid> collisionGridData; // Full av structs som beskriver posisjonen til hjørnene og typen blokk
    for(int row = 0; row < map.mapGrid.size(); row++){
        for(int col = 0; col < map.mapGrid.at(row).size(); col++){
            if(map.mapGrid.at(row).at(col) != Aperture::BlockType::Air){
                double left = col * map.block_width;
                double top = row * map.block_height;
                double right = left + map.block_width;
                double bottom = top + map.block_height;

                collisionGridData.push_back({
                    left, top,      
                    right, top,    
                    left, bottom,  
                    right, bottom, 
                    row, col,
                    map.mapGrid.at(row).at(col)
                });
            }
        }
    }

    // Skjekker opp og ned kollisjoner. 
    for(const auto& cd : collisionGridData){
        bool horizontalOverlap = (chellLeft+5 < cd.topRightX) && (chellRight-5 > cd.topLeftX); // Legger til 5 og trekker fra 5 sånn at vi ikke sitter fast i veggen, gjør hitboksen for å stå oppå noe mindre enn den for å kollidere. 
        bool verticleOverlapTop = (chellBottom > cd.topLeftY) && (chellTop < cd.topLeftY);
        bool verticleOverlapBottom = (chellTop < cd.bottomLeftY) && (chellBottom > cd.bottomLeftY);

        if(horizontalOverlap && verticleOverlapTop){// Skjekker kollisjon ovenifra, om man lander på en blokk
            return {true, Direction::Up, cd.type, cd.row, cd.col}; 
        }

        // Skjekker tilsvarende for nedenifra, om man dunker i den
        if(horizontalOverlap && verticleOverlapBottom){
            return {true, Direction::Down, cd.type, cd.row, cd.col}; 
        }
    }

    return {false, Direction::None, Aperture::BlockType::Air}; //Ingen kollisjon
};


void Aperture::Collision::portalCollision(Map& map,PortalGun& portalgun){ //Gjør det samme som for chell, bare for portalprojectile

    std::vector<collisionGrid> collisionGridData; 
    for(int row = 0; row < map.mapGrid.size(); row++){
        for(int col = 0; col < map.mapGrid.at(row).size(); col++){
            if(map.mapGrid.at(row).at(col) != Aperture::BlockType::Air){
                double left = col * map.block_width;
                double top = row * map.block_height;
                double right = left + map.block_width;
                double bottom = top + map.block_height;

                collisionGridData.push_back({
                    left, top,      
                    right, top,    
                    left, bottom,  
                    right, bottom, 
                    row,col,
                    map.mapGrid.at(row).at(col)
                });
            }
        }
    }

    for(auto& proj : portalgun.portalProjInfo){
        double projLeft = proj.location.x;
        double projRight = proj.location.x+portalgun_width;
        double projTop = proj.location.y;
        double projBottom = proj.location.y + portalgun_height;
        
        if(proj.velocity.x > 0){
            for(const auto& cd : collisionGridData){
                bool horizontalOverlap = (projLeft < cd.topRightX) && (projRight > cd.topLeftX); 
                bool verticalOverlap = (projBottom > cd.topLeftY) && (projTop < cd.bottomLeftY);

                if(horizontalOverlap && verticalOverlap){
                    proj.didCollide = true;
                    proj.type = cd.type;
                    proj.collisionDirection = Direction::Left; //Dersom kulen kommer fra høyre er kollisjon venstre av blokk
                    proj.blockRow = cd.row;
                    proj.blockCol = cd.col; 
                }
            } 
        }

        if(proj.velocity.x < 0){
            for(const auto& cd : collisionGridData){
                bool horizontalOverlap = (projLeft < cd.topRightX) && (projRight > cd.topLeftX); 
                bool verticalOverlap = (projBottom > cd.topLeftY) && (projTop < cd.bottomLeftY);

                if(horizontalOverlap && verticalOverlap){
                    proj.didCollide = true;
                    proj.type = cd.type;
                    proj.collisionDirection = Direction::Right; 
                    proj.blockRow = cd.row;
                    proj.blockCol = cd.col; 
                }
            } 
        }

        if(proj.velocity.y > 0){
            for(const auto& cd : collisionGridData){
                bool horizontalOverlap = (projLeft < cd.topRightX) && (projRight > cd.topLeftX); 
                bool verticalOverlap = (projBottom > cd.topLeftY) && (projTop < cd.bottomLeftY);

                if(horizontalOverlap && verticalOverlap){
                    proj.didCollide = true;
                    proj.type = cd.type;
                    proj.collisionDirection = Direction::Up; 
                    proj.blockRow = cd.row;
                    proj.blockCol = cd.col; 
                }
            } 
        }

        if(proj.velocity.y < 0){
            for(const auto& cd : collisionGridData){
                bool horizontalOverlap = (projLeft < cd.topRightX) && (projRight > cd.topLeftX); 
                bool verticalOverlap = (projBottom > cd.topLeftY) && (projTop < cd.bottomLeftY);

                if(horizontalOverlap && verticalOverlap){
                    proj.didCollide = true;
                    proj.type = cd.type;
                    proj.collisionDirection = Direction::Down; 
                    proj.blockRow = cd.row;
                    proj.blockCol = cd.col; 
                }
            } 
        }
    }
};
