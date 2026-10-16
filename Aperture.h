#pragma once 
#include "AnimationWindow.h"
#include "Chell.h"
#include "Map.h"
#include "PortalGun.h"
#include "Interactables.h"
#include "Utilities.h"
#include "Assets.h"




namespace Aperture{
    void run();
    void handleInput(TDT4102::AnimationWindow& window, Chell& chell,PortalGun& portal,Collision& collision,Map& map,int& jumpframe,bool& justPressedQ,bool& justPressedE, int& screen,FloatingPoint& chell_spawnpoint);
    void handleMouseInput(TDT4102::AnimationWindow& window,int& screen,TDT4102::Point& mousePoint,bool& justPressedMouse);
    void updateChellPosition(TDT4102::AnimationWindow& window,Chell& chell,Collision& collision,Map&map,int& jumpframe);
    void updatePortalPositions(TDT4102::AnimationWindow& window,PortalGun& portalgun,Map& map,Collision& collision);
    void teleportUpdateChell(TDT4102::AnimationWindow& window,Chell& chell, Map&map,Collision& collision);
    void checkYCollision(TDT4102::AnimationWindow& window,Chell& chell, Map&map,Collision& collision, int& jumpframe);

}