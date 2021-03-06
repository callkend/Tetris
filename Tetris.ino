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
    NO_COLLISION = 0,
    COLLISION_ON_RIGHT = 1,
    COLLISION_ON_LEFT = 2,
    COLLISION_ON_BOTTOM = 4,
} Collision_e;

typedef enum
{
    LINE,
    NORMAL_L,
    BACKWARDS_L,
    NORMAL_Z,
    BACKWARDS_Z,
    NORMAL_T,
    SQUARE,
    NO_SHAPE,
} Shape_e;

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
} Shape_t;

typedef union{

    struct{
        uint8_t R;
        uint8_t G;
        uint8_t B;
        uint8_t W;
    };

    struct {
        uint32_t All;
    };
} Color_t;
 
uint16_t Color32to16(uint32_t input){

    Color_t c ;
    c.All = input;

    return ((uint16_t)(c.R & 0xF8) << 8) | ((uint16_t)(c.G & 0xFC) << 3) | (c.B>> 3);
}

Shape_t GetRandomShape(void)
{

    Shape_t result;
    result.Name = (Shape_e)random(7);

    switch (result.Name)
    {
    case LINE:
        result.Color = LINE_COLOR;
        result.Points[0] = {0, 3};
        result.Points[1] = {0, 1};
        result.Points[2] = {0, -1};
        result.Points[3] = {0, -3};
        break;
    case NORMAL_L:
        result.Color = NORMAL_L_COLOR;
        result.Points[0] = {-1, -2};
        result.Points[1] = {-1, 0};
        result.Points[2] = {-1, 2};
        result.Points[3] = {1, 2};
        break;
    case BACKWARDS_L:
        result.Color = BACKWARD_L_COLOR;
        result.Points[0] = {1, -2};
        result.Points[1] = {1, 0};
        result.Points[2] = {-1, 2};
        result.Points[3] = {1, 2};
        break;
    case NORMAL_T:
        result.Color = NORMAL_T_COLOR;
        result.Points[0] = {0, -1};
        result.Points[1] = {-2, 1};
        result.Points[2] = {0, 1};
        result.Points[3] = {2, 1};
        break;
    case NORMAL_Z:
        result.Color = NORMAL_Z_COLOR;
        result.Points[0] = {-2, -1};
        result.Points[1] = {0, -1};
        result.Points[2] = {0, 1};
        result.Points[3] = {2, 1};
        break;
    case BACKWARDS_Z:
        result.Color = BACKWARD_Z_COLOR;
        result.Points[0] = {0, -1};
        result.Points[1] = {2, -1};
        result.Points[2] = {-2, 1};
        result.Points[3] = {0, 1};
        break;
    case SQUARE:
        result.Color = SQUARE_COLOR;
        result.Points[0] = {1, -1};
        result.Points[3] = {1, 1};
        result.Points[1] = {-1, -1};
        result.Points[2] = {-1, 1};
        break;
    }
    return result;
}

Shape_t currentShape;
Shape_t nextShape;
Location_t playerOffset;

ShapePoint_t TransposePoint(ShapePoint_t point)
{
    int8_t temp = point.X;
    point.X = point.Y;
    point.Y = temp;
    return point;
}

ShapePoint_t RotatePointClockwise(ShapePoint_t point)
{

    point.Y *= -1;
    point = TransposePoint(point);
    return point;
}

ShapePoint_t RotatePointAntiClockwise(ShapePoint_t point)
{

    point = TransposePoint(point);
    point.Y *= -1;
    return point;
}

void RotateShape(Shape_t *shape)
{

    ShapePoint_t *point;

    for (int y = 0; y < POINTS_PER_SHAPE; ++y)
    {
        point = &shape->Points[y];
        *point = RotatePointClockwise(*point);
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

    const Location_t previewCenter = {PreviewOffsetX + 1,
                                      PreviewOffsetY + 2};

    DrawShape(previewCenter, shape);
}

Location_t *PlotShape(Location_t center, Shape_t shape)
{

    static Location_t result[POINTS_PER_SHAPE];

    for (int y = 0; y < POINTS_PER_SHAPE; ++y)
    {
        ShapePoint_t location = shape.Points[y];
        location.X = location.X >> 1;
        location.Y = location.Y >> 1;

        result[y] = {(uint8_t)(location.X + (int8_t)center.X),
                     (uint8_t)(location.Y + (int8_t)center.Y)};
    }

    return result;
}

void DrawPoints(Location_t *points, int count, int color)
{

    for (int y = 0; y < count; y++)
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

#define GameOffsetX 1
#define GameOffsetY 0
#define GameSizeX 10
#define GameSizeY 16

uint8_t LineErase()
{
    uint8_t linesCleared = 0;
    int fillCount;

    Location_t playableSpace;
    for (int y = (GameOffsetY + GameSizeY - 1); y >= GameOffsetY; --y)
    {
        playableSpace.Y = y;
        fillCount = 0;
        for (int x = GameOffsetX; x < (GameOffsetX + GameSizeX); ++x)
        {
            playableSpace.X = x;
            uint16_t pixel = GetPixel(playableSpace);

            if (pixel != BACKGROUND_COLOR)
            {
                ++fillCount;
            }

            if (linesCleared > 0)
            {
                // Shift pixel
                matrix.drawPixel(playableSpace.X, playableSpace.Y + linesCleared, pixel);
            }
        }

        if (fillCount >= GameSizeX)
        {
            ++linesCleared;
        }
    }

    return linesCleared;
}

uint16_t GetPixel(Location_t location)
{
    uint8_t address = (location.Y << 4) + location.X;

    // Correct for the ZigZag in the LED Array
    if (!bitRead(address, 4))
    {
        address ^= 0x0F;
    }

    //matrix.setPixelColor(address, LINE_COLOR);
    // Maybe add code to make this uint32_t color the same as the uint16_t colors
    return Color32to16(matrix.getPixelColor(address));
}

bool ContainsFlag(int input, int flag)
{
    return (input & flag) > 0;
}

// Returns true if points contains the given point
bool ContainsPoint(Location_t* points, Location_t point){
    for (int i = 0; i < POINTS_PER_SHAPE; ++i){
        Location_t location = points[i];

        if (location.X == point.X && location.Y == point.Y){
            return true;
        }
    }

    return false;
}

// Returns true if the given point collies with any colored block
bool PointCollides(Location_t* points, Location_t point){

    if (ContainsPoint(points, point)){
        // Return false if the point collides with its self
        return false;
    } else {
        return GetPixel(point) != BACKGROUND_COLOR
    }
}

// Returns the types of collistions the given shape will encounter 
// at a given location
Collision_e DetectCollision(Location_t playerPostion, Shape_t shape)
{
    Location_t *points = PlotShape(playerPostion, shape);

    int result = NO_COLLISION;

    for (int i = 0; i < POINTS_PER_SHAPE; ++i)
    {
        Location_t location = points[i];

        // Check for bottom collistions
        if (!ContainsFlag(result, COLLISION_ON_BOTTOM))
        {
            if (location.Y >= (GameOffsetY + GameSizeY - 1))
            {
                result |= COLLISION_ON_BOTTOM;
            }
            else
            {
                ++location.Y;

                if (PointCollides(points, location))
                {
                    result |= COLLISION_ON_BOTTOM;
                }

                --location.Y;
            }
        }

        // Check for left collistions
        if (!ContainsFlag(result, COLLISION_ON_LEFT))
        {
            if (location.X <= GameOffsetX)
            {
                result |= COLLISION_ON_LEFT;
            }
            else
            {
                --location.X;

                if (PointCollides(points, location))
                {
                    result |= COLLISION_ON_LEFT;
                }

                ++location.X;
            }
        }

        // Check for right collistions
        if (!ContainsFlag(result, COLLISION_ON_RIGHT))
        {
            if (location.X >= (GameOffsetX + GameSizeX - 1))
            {
                result |= COLLISION_ON_RIGHT;
            }
            else
            {
                ++location.X;

                if (PointCollides(points, location))
                {
                    result |= COLLISION_ON_RIGHT;
                }

                --location.X;
            }
        }
    }

    return (Collision_e)result;
}

uint8_t frame = 0;

void setup()
{
    Serial.begin(9600);
    randomSeed(analogRead(0));
    GameState = START_GAME;
    //Initializes the LED matrix, clears it, and setups the IO
    initPortableArcade(&matrix);
}

void loop()
{
    static Direction_e lastDirection;
    static Location_t lastOffset;
    static Shape_t lastShape;

    switch (GameState)
    {
    case PRE_GAME:
        /* code */
        break;
    default:
    case START_GAME:
        playerOffset.X = 5;
        playerOffset.Y = 2;
        matrix.fill(BACKGROUND_COLOR);
        nextShape = GetRandomShape();
        DrawPreview(nextShape);
        matrix.drawLine(0, 0, 0, 15, WALL_COLOR);
        matrix.drawLine(11, 0, 11, 15, WALL_COLOR);
        matrix.fillRect(11, 6, 15, 15, WALL_COLOR);
        currentShape = GetRandomShape();
        matrix.show();
        GameState = RUNNING_GAME;
        lastShape.Name = NO_SHAPE;
        break;
    case RUNNING_GAME:
    {
        // Move the shape left and right
        Direction_e currentDirection = GetDirection();
        Collision_e collision = DetectCollision(playerOffset, currentShape);

        bool updateShape = false;

        if (currentDirection != lastDirection)
        {
            lastDirection = currentDirection;

            switch (currentDirection)
            {
            case LEFT:
                if (!ContainsFlag(collision, COLLISION_ON_LEFT))
                {
                    --playerOffset.X;
                    updateShape = true;
                }
                break;

            case RIGHT:
                if (!ContainsFlag(collision, COLLISION_ON_RIGHT))
                {
                    ++playerOffset.X;
                    updateShape = true;
                }
                break;

            case DOWN:
                    playerOffset.Y = 14;
                break;

            case UP:
            {
                Shape_t shapeStorage = currentShape;
                RotateShape(&currentShape);

                Collision_e rotateCollision = DetectCollision(playerOffset, currentShape);

                if (rotateCollision == (COLLISION_ON_LEFT | COLLISION_ON_RIGHT))
                {
                    // Invalid rotation
                    currentShape = shapeStorage;
                }
                else if (ContainsFlag(rotateCollision, COLLISION_ON_RIGHT))
                {
                    --playerOffset.X;
                }
                else if (ContainsFlag(rotateCollision, COLLISION_ON_LEFT))
                {

                    ++playerOffset.X;

                    if (currentShape.Name == LINE)
                    {
                        rotateCollision = DetectCollision(playerOffset, currentShape);

                        if (rotateCollision == (COLLISION_ON_LEFT | COLLISION_ON_RIGHT))
                        {
                            // Invalid rotation
                            currentShape = shapeStorage;
                        }
                        else if (ContainsFlag(rotateCollision, COLLISION_ON_LEFT))
                        {
                            ++playerOffset.X;
                        }
                    }
                }

                updateShape = true;
            }

            break;
            }
        }

        // Move down if it is time
        static uint16_t downCount = 0;
        if (++downCount > 500)
        {
            downCount = 0;

            if (ContainsFlag(collision, COLLISION_ON_BOTTOM))
            {
                playerOffset.Y = 2;
                playerOffset.X = 5;
                currentShape = nextShape;
                nextShape = GetRandomShape();
                DrawPreview(nextShape);
                LineErase();
                lastShape.Name = NO_SHAPE;
            }
            else
            {
                ++playerOffset.Y;
                updateShape = true;
            }
        }

        // Update the shapes location if needed
        if (updateShape)
        {
            if (lastShape.Name != NO_SHAPE)
            {
                ClearShape(lastOffset, lastShape);
            }

            lastOffset = playerOffset;
            lastShape = currentShape;

            DrawShape(playerOffset, currentShape);
            matrix.show();
        }

        delay(1);
    }
    break;
    case END_GAME:

        break;
    }
}