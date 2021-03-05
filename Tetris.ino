/*
Wyatt and Kendall
Tetris
*/

#include <Adafruit_GFX_Library>
#include <Adafruit_NeoMatrix>
#include <Adafruit_NeoPixel>
#include "portableArcade.h"

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

enum { PRE_GAME, START_GAME, RUNNING_GAME, END_GAME, } GameState;

// Defines the Matrix.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, MATRIX_OUTPUT_PIN,
                                                NEO_MATRIX_TOP + NEO_MATRIX_RIGHT +
                                                NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                                                NEO_GRB + NEO_KHZ800);

void Setup()
{
    randomSeed(analogRead(0))

    //Intializes the LED matrix, clears it, and setups the IO
    initPortableArcade(matix);
}

void Loop()
{
switch ()
{
case PRE_GAME:
    /* code */
    break;
default:
case START_GAME:

    break;
case RUN_GAME:

    break;
case END_GAME:

    break;
}
}