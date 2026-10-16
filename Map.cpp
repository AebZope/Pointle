#include "Map.h"
#include <fstream>
#include <iostream>
#include <filesystem>

void Aperture::Map::readMaptoGrid(const std::string& filename,FloatingPoint& startloc){
    switchRow.clear(); //Tømmer både row og col vektor
    switchCol.clear();
    buttonTimer = 500; //Passer på å skru av knapper

    std::string pathToMap = "res/maps/"; //path til alle maps
    pathToMap += filename;
    std::ifstream file(pathToMap); 


    try{
        if(!file){
            throw 1; //Feil i fillesning
        }
        std::string line;

        mapGrid.clear(); //Passer på å fjerne gamle grid, slik at man ikke får to kart

        file >> startloc.x >> startloc.y;
        if(file.fail()){
            throw 2; //Feil i Løsning av startposisjon
        }
        file.ignore();

        
        int tempRow = 0;

        while(std::getline(file,line)){
            std::vector<BlockType> row;
            int tempCol = 0;

            for(char c : line){
                switch(c){
                    case '#': row.push_back(BlockType::Solid); break;
                    case 'G': row.push_back(BlockType::Glass); break;
                    case 'W': row.push_back(BlockType::White); break;
                    case '.': row.push_back(BlockType::Air); break;
                    case 'C': row.push_back(BlockType::Cake); break; 
                    case 'B': row.push_back(BlockType::Button); break;
                    case 'S': row.push_back(BlockType::Solid); 
                            switchRow.push_back(tempRow); //Legger til hvor switch blokkene skal være
                            switchCol.push_back(tempCol);
                            break;
                }
                tempCol += 1;
            }
            mapGrid.push_back(row);
            tempRow += 1;
        }
    }
    catch(int error){
        if(error == 1){
            std::cout << "Could not read file: " << pathToMap << std::endl;
        }
        else if(error == 2){
            std::cout << "Could not read SpawnPoint in file: " << pathToMap << std::endl;
        }

    }
    
};

void Aperture::Map::drawMap(TDT4102::AnimationWindow& window,Assets& assets){
    if(buttonOn){
        for(int i = 0; i < switchRow.size(); i++){
            mapGrid.at(switchRow.at(i)).at(switchCol.at(i)) = BlockType::Air;
        }
    }
    else{
        for(int i = 0; i < switchRow.size(); i++){
            mapGrid.at(switchRow.at(i)).at(switchCol.at(i)) = BlockType::Solid;
        }
    }


    for(int y = 0; y < mapGrid.size(); y++){
        for(int x = 0; x < mapGrid.at(0).size();x++){
            TDT4102::Point drawPoint = {x*block_width,y*block_height}; //gir øverste hjørne av blokk
            BlockType currentType = mapGrid.at(y).at(x);
            if(currentType == BlockType::Solid){
                window.draw_image(drawPoint,assets.Solid,block_width,block_height);
            }
            else if(currentType == BlockType::Glass){
                window.draw_image(drawPoint,assets.Grid,block_width,block_height);
            }
            else if(currentType == BlockType::White){
                window.draw_image(drawPoint,assets.White,block_width,block_height);
            }
            else if(currentType == BlockType::BluePortal){
                if(blueDirection == Direction::Left){
                    window.draw_image(drawPoint,assets.BluePortalLeft,block_width,block_height);
                }
                if(blueDirection == Direction::Right){
                    window.draw_image(drawPoint,assets.BluePortalRight,block_width,block_height);
                }
                if(blueDirection == Direction::Up){
                    window.draw_image(drawPoint,assets.BluePortalUp,block_width,block_height);
                }
                if(blueDirection == Direction::Down){
                    window.draw_image(drawPoint,assets.BluePortalDown,block_width,block_height);
                }
            }
            else if(currentType == BlockType::OrangePortal){
                if(orangeDirection == Direction::Left){
                    window.draw_image(drawPoint,assets.OrangePortalLeft,block_width,block_height);
                }
                if(orangeDirection == Direction::Right){
                    window.draw_image(drawPoint,assets.OrangePortalRight,block_width,block_height);
                }
                if(orangeDirection == Direction::Up){
                    window.draw_image(drawPoint,assets.OrangePortalUp,block_width,block_height);
                }
                if(orangeDirection == Direction::Down){
                    window.draw_image(drawPoint,assets.OrangePortalDown,block_width,block_height);
                }
            }
            else if(currentType == BlockType::Cake){
                window.draw_image(drawPoint,assets.Cake,block_width,block_height);
            }
            else if(currentType == BlockType::Button){
                window.draw_image(drawPoint,assets.Button,block_width,block_height);
            }
        }
    }
};

void Aperture::Map::drawPauseMenu(TDT4102::AnimationWindow& window,Assets& assets){
    TDT4102::Point zeroPoint = {0,0};
    window.draw_image(zeroPoint,assets.PauseMenu,window.width(),window.height());
}

void Aperture::Map::drawMainMenu(TDT4102::AnimationWindow& window,Assets& assets){
    TDT4102::Point zeroPoint = {0,0};
    window.draw_image(zeroPoint,assets.MainMenu,window.width(),window.height());
};

void Aperture::Map::drawControlsMenu(TDT4102::AnimationWindow& window,Assets& assets){
    TDT4102::Point zeroPoint = {0,0};
    window.draw_image(zeroPoint,assets.ControlsMenu,window.width(),window.height());
};