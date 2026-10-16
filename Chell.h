#pragma once
#include "Utilities.h"
#include "AnimationWindow.h"
#include "Assets.h"
#include "MovingObject.h"

namespace Aperture { 
    class Chell : public MovingObject{ //Spillekarakteren som arver location og velocity 
        public:
        bool nextLevel; //True om chell har kollidert med "cake" og skal videre til neste level
        bool isMoving; //Om Chell beveger seg eller ikke
        bool isJumping; //Om Chell hopper
        bool onGround; //Om Chell er på bakken
        int movingFrames = 0; //Tilsvarer hvor lenge chell har vært i bevegelse
        Direction previousDirection = Direction::Right; //Forrige retning chell pekte (viktig for idle drawing)
        Direction facingDirection = Direction::Right;
        CollisionInfo collisionInfoX; 
        CollisionInfo collisionInfoY;
    
        void draw(Aperture::Assets& assets,TDT4102::AnimationWindow& window); //Tegner figuren
        void setDirection(Aperture::Direction direction); //Endrer retningen (Trenger ikke nødvendigvis denne fordi er public, men jaja)
        void portalVelocitySwap(Direction inDirection,Direction outDirection); //Bevarer bevegelsesmengde gjennom portaler
    };
}