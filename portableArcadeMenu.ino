/*
* kendcall
* A Menu to switch between the game loaded on to the portable arcade
*/

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT_Macros.h>

#include "Tetris.h"
#include "Snake.h"
#include "portableArcade.h"

/** @brief A enum to track which menu is being used*/
enum MenuSelect_e
{
    GAME_SELECT,
    TETRIS_MENU,
    SNAKE_MENU,
} MenuSelect;

/** @brief The object used to change the LEDs */
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, MATRIX_OUTPUT_PIN,
                                               NEO_MATRIX_TOP + NEO_MATRIX_RIGHT +
                                               NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                                               NEO_GRB + NEO_KHZ800);

void setup(){
    initPortableArcade(&matrix);
    MenuSelect = GAME_SELECT;
}

/*
MAIN MENU  ->   Snake   ->  Easy    -> Start Snake Easy
                            Hard    -> Start Snake Hard
                
                Tetris  ->  Start Tetris
*/

void loop(){
    static int8_t textIndex = 0;
    static uint8_t selectedMenuItem = 0;

    switch (MenuSelect)
    {
    case GAME_SELECT:
        matrix.fillScreen(BACKGROUND_COLOR);
        matrix.setCursor(textIndex, 0);
        switch (selectedMenuItem)
        {
        case 0:
            matrix.setTextColor(matrix.Color(255, 93, 21));
            matrix.print("Tetris");
            matrix.setCursor(textIndex, 8);
            matrix.setTextColor(matrix.Color(93, 255, 21));
            matrix.print("Snake");
            if (GetDirection() == RIGHT){
                MenuSelect = TETRIS_MENU;
            }
            break;
        
        case 1:
            matrix.setTextColor(matrix.Color(93, 255, 21));
            matrix.print("Tetris");
            matrix.setCursor(textIndex, 8);
            matrix.setTextColor(matrix.Color(255, 93, 21));
            matrix.print("Snake");
            if (GetDirection() == RIGHT){
                MenuSelect = SNAKE_MENU;
            }
            break;
        }

        if (GetDirection() == UP)
            selectedMenuItem = 0;

        if (GetDirection() == DOWN)
            selectedMenuItem = 1;
    if (--textIndex < -34)
    {
        textIndex = matrix.width();
    }
        break;
    
    case TETRIS_MENU:
        matrix.fillScreen(BACKGROUND_COLOR);
        matrix.setCursor(textIndex, 0);

        matrix.setTextColor(matrix.Color(255, 93, 21));
        matrix.print("Start");
        matrix.setCursor(textIndex, 8);
        matrix.setTextColor(matrix.Color(255, 93, 21));
        matrix.print("Tetris");

        if (GetDirection() == RIGHT){

            tetrisSetup();

            while (tetrisLoop());
        }
        if (GetDirection()== LEFT){
            MenuSelect = GAME_SELECT;
        }
        

        if (--textIndex < -34)
        {
            textIndex = matrix.width();
        }
        break;
        case SNAKE_MENU:
        matrix.fillScreen(BACKGROUND_COLOR);
        matrix.setCursor(textIndex, 0);

        matrix.setTextColor(matrix.Color(255, 93, 21));
        matrix.print("Start");
        matrix.setCursor(textIndex, 8);
        matrix.setTextColor(matrix.Color(255, 93, 21));
        matrix.print("Snake");

        if (GetDirection() == RIGHT){

            snakeSetup();

            while (snakeLoop());
        }
        if (GetDirection()== LEFT){
            MenuSelect = GAME_SELECT;
        }
        

        if (--textIndex < -34)
        {
            textIndex = matrix.width();
        }
        break;
    }

        matrix.show();
        
        delay(50);
}