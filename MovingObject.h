#pragma once 
#include "AnimationWindow.h" 
#include "Utilities.h"


namespace Aperture{

  class MovingObject{ //Class som Chell og PortalProjectile kan arve fra for å endre på posisjon
        public:
        FloatingPoint location; //Floatingpoint som tilsvarer posisjonen til et object
        FloatingPoint velocity; //Tilsvarende for hastigheten
    };
}
