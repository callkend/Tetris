

#ifndef PORTABLE_ARCADE_H
#define PORTABLE_ARCADE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>

// Defines the pin in which the data is sent to the matrix.
#define MATRIX_OUTPUT_PIN 6

#define JOYSTICK_DOWN_PIN 13
#define JOYSTICK_UP_PIN 12
#define JOYSTICK_RIGHT_PIN 11
#define JOYSTICK_LEFT_PIN 10

#define SCORE_BOARD_RESET_PIN 0

#define GOBBLE_CLOCK_PIN 1
#define GOBBLE_UPDOWN_SELECT_PIN 2

#define SCORE_COUNT_PIN 3
#define SCORE_100_PIN 4

typedef enum
{
  NO_DIRECTION,
  UP,
  DOWN,
  LEFT,
  RIGHT,
} Direction_e;

void initPortableArcade(Adafruit_NeoMatrix* matix);

Direction_e GetDirection(void);

#ifdef	__cplusplus
}
#endif

#endif // PORTABLE_ARCADE_H
