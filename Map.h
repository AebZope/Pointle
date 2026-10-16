#pragma once 
#include <vector>
#include <string>
#include "Utilities.h"
#include "AnimationWindow.h"
#include "Assets.h"

namespace Aperture { 
    class Map {
        public:
        int block_width = 120;
        int block_height = 120;

        //Gir retningen Portalen er på blokken, man skal kun kunne gå inn og ut her. 
        Direction orangeDirection; 
        Direction blueDirection;

        //Gir indekseringen til Portalene, sånn at de kan endres ved skyting
        int orangeRow; 
        int orangeCol;
        int blueRow;
        int blueCol;

        //Gir om portalene er plassert eller ikke for å unngå forvirring i teleportasjon
        bool blueIsPlaced = false;
        bool orangeIsPlaced = false;
        bool buttonOn; //Knappen man står på, om den er på eller ikke

        int buttonTimer;

        std::vector<std::string> level_list = {"playground.map","level1.map","level2.map",
                "level3.map","level4.map","level5.map","level6.map","level7.map","level8.map","level9.map","level10.map"};
        int levelIndex; //Indeks i levelliste for hvilken level man er på

        std::vector<int> switchRow; //Vektor med verdier for col og row av Switch, der switch er blokker som skal endres basert på knapp
        std::vector<int> switchCol;
        
        std::vector<std::vector<BlockType>> mapGrid; 

        void readMaptoGrid(const std::string& filename,FloatingPoint& startloc);
        void drawMap(TDT4102::AnimationWindow& window,Assets& assets);
        void placePortal(); //Må lages, tar koordinatene til ny portal og plasserer en portal der, må fjerne gamle portal også
        void drawPauseMenu(TDT4102::AnimationWindow& window,Assets& assets);
        void drawMainMenu(TDT4102::AnimationWindow& window,Assets& assets);
        void drawControlsMenu(TDT4102::AnimationWindow& window,Assets& assets);
    };
}