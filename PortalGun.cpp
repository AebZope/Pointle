#include "PortalGun.h"

void Aperture::PortalGun::fire(FloatingPoint startLocation, Direction direction,bool blue,Direction& previousDirection){
    PortalProjectile portalProjectile;
    portalProjectile.location = startLocation;
    portalProjectile.isBlue = blue;
    portalProjectile.velocity = {0,0};
    portalProjectile.didCollide = false; //I utgangspunktet har den ikke kollidert
    portalProjectile.type = BlockType::Air;
    portalProjectile.collisionDirection = Direction::None;
    
    if(direction == Direction::Right){
        portalProjectile.velocity.x = portalGun_movement_speed;
        portalProjectile.location.x += 55; //Korrigerer hvor skuddet starter, så det passer med tegnet figur istedenfor å starte fra top left av figure
        portalProjectile.location.y += 44;
    }
    else if(direction == Direction::Left){
        portalProjectile.velocity.x = -portalGun_movement_speed;
        portalProjectile.location.x += 5;
        portalProjectile.location.y += 44;
    }
    else if(direction == Direction::Up){
        if(previousDirection == Direction::Right){
            portalProjectile.location.x += 43;
            portalProjectile.location.y += 5;
        }
        else{
            portalProjectile.location.x += 17;
            portalProjectile.location.y += 5;
        }
        portalProjectile.velocity.y = -portalGun_movement_speed;
    }
    else if(direction == Direction::Down){
        if(previousDirection == Direction::Right){
            portalProjectile.location.x += 35;
            portalProjectile.location.y += 70;
        }
        else{
            portalProjectile.location.x += 25;
            portalProjectile.location.y += 70;
        }
        portalProjectile.velocity.y = portalGun_movement_speed;
    }

    portalProjInfo.push_back(portalProjectile); //Legger til i en liste, slik at jeg kan ha flere på skjermen samtidig. 
};

void Aperture::PortalGun::update(TDT4102::AnimationWindow& window){ //Oppdaterer posisjon og fjerning av skudd som kolliderer
    for(auto& portalProjectile : portalProjInfo){
        portalProjectile.location.x += portalProjectile.velocity.x;
        portalProjectile.location.y += portalProjectile.velocity.y;
    }

    std::erase_if(portalProjInfo, [&](const PortalProjectile& portalprojectile){ //Fjerner portalskudd ut av range og som kolliderer med solid
        return portalprojectile.location.x < 0 || portalprojectile.location.x > window.width()
            || portalprojectile.location.y < 0 || portalprojectile.location.y > window.height()
            || portalprojectile.type == BlockType::Solid || portalprojectile.type == BlockType::BluePortal
            || portalprojectile.type == BlockType::OrangePortal || portalprojectile.type == BlockType::Button;});

}

void Aperture::PortalGun::draw(TDT4102::AnimationWindow& window){
    
    for(auto& portalprojectile : portalProjInfo){
        TDT4102::Point currentLocation = {static_cast<int>(portalprojectile.location.x),static_cast<int>(portalprojectile.location.y)};

        if(portalprojectile.isBlue){
        window.draw_rectangle(currentLocation,portalgun_width,portalgun_height,TDT4102::Color::blue);
        }
        else{
        window.draw_rectangle(currentLocation,portalgun_width,portalgun_height,TDT4102::Color::orange);       
        }
    }
};