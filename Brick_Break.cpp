/*
    Kendall Callister
    5/27/22
    Brick Break
    The goal of Brick Break is to destroy the bricks at the top of screen using a ball that needs to be bounced using
    the players paddle. Each level cleared increases the number of bricks on the screen and the number of balls that need
    to be juggled.
*/

//Libaries
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "portableArcade.h"
#include "games.h"

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define ZigZag

#define GameSizeX 14
#define GameSizeY 15

typedef enum
{
    NO_COLLISION,
    RIGHT_COLLISION,
    LEFT_COLLISION,
    TOP_COLLISION,
    BOTTOM_COLLISION,
}

extern Adafruit_NeoMatrix matrix;

//Colors
//Set Color Red
const uint16_t COLOR_RED = matrix.Color(255, 0, 0);
//Set Color Green
const uint16_t COLOR_GREEN = matrix.Color(0, 255, 0);
//Set Color Blue
const uint16_t COLOR_BLUE = matrix.Color(0, 0, 255);

// Sets GREEN, RED, BLUE rotating colors for scrolling text.
const uint16_t colors[] = {COLOR_RED,
                           COLOR_GREEN,
                           COLOR_BLUE};

//Sets Snake Color
const uint16_t SNAKE_COLOR = matrix.Color(255, 102, 0);
//Sets Cherry Color
const uint16_t BALL_COLOR = matrix.Color(173, 19, 206);

Direction_e PaddleDirection = NO_DIRECTION;
Direction_e BallDirection = NO_DIRECTION;

void brickBreakSetup()
{
  // Randomizes where the ball will start
  randomSeed(analogRead(0));

  //Initializes the LED matrix, clears it, and setups the IO
  initPortableArcade(&matrix);
}

bool brickBreakLoop()
{
    //
    delay(1);

}