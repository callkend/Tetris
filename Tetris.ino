/*
Wyatt and Kendall
Tetris
*/

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <gamma.h>

#include "portableArcade.h"


enum GameState_e
{
    PRE_GAME,
    START_GAME,
    RUNNING_GAME,
    END_GAME,
} GameState;

// Defines the Matrix.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, MATRIX_OUTPUT_PIN,
                                               NEO_MATRIX_TOP + NEO_MATRIX_RIGHT +
                                                   NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                                               NEO_GRB + NEO_KHZ800);

const uint16_t LINE_COLOR = matrix.Color(17, 157, 242);
const uint16_t NORMAL_L_COLOR = matrix.Color(235, 74, 2);
const uint16_t BACKWARD_L_COLOR = matrix.Color(17, 44, 233);
const uint16_t NORMAL_Z_COLOR = matrix.Color(65, 173, 1);
const uint16_t BACKWARD_Z_COLOR = matrix.Color(221, 4, 31);
const uint16_t NORMAL_T_COLOR = matrix.Color(184, 24, 134);
const uint16_t SQUARE_COLOR = matrix.Color(252, 189, 26);
const uint16_t BACKGROUND_COLOR = matrix.Color(0, 0, 0);
const uint16_t WALL_COLOR = matrix.Color(119, 119, 119);

typedef enum
{
    LINE,
    NORMAL_L,
    BACKWARDS_L,
    NORMAL_Z,
    BACKWARDS_Z,
    NORMAL_T,
    SQUARE,
} Shape_e;

typedef enum {
    ROTATE_TRANSPOSE,
    ROTATE_NEGATE,
} ShapeRotate_e;

typedef struct
{   
    int8_t X;
    int8_t Y;
} ShapePoint_t;

typedef struct
{
   byte X : 4;
   byte Y : 4;
} Location_t;

#define POINTS_PER_SHAPE 4
typedef struct
{
    Shape_e Name;
    uint16_t Color;

    ShapePoint_t Points[POINTS_PER_SHAPE];

    ShapeRotate_e NextRotate;
} Shape_t;

Shape_t GetRandomShape(void)
{

    Shape_t result;
    result.Name = (Shape_e)random(7);
    
    switch (result.Name)
    {
    case LINE:
        result.Color = LINE_COLOR;
        result.Points[0] = {0, -3};
        result.Points[1] = {0, -1};
        result.Points[2] = {0, 1};
        result.Points[3] = {0, 3};
        break;
    case NORMAL_L:
        result.Color = NORMAL_L_COLOR;
        result.Points[0] = {-1, 2};
        result.Points[1] = {-1, 0};
        result.Points[2] = {-1, -2};
        result.Points[3] = {1, -2};
        break;
    case BACKWARDS_L:
        result.Color = BACKWARD_L_COLOR;
        result.Points[0] = {1, 2};
        result.Points[1] = {1, 0};
        result.Points[2] = {-1, -2};
        result.Points[3] = {1, -2};
        break;
    case NORMAL_T:
        result.Color = NORMAL_T_COLOR;
        result.Points[0] = {0, 1};
        result.Points[1] = {-2, -1};
        result.Points[2] = {0, -1};
        result.Points[3] = {2, -1};
        break;
    case NORMAL_Z:
        result.Color = NORMAL_Z_COLOR;
        result.Points[0] = {-2, 1};
        result.Points[1] = {0, 1};
        result.Points[2] = {0, -1};
        result.Points[3] = {2, -1};
        break;
    case BACKWARDS_Z:
        result.Color = BACKWARD_Z_COLOR;
        result.Points[0] = {0, 1};
        result.Points[1] = {2, 1};
        result.Points[2] = {-2, -1};
        result.Points[3] = {0, -1};
        break;
    case SQUARE:
        result.Color = SQUARE_COLOR;
        result.Points[0] = {1, 1};
        result.Points[3] = {1, -1};
        result.Points[1] = {-1, 1};
        result.Points[2] = {-1, -1};
        break;
    }
    return result;
}

Shape_t currentShape;
Shape_t nextShape;
Location_t playerOffset;

void RotateShape(Shape_t* shape){

    ShapePoint_t* point;
    if (shape->NextRotate == ROTATE_TRANSPOSE)
    {
        // Transpose the shape
        shape->NextRotate = ROTATE_NEGATE;
        uint8_t temp;
        for (int i = 0; i < POINTS_PER_SHAPE; ++i){
            point = &shape->Points[i];

            // Fancy XOR Swap;
            point->X ^= point->Y;
            point->Y ^= point->X;
            point->X ^= point->X;
        } 
    } else {
        // Negate the shape
        shape->NextRotate = ROTATE_TRANSPOSE;
        for (int i = 0; i < POINTS_PER_SHAPE; ++i){
            point = &shape->Points[i];

            point->X *= -1;
            point->Y *= -1;
        } 
    }
}

#define PreviewOffsetX 13
#define PreviewOffsetY 1
#define PreviewSizeX 3
#define PreviewSizeY 4

void DrawPreview(Shape_t shape)
{
    matrix.fillRect(PreviewOffsetX, PreviewOffsetY,
                    PreviewSizeX, PreviewSizeY, BACKGROUND_COLOR);

    const Location_t previewCenter = { PreviewOffsetX + 1, 
                                       PreviewSizeY + 2};

    DrawShape(previewCenter, shape);
}

Location_t* PlotShape (Location_t center, Shape_t shape){

    Location_t result[POINTS_PER_SHAPE];

    for (int i = 0; i < POINTS_PER_SHAPE; ++i)
    {
        ShapePoint_t location = shape.Points[i];
        location.X = location.X >> 1;
        location.Y = location.Y >> 1;

        result[i] = { (uint8_t)(location.X + (int8_t)center.X),
                      (uint8_t)(location.Y + (int8_t)center.Y) };
    }
}

void DrawPoints(Location_t* points, int count, int color){

    for (int i = 0; i < count; i++)
    {
        matrix.drawPixel(points->X, points->Y, color);
        ++points;
    }
}

void DrawShape(Location_t center, Shape_t shape)
{
    DrawPoints(PlotShape(center, shape), POINTS_PER_SHAPE, shape.Color);
}

void ClearShape(Location_t center, Shape_t shape)
{
    DrawPoints(PlotShape(center, shape), POINTS_PER_SHAPE, BACKGROUND_COLOR);
}

void setup()
{
    //Serial.begin(9600);
    randomSeed(analogRead(0));
    GameState = START_GAME;
    //Initializes the LED matrix, clears it, and setups the IO
    initPortableArcade(&matrix);
}

void loop()
{
    switch (GameState)
    {
    case PRE_GAME:
        /* code */
        break;
    default:
    case START_GAME:
        playerOffset.X = 5;
        playerOffset.Y = 0;
        matrix.fill(BACKGROUND_COLOR);
        nextShape = GetRandomShape();
        DrawPreview(nextShape);
        matrix.drawLine(0, 0, 0, 15, WALL_COLOR);
        matrix.drawLine(11, 0, 11, 15, WALL_COLOR);
        matrix.fillRect(11,6,15,15,WALL_COLOR);
        currentShape = GetRandomShape();
        matrix.show();
        GameState = RUNNING_GAME;
        break;
    case RUNNING_GAME:
    ++playerOffset.Y;
    DrawShape(playerOffset, currentShape);
    matrix.show();
    ClearShape(playerOffset, currentShape);
    if (playerOffset.Y == 15)
    {
        playerOffset.Y = 0;
        currentShape = nextShape;
        nextShape = GetRandomShape();
        DrawPreview(nextShape);
    }
    
        switch (GetDirection())
        {
        case LEFT:
        if(playerOffset.X > 1){
            --playerOffset.X;
        }
            break;
    
        case RIGHT:
        if(playerOffset.X < 8){
            ++playerOffset.X;
        }
            break;

        case DOWN:

            break;

        case UP:
            RotateShape(&currentShape);
            break;
        }
        break;
    case END_GAME:

        break;
    }
    delay(250);
}
