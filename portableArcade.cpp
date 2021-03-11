
#include <Adafruit_NeoMatrix.h>
#include "portableArcade.h"

void initPortableArcade(Adafruit_NeoMatrix *matrix)
{
  // Tell matrix to start, clear the screen, and set the Brightness of the martix.
  matrix->begin();
  matrix->fillScreen(0);
  matrix->setTextWrap(false);
  matrix->setBrightness(40);
  matrix->setTextColor(matrix->Color(128, 0, 0));
  matrix->show();

  //PULLUPs the controller so that when direction made that arduino takes input.
  pinMode(JOYSTICK_DOWN_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_UP_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_RIGHT_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_LEFT_PIN, INPUT_PULLUP);

  //Setups the pins need to interface with external digital logic that will display information about the game.
  pinMode(SCORE_BOARD_RESET_PIN, OUTPUT);
  pinMode(GOBBLE_CLOCK_PIN, OUTPUT);
  pinMode(GOBBLE_UPDOWN_SELECT_PIN, OUTPUT);
  pinMode(SCORE_COUNT_PIN, OUTPUT);
  pinMode(SCORE_100_PIN, OUTPUT);

  // Allows for the arduino to talk to the computer.
  //Serial.begin(9600);
}

Direction_e GetDirection(void)
{
  if (!digitalRead(JOYSTICK_DOWN_PIN))
  {
    return DOWN;
  }
  else if (!digitalRead(JOYSTICK_UP_PIN))
  {
    return UP;
  }
  else if (!digitalRead(JOYSTICK_RIGHT_PIN))
  {
    return RIGHT;
  }
  else if (!digitalRead(JOYSTICK_LEFT_PIN))
  {
    return LEFT;
  }
  else
  {
    return NO_DIRECTION;
  }
}