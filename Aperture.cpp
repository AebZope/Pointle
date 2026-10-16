#include "Aperture.h"
#include <iostream>

void Aperture::run(){
    TDT4102::AnimationWindow window;
    Chell chell;
    PortalGun portalGun;
    Map map; 
    Assets assets;
    Collision collision; 

    int jumpframe = 0; //Frames man har holdt ned hoppknappen for 
    bool justPressedQ = false; //Passer på at skyt knappen er toggle og ikke konstant mens man holder knappen
    bool justPressedE = false; 
    bool justPressedMouse = false; //Tilsvarende for mus input 
    bool nextLevel = false; //Om man skal videre til neste level 
    int screen = 0; // Viser til om vi spiller, eller hvilken menu man er på. 0 - main menu, 1 - spiller, 2 - pause, 3 - instruks
    TDT4102::Point mousePoint; //Posisjon av museklikk
    FloatingPoint chell_SpawnPoint; //Der karakteren starter
    

    while(!window.should_close()){
        if(screen == 0){ //Main menu tegning, med oppdatering for å skjekke hvilke level man trykker på. Man kan gå til en gitt level eller se på kontroller
            map.drawMainMenu(window,assets);
            handleMouseInput(window,screen,mousePoint,justPressedMouse);

            if(mousePoint.x > 115 && mousePoint.x < 645 && mousePoint.y > 400 && mousePoint.y < 480){ //Playground knapp
                map.levelIndex = 0; //Setter til playground level 
                screen = 1; //Endrer til spillemodus
            }
            else if(mousePoint.x > 840 && mousePoint.x < 1265 && mousePoint.y > 400 && mousePoint.y < 485){//Knapp til Controls
                screen = 3;
            }

            else if(mousePoint.y > 550 && mousePoint.y < 700){//Vi ligger på øverste rad av leveler
                if(mousePoint.x > 135 && mousePoint.x < 295){ //knapp 1
                    map.levelIndex = 1; //Setter til riktig level 
                    screen = 1; //Setter til spillmodus 
                }
                else if(mousePoint.x > 380 && mousePoint.x < 540){ //kanpp 2
                    map.levelIndex = 2; 
                    screen = 1;
                }
                else if(mousePoint.x > 625 && mousePoint.x < 785){ //knapp 3 
                    map.levelIndex = 3; 
                    screen = 1;    
                }
                else if(mousePoint.x > 870 && mousePoint.x < 1030){
                    map.levelIndex = 4; 
                    screen = 1;
                }
                else if(mousePoint.x > 1110 && mousePoint.x < 1270){
                    map.levelIndex = 5; 
                    screen = 1;
                }
            }
            else if(mousePoint.y > 720 && mousePoint.y < 870){ //Tilsvarende for knappene på andre rekke
                if(mousePoint.x > 135 && mousePoint.x < 295){ 
                    map.levelIndex = 6; 
                    screen = 1; 
                }
                else if(mousePoint.x > 380 && mousePoint.x < 540){ 
                    map.levelIndex = 7; 
                    screen = 1;
                }
                else if(mousePoint.x > 625 && mousePoint.x < 785){ 
                    map.levelIndex = 8; 
                    screen = 1;    
                }
                else if(mousePoint.x > 870 && mousePoint.x < 1030){
                    map.levelIndex = 9; 
                    screen = 1;
                }
                else if(mousePoint.x > 1110 && mousePoint.x < 1270){
                    map.levelIndex = 10; 
                    screen = 1;
                }
            }

            map.readMaptoGrid(map.level_list.at(map.levelIndex),chell_SpawnPoint); //Leser kartet til en matrise med BlockType "MapGrid"
            chell.location = chell_SpawnPoint; // Setter startposisjonen til første spawnpoint
            map.blueIsPlaced = false;
            map.orangeIsPlaced = false;
        }

        if(screen == 1){ //Her spiller spillet som vanlig, uten å pause for menyen. 
            handleInput(window,chell,portalGun,collision,map,jumpframe,justPressedQ,justPressedE,screen,chell_SpawnPoint); //Håndterer intput og Chell kollisjoner
            portalGun.update(window); //Oppdaterer bevegelsen og posisjonen til Portalskudd, fjerner for out of bounds og kollisjoner
            collision.portalCollision(map,portalGun); //Oppdaterer vektor med portalbeskrivelse med portalkollisjoner
            updatePortalPositions(window,portalGun,map,collision); //Oppdaterer posisjonene til portalene etter at man har skutt
            updateChellPosition(window,chell,collision,map,jumpframe); //Trekker karakteren nedover med tyngdekraft 
            teleportUpdateChell(window,chell,map,collision);
            checkYCollision(window,chell,map,collision,jumpframe);

            map.drawMap(window,assets);
            chell.draw(assets,window);
            portalGun.draw(window);

            if(chell.nextLevel && map.levelIndex < 10){
                map.levelIndex += 1;
                map.blueIsPlaced = map.orangeIsPlaced = false;
                map.readMaptoGrid(map.level_list.at(map.levelIndex),chell_SpawnPoint); //Leser kartet til en matrise med BlockType "MapGrid"
                map.blueIsPlaced = false;
                map.orangeIsPlaced = false;
                chell.location = chell_SpawnPoint; 
                chell.nextLevel = false;
            }

            if(chell.nextLevel && map.levelIndex == 10){
                screen = 0;
                chell.nextLevel = false;
            }
        }

        else if(screen == 2){ //Pausemeny, med resume og quit to main menu.
            chell.isMoving = false;
            map.drawMap(window,assets);
            chell.draw(assets,window);
            portalGun.draw(window);

            map.drawPauseMenu(window,assets); //Tegner Pausemeny
            handleMouseInput(window,screen,mousePoint,justPressedMouse);
            if(mousePoint.x > 528 && mousePoint.x < 922 && mousePoint.y > 555 && mousePoint.y < 670){
                screen = 0; // Sender tilbake til Main Menu
            }
            else if(mousePoint.x > 480 && mousePoint.x < 960 && mousePoint.y > 235 && mousePoint.y < 350){
                screen = 1; //Stopper Pause og setter tilbake til spillemodus
            }
        }

        else if(screen == 3){ //Her har vi Controls menyen
            map.drawControlsMenu(window,assets);
            handleMouseInput(window,screen,mousePoint,justPressedMouse);
            if(mousePoint.x > 550 && mousePoint.x < 900 && mousePoint.y > 710 && mousePoint.y < 830){ //Back Knapp
                screen = 0; //Tilbake til main menu
            }
        }
        
        window.next_frame();
    }
};

void Aperture::handleInput(TDT4102::AnimationWindow& window,Chell& chell,PortalGun& portal, Collision& collision,Map& map,int& jumpframe,bool& justPressedQ,bool& justPressedE,int& screen,FloatingPoint& chell_spawnpoint){
    chell.collisionInfoX = collision.chellCollisionX(map,chell);

    if(window.is_key_down(KeyboardKey::LEFT) && !(chell.collisionInfoX.didCollide && chell.collisionInfoX.direction == Direction::Right)){
        chell.setDirection(Direction::Left);
        chell.isMoving = true;
        if(chell.velocity.x > -chell_movement_speed){
            chell.velocity.x -= walkAcc;
        }
    }
    
    if(window.is_key_down(KeyboardKey::RIGHT) && !(chell.collisionInfoX.didCollide && chell.collisionInfoX.direction == Direction::Left)){
        chell.isMoving = true;
        chell.setDirection(Direction::Right);
        if(chell.velocity.x < chell_movement_speed){
            chell.velocity.x += walkAcc;
        }
    }

    if(window.is_key_down(KeyboardKey::UP)){ 
        chell.isMoving = false;
        chell.setDirection(Direction::Up);
    }
    if(window.is_key_down(KeyboardKey::DOWN)){
        chell.isMoving = false;
        chell.setDirection(Direction::Down);
    }

    if(window.is_key_down(KeyboardKey::SPACE) && chell.onGround && !chell.isJumping){  
        chell.isJumping = true;
        jumpframe = 0;
    }

    if(window.is_key_down(KeyboardKey::SPACE) && chell.isJumping){
        if(jumpframe < 7){
            chell.velocity.y -= jumpAcc;
            jumpframe += 1;
        }
    }

    else{ 
        chell.isJumping = false; 
    }

    if(window.is_key_down(KeyboardKey::Q)){
        if(!justPressedQ){ 
            justPressedQ = true;
            portal.fire(chell.location,chell.facingDirection,true,chell.previousDirection);
        }
    }
    else{
        justPressedQ = false;
    }

    if(window.is_key_down(KeyboardKey::E)){
        if(!justPressedE){
            justPressedE = true;
            portal.fire(chell.location,chell.facingDirection,false,chell.previousDirection);
        }
    } 
    else{
        justPressedE = false;
    }

    if(!window.is_key_down(KeyboardKey::LEFT) && !window.is_key_down(KeyboardKey::RIGHT)){
        chell.isMoving = false;
    }

    if(window.is_key_down(KeyboardKey::ESCAPE)){
        screen = 2;
    }

    if(window.is_key_down(KeyboardKey::R)){
        chell.location = chell_spawnpoint;
        chell.velocity.x = 0;
        chell.velocity.y = 0;
    }
};


void Aperture::handleMouseInput(TDT4102::AnimationWindow& window,int& screen,TDT4102::Point& mousePoint,bool& justPressedMouse){
    bool didMousePress = window.is_left_mouse_button_down();

    if(didMousePress && !justPressedMouse){
        mousePoint = window.get_mouse_coordinates();
    }
    else{
        mousePoint = {0,0};
    }
    justPressedMouse = didMousePress;
}


void Aperture::updateChellPosition(TDT4102::AnimationWindow& window,Chell& chell,Collision& collision,Map& map,int& jumpframe){
    chell.onGround = (chell.velocity.y == 0);

    //Karakteren opplever friksjon om de er i kontakt med bakken, og luftmotstand ellers i x-retning. 
    if(chell.onGround && !window.is_key_down(KeyboardKey::LEFT) && !window.is_key_down(KeyboardKey::RIGHT)){
        chell.velocity.x *= friction; //Dette skaper en bug der man kan få VELDIG høy horisontal fart om man holder ned piltast, men den er litt morsom så jeg lar den være <3
    }
    else{
        chell.velocity.x *= airResistance;
    }

    //y-retning opplever alltid tyngdekraft og luftmotstand 
    chell.velocity.y *=airResistance;
    chell.velocity.y += gravityAcc;


    if(chell.velocity.y > maxFallSpeed){
        chell.velocity.y = maxFallSpeed;
    }

    chell.location.x += chell.velocity.x;

    chell.collisionInfoX = collision.chellCollisionX(map, chell);
    if(chell.collisionInfoX.didCollide){
        if(chell.collisionInfoX.type == BlockType::Cake){
            chell.nextLevel = true;
        }
        if((chell.collisionInfoX.direction == Direction::Left || chell.collisionInfoX.direction == Direction::Right)){
            if(!(chell.collisionInfoX.type == BlockType::BluePortal && chell.collisionInfoX.direction == map.blueDirection) 
            && !(chell.collisionInfoX.type == BlockType::OrangePortal && chell.collisionInfoX.direction == map.orangeDirection)){
                if(chell.velocity.x > 0){
                chell.location.x = map.block_width*chell.collisionInfoX.blockCol-chell_width;
                }
                else if(chell.velocity.x < 0){
                    chell.location.x = map.block_width*chell.collisionInfoX.blockCol+map.block_width;
                }
                chell.velocity.x = 0;
            }
        }
    }

    chell.location.y += chell.velocity.y;
};

void Aperture::checkYCollision(TDT4102::AnimationWindow& window, Chell& chell, Map& map, Collision& collision, int& jumpframe) {
    chell.collisionInfoY = collision.chellCollisionY(map, chell);
    
    if(chell.collisionInfoY.type == BlockType::Button && chell.collisionInfoY.direction == Direction::Up){
        map.buttonTimer = 0;
    }
    else if(map.buttonTimer < 80){
        map.buttonTimer += 1;
    }

    if(map.buttonTimer < 80){
        map.buttonOn = true;
    }
    else{
        map.buttonOn = false;
    }


    if(chell.collisionInfoY.didCollide){
        if(chell.collisionInfoY.type == BlockType::Cake){ //Sender spilleren til neste level om de treffer kaken
            chell.nextLevel = true;
        }

        chell.location.y -= chell.velocity.y;
        chell.velocity.y = 0; 
    }
}

void Aperture::updatePortalPositions(TDT4102::AnimationWindow& window,PortalGun& portalgun,Map& map,Collision& collision){
    collision.portalCollision(map,portalgun);
   std::erase_if(portalgun.portalProjInfo, [&](const auto& portalProjectile){
        if(portalProjectile.didCollide){
            if(portalProjectile.type == BlockType::White){
                if(portalProjectile.isBlue){
                    if(map.blueIsPlaced){
                        map.mapGrid.at(map.blueRow).at(map.blueCol) = BlockType::White; //Gjør den gamle portalen hvit 
                    }
                    map.blueIsPlaced = true;
                    map.blueCol = portalProjectile.blockCol;
                    map.blueRow = portalProjectile.blockRow;
                    map.blueDirection = portalProjectile.collisionDirection;
                    map.mapGrid.at(portalProjectile.blockRow).at(portalProjectile.blockCol) = BlockType::BluePortal;
                }
                else{
                    if(map.orangeIsPlaced){
                        map.mapGrid.at(map.orangeRow).at(map.orangeCol) = BlockType::White;
                    }
                    map.orangeIsPlaced = true;
                    map.orangeCol = portalProjectile.blockCol;
                    map.orangeRow = portalProjectile.blockRow;
                    map.orangeDirection = portalProjectile.collisionDirection;
                    map.mapGrid.at(portalProjectile.blockRow).at(portalProjectile.blockCol) = BlockType::OrangePortal;
                }
                return true; //Fjerner portalskudd fra liste 
            }
            else if(portalProjectile.type == BlockType::BluePortal){
                if(portalProjectile.isBlue){
                    map.blueDirection = portalProjectile.collisionDirection; //Oppdaterer portalretning
                }
                else{
                    map.blueIsPlaced = false; //Vi har ikke lengre en plassert blå portal

                    if(map.orangeIsPlaced){ //Dersom det fins en oransj portal, fjerner vi den
                        map.mapGrid.at(map.orangeRow).at(map.orangeCol) = BlockType::White;
                    }

                    map.orangeIsPlaced = true; //plasserer ny oransh portal
                    map.orangeCol = portalProjectile.blockCol;
                    map.orangeRow = portalProjectile.blockRow;
                    map.orangeDirection = portalProjectile.collisionDirection;

                    map.mapGrid.at(portalProjectile.blockRow).at(portalProjectile.blockCol) = BlockType::OrangePortal;
                }
                return true;
            }
            else if(portalProjectile.type == BlockType::OrangePortal){
                if(!portalProjectile.isBlue){
                    map.orangeDirection = portalProjectile.collisionDirection;
                }
                else{
                    map.orangeIsPlaced = false;

                    if(map.blueIsPlaced){
                        map.mapGrid.at(map.blueRow).at(map.blueCol) = BlockType::White;
                    }

                    map.blueIsPlaced = true;
                    map.blueCol = portalProjectile.blockCol; 
                    map.blueRow = portalProjectile.blockRow;
                    map.blueDirection = portalProjectile.collisionDirection;

                    map.mapGrid.at(portalProjectile.blockRow).at(portalProjectile.blockCol) = BlockType::BluePortal;
                }
                return true;
            }
        }
        return false;
    });
}


void Aperture::teleportUpdateChell(TDT4102::AnimationWindow& window,Chell&chell, Map&map,Collision& collision){
    chell.collisionInfoX = collision.chellCollisionX(map,chell);
    chell.collisionInfoY = collision.chellCollisionY(map,chell);

    if(chell.collisionInfoX.didCollide){ //Vi har kollisjon langs x
        if(chell.collisionInfoX.type == BlockType::BluePortal && chell.collisionInfoX.direction == map.blueDirection){
            if(map.orangeIsPlaced){
                chell.portalVelocitySwap(map.blueDirection,map.orangeDirection);
                if(map.orangeDirection == Direction::Right){
                chell.location = {static_cast<double>(map.orangeCol*map.block_width)+map.block_width,static_cast<double>(map.orangeRow*map.block_height)}; 
                }
                else if(map.orangeDirection == Direction::Left){
                    chell.location = {static_cast<double>(map.orangeCol*map.block_width)-chell_width,static_cast<double>(map.orangeRow*map.block_height)}; 
                }
                else if(map.orangeDirection == Direction::Up){
                    chell.location = {static_cast<double>(map.orangeCol*map.block_width),static_cast<double>(map.orangeRow*map.block_height)-chell_height}; 
                }
                else if(map.orangeDirection == Direction::Down){
                    chell.location = {static_cast<double>(map.orangeCol*map.block_width),static_cast<double>(map.orangeRow*map.block_height)+map.block_height}; 
                }
            }
            else{
                chell.location.x -= chell.velocity.x;
                chell.velocity.x = 0;
            }
        }
        else if(chell.collisionInfoX.type == BlockType::OrangePortal && chell.collisionInfoX.direction == map.orangeDirection){
            if(map.blueIsPlaced){
                chell.portalVelocitySwap(map.orangeDirection,map.blueDirection);
                if(map.blueDirection == Direction::Right){
                    chell.location = {static_cast<double>(map.blueCol*map.block_width)+map.block_width,static_cast<double>(map.blueRow*map.block_height)}; 
                }
                else if(map.blueDirection == Direction::Left){
                    chell.location = {static_cast<double>(map.blueCol*map.block_width)-chell_width,static_cast<double>(map.blueRow*map.block_height)}; 
                }
                else if(map.blueDirection == Direction::Up){
                    chell.location = {static_cast<double>(map.blueCol*map.block_width),static_cast<double>(map.blueRow*map.block_height)-chell_height}; 
                }
                else if(map.blueDirection == Direction::Down){
                    chell.location = {static_cast<double>(map.blueCol*map.block_width),static_cast<double>(map.blueRow*map.block_height)+map.block_height}; 
                }
            }
            else{
                chell.location.x -= chell.velocity.x;
                chell.velocity.x = 0;
            }
        }
    }

    if(chell.collisionInfoY.didCollide){ //Vi har kollisjon langs y
        if(chell.collisionInfoY.type == BlockType::BluePortal && chell.collisionInfoY.direction == map.blueDirection){
            if(map.orangeIsPlaced){
                chell.portalVelocitySwap(map.blueDirection,map.orangeDirection);
                if(map.orangeDirection == Direction::Right){
                chell.location = {static_cast<double>(map.orangeCol*map.block_width)+map.block_width,static_cast<double>(map.orangeRow*map.block_height)}; 
                }
                else if(map.orangeDirection == Direction::Left){
                    chell.location = {static_cast<double>(map.orangeCol*map.block_width)-chell_width,static_cast<double>(map.orangeRow*map.block_height)}; 
                }
                else if(map.orangeDirection == Direction::Up){
                    chell.location = {static_cast<double>(map.orangeCol*map.block_width),static_cast<double>(map.orangeRow*map.block_height)-chell_height}; 
                }
                else if(map.orangeDirection == Direction::Down){
                    chell.location = {static_cast<double>(map.orangeCol*map.block_width),static_cast<double>(map.orangeRow*map.block_height)+map.block_height}; 
                }
            }
            else{
                chell.location.y -= chell.velocity.y;
                chell.velocity.y = 0;
            }
        }
        else if(chell.collisionInfoY.type == BlockType::OrangePortal && chell.collisionInfoY.direction == map.orangeDirection){
            if(map.blueIsPlaced){
                chell.portalVelocitySwap(map.orangeDirection,map.blueDirection);
                if(map.blueDirection == Direction::Right){
                    chell.location = {static_cast<double>(map.blueCol*map.block_width)+map.block_width,static_cast<double>(map.blueRow*map.block_height)}; 
                }
                else if(map.blueDirection == Direction::Left){
                    chell.location = {static_cast<double>(map.blueCol*map.block_width)-chell_width,static_cast<double>(map.blueRow*map.block_height)}; 
                }
                else if(map.blueDirection == Direction::Up){
                    chell.location = {static_cast<double>(map.blueCol*map.block_width),static_cast<double>(map.blueRow*map.block_height)-chell_height}; 
                }
                else if(map.blueDirection == Direction::Down){
                    chell.location = {static_cast<double>(map.blueCol*map.block_width),static_cast<double>(map.blueRow*map.block_height)+map.block_height}; 
                }
            }
            else{
                chell.location.y -= chell.velocity.y;
                chell.velocity.y = 0;
            }
        }
    }
    

}