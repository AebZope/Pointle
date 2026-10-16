#pragma once
#include "Utilities.h"
#include "Chell.h"
#include "PortalGun.h"
#include "Map.h"

namespace Aperture{
    class Collision{    
        public: 

        CollisionInfo chellCollisionX(Map& map,Chell& chell);
        CollisionInfo chellCollisionY(Map&map,Chell& chell);

        void portalCollision(Map& map,PortalGun& portalgun); //tilsvarende for portal
              
    };
}