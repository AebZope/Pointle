#pragma once 

namespace Aperture {

    static const double chell_movement_speed = 4;
    static const double portalGun_movement_speed = 30;
    static const double gravityAcc = 0.3;
    static const double walkAcc = 0.25;
    static const double jumpAcc = 1.5;
    static const double friction = 0.50;
    static const double airResistance = 0.998;
    static const double maxFallSpeed =35;
    static const double maxVelocity = 35;
    

    static const int chell_width = 70;
    static const int chell_height = 90;

    static const int portalgun_width = 8;
    static const int portalgun_height = 5;

    enum class BlockType {
        Solid, White, Glass, BluePortal, OrangePortal, Air, Cake, Button
    };

    enum class Direction {
        Right,Left,Up,Down,None //La til None for å skjekke for ingen kollisjon, har ingenting å si for Chell. 
    };

    struct FloatingPoint {
        double x = 0; 
        double y = 0;
    }; 

    struct CollisionInfo{ //Info for kollisjon av karakter, om de kolliderte, retninge kollisjonen er fra, typen blokk, og indekseringen til blokken som ble kolliderte med 
        bool didCollide;
        Direction direction;
        BlockType type;
        int blockRow;
        int blockCol;
    };



    struct collisionGrid{ //Beskriver systemet som noe kan kollidere i, koordinater til hjørnene til alle ikke luft blokker, og typen blokk samt indekseringen 
        double topLeftX, topLeftY;
        double topRightX, topRightY;
        double bottomLeftX, bottomLeftY;
        double bottomRightX, bottomRightY;
        int row, col;
        BlockType type;
    };
}