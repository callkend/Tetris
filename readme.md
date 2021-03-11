# Portable Arcade - Tetris
## Authors
- Kendall Callister (callkend)
- Wyatt Callister (callwyat)

## Description
Using the amazing portable arcade with a 16x16 LED Array, and an arcade joystick, we are going to make Tetris! It's going to be hard...

## Screen Layout
### Legend
- B > Boarder
- G > Game Space
- P > Preview

### Display
|   | 0 |1  | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10  | 11  | 12  | 13  | 14  | 15  |
| -:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0  | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |
|1  | B | G | G | G | G | G | G | G | G | G | G | G | B | P | P | P |
|2  | B | G | G | G | G | G | G | G | G | G | G | G | B | P | P | P |
|3  | B | G | G | G | G | G | G | G | G | G | G | G | B | P | P | P |
|4  | B | G | G | G | G | G | G | G | G | G | G | G | B | P | P | P |
|5  | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |
|6  | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |
|7  | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |
|8  | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |
|9  | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |
|10 | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |
|11 | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |
|12 | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |
|13 | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |
|14 | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |
|15 | B | G | G | G | G | G | G | G | G | G | G | G | B |   |   |   |

### Shapes
All shapes need to be centered. On shapes with even number of rows or columns, this 
centering is accomplished by offsetting everything by 0.5. This makes rotating the 
shapes much much easier.
#### Line
|      | -1.5 | -1.0 | -0.5 |  0.0 |  0.5 |  1.0 |  1.5 |
|------|:----:|:----:|:----:|:----:|:----:|:----:|:----:|
| -2.0 |      |      |      |      |      |      |      |
| -1.5 |      |      |      |   X  |      |      |      |
| -1.0 |      |      |      |      |      |      |      |
| -0.5 |      |      |      |   X  |      |      |      |
| -0.0 |      |      |      |      |      |      |      |
|  0.5 |      |      |      |   X  |      |      |      |
|  1.0 |      |      |      |      |      |      |      |
|  1.5 |      |      |      |   X  |      |      |      |
|  2.0 |      |      |      |      |      |      |      |

#### "L"
|      | -1.5 | -1.0 | -0.5 |  0.0 |  0.5 |  1.0 |  1.5 |
|------|:----:|:----:|:----:|:----:|:----:|:----:|:----:|
| -2.0 |      |      |      |      |      |      |      |
| -1.5 |      |      |      |      |      |      |      |
| -1.0 |      |      |   X  |      |      |      |      |
| -0.5 |      |      |      |      |      |      |      |
| -0.0 |      |      |   X  |      |      |      |      |
|  0.5 |      |      |      |      |      |      |      |
|  1.0 |      |      |   X  |      |   X  |      |      |
|  1.5 |      |      |      |      |      |      |      |
|  2.0 |      |      |      |      |      |      |      |

#### "T"
|      | -1.5 | -1.0 | -0.5 |  0.0 |  0.5 |  1.0 |  1.5 |
|------|:----:|:----:|:----:|:----:|:----:|:----:|:----:|
| -2.0 |      |      |      |      |      |      |      |
| -1.5 |      |      |      |      |      |      |      |
| -1.0 |      |      |      |      |      |      |      |
| -0.5 |      |      |      |   X  |      |      |      |
| -0.0 |      |      |      |      |      |      |      |
|  0.5 |      |   X  |      |   X  |      |   X  |      |
|  1.0 |      |      |      |      |      |      |      |
|  1.5 |      |      |      |      |      |      |      |
|  2.0 |      |      |      |      |      |      |      |

#### "Z"
|      | -1.5 | -1.0 | -0.5 |  0.0 |  0.5 |  1.0 |  1.5 |
|------|:----:|:----:|:----:|:----:|:----:|:----:|:----:|
| -2.0 |      |      |      |      |      |      |      |
| -1.5 |      |      |      |      |      |      |      |
| -1.0 |      |      |      |      |      |      |      |
| -0.5 |      |   X  |      |   X  |      |      |      |
| -0.0 |      |      |      |      |      |      |      |
|  0.5 |      |      |      |   X  |      |   X  |      |
|  1.0 |      |      |      |      |      |      |      |
|  1.5 |      |      |      |      |      |      |      |
|  2.0 |      |      |      |      |      |      |      |