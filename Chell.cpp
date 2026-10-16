#include "Chell.h"

void Aperture::Chell::draw(Aperture::Assets& assets,TDT4102::AnimationWindow& window){
    TDT4102::Point currentLocation = {static_cast<int>(location.x),static_cast<int>(location.y)};

    if(facingDirection == Direction::Right){
        previousDirection = Direction::Right; //Viktig for at opp og ned ser i riktig retning
        if(isMoving){
            movingFrames += 1;
            if((movingFrames/10)%2 == 0){
                window.draw_image(currentLocation,assets.ChellRightRun1,chell_width,chell_height);
            }
            else{
                window.draw_image(currentLocation,assets.ChellRightRun2,chell_width,chell_height);
            }
        }
        else{
            window.draw_image(currentLocation,assets.ChellRightIdle,chell_width,chell_height);
        }
    }
    else if(facingDirection == Direction::Left){
        previousDirection = Direction::Left;
        if(isMoving){
            movingFrames += 1;
            if((movingFrames/10)%2 == 0){
                window.draw_image(currentLocation,assets.ChellLeftRun1,chell_width,chell_height);
            }
            else{
                window.draw_image(currentLocation,assets.ChellLeftRun2,chell_width,chell_height);
            }
        }
        else{
            window.draw_image(currentLocation,assets.ChellLeftIdle,chell_width,chell_height);
        }
    }
    else if(facingDirection == Direction::Up){
        if(previousDirection == Direction::Right){
            window.draw_image(currentLocation,assets.ChellUpRightIdle,chell_width,chell_height);
        }
        else{
            window.draw_image(currentLocation,assets.ChellUpLeftIdle,chell_width,chell_height);
        }
        
    }
    else if(facingDirection == Direction::Down){
        if(previousDirection == Direction::Right){
            window.draw_image(currentLocation,assets.ChellDownRightIdle,chell_width,chell_height);
        }
        else{
            window.draw_image(currentLocation,assets.ChellDownLeftIdle,chell_width,chell_height);
        }
    }
}

void Aperture::Chell::setDirection(Aperture::Direction direction){
    facingDirection = direction;
};


void Aperture::Chell::portalVelocitySwap(Direction inDirection,Direction outDirection){ 
    //bytter på hastigheten så det gir mening fysisk for portalene
    double vx = velocity.x;
    double vy = velocity.y;

    if(inDirection == Direction::Right){
        if(outDirection == Direction::Up){
            velocity.x = vy;
            velocity.y = vx;
        }
        if(outDirection == Direction::Down){
            velocity.x = vy;
            velocity.y = -vx;
        }
        if(outDirection == Direction::Right){
            velocity.x = -vx;
            velocity.y = vy;
            facingDirection = Direction::Right;
        }
    }

    if(inDirection == Direction::Left){
        if(outDirection == Direction::Up){
            velocity.x = vy;
            velocity.y = -vx;
        }
        if(outDirection == Direction::Down){
            velocity.x = vy;
            velocity.y = vx;
        }
        if(outDirection == Direction::Left){
            velocity.x = -vx;
            velocity.y = vy;
            facingDirection = Direction::Left;
        }
    }

    if(inDirection == Direction::Up){
        if(outDirection == Direction::Up){
            velocity.x = vx;
            velocity.y = -vy;
        }
        if(outDirection == Direction::Right){
            velocity.x = vy;
            velocity.y = vx;
            facingDirection = Direction::Right;
        }
        if(outDirection == Direction::Left){
            velocity.x = -vy;
            velocity.y = vx;
            facingDirection = Direction::Left;
        }
    }

    if(inDirection == Direction::Down){
        if(outDirection == Direction::Down){
            velocity.x = vx;
            velocity.y = -vy;
        }
        if(outDirection == Direction::Right){
            velocity.x = -vy;
            velocity.y = vx;
            facingDirection = Direction::Right;
        }
        if(outDirection == Direction::Left){
            velocity.x = vy;
            velocity.y = vx;
            facingDirection = Direction::Left;
        }
    }



};