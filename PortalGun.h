#pragma once
#include "Utilities.h"
#include "AnimationWindow.h"
#include "MovingObject.h"


namespace Aperture {
    class PortalProjectile : public MovingObject{ //Individuell portal "skudd" som arver location og velocity 
        public:
        bool isBlue;
        bool didCollide; //Viser til om hver projectile har kollidert og hva med. 
        Aperture::BlockType type; 
        Aperture::Direction collisionDirection;
        int blockRow;
        int blockCol;
    };
}


namespace Aperture {
    class PortalGun{
    public: 
    std::vector<PortalProjectile> portalProjInfo;
    void fire(FloatingPoint startLocation, Direction direction,bool isBlue,Direction& previousDirection);
    void update(TDT4102::AnimationWindow& window);
    void draw(TDT4102::AnimationWindow& window);
    };
    
}