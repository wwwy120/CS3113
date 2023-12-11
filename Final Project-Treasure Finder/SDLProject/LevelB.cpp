#include "LevelB.h"
#include "Utility.h"

#define DECORATION_WIDTH 48
#define DECORATION_HEIGHT 48
#define UNDERGROUND_WIDTH 32
#define UNDERGROUND_HEIGHT 32
#define LEVEL_WIDTH 32
#define LEVEL_HEIGHT 32

unsigned int LEVEL_DATA_B_DECOR[] =
{
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2, 76, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92,  92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 77,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2, 75,145,200,200,200,200,200,200,200,200,200,200,200,200,200,200,  200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,147, 73,  1,  2,  1, 2, 1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 55, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 57,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 3, 1, 2, 74, 2, 74, 2, 1, 74, 74, 74, 3, 1, 1, 2, 74, 2, 74, 1, 4, 74, 74, 2, 2, 74, 74, 4, 3, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 2, 1, 1, 2, 1, 2, 2, 74, 3, 74, 1, 1, 1, 1, 74,  3, 1, 4, 74, 1, 2, 2, 1, 4, 74, 2, 2, 1, 2, 2, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 74, 1, 1, 1, 1, 74, 74, 1, 2, 1, 74, 74, 2, 2, 74,  1, 4, 2, 4, 1, 74, 1, 74, 3, 74, 74, 74, 74, 74, 74, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 1, 74, 2, 74, 3, 1, 2, 74, 2, 2, 1, 1, 1, 74, 1, 1, 2, 1, 74, 74, 2, 1, 74, 2, 74, 1, 3, 4, 74, 3, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 1, 2, 2, 1, 2, 3, 1, 74, 1, 1, 74, 74, 74, 74, 2,  2, 1, 74, 74, 2, 2, 74, 1, 2, 74, 74, 74, 2, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 2, 1, 1, 2, 74, 1, 2, 3, 1, 2, 1, 2, 1, 74, 1,  1, 1, 2, 2, 2, 1, 1, 2, 1, 2, 1, 2, 1, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 3, 1, 2, 74, 2, 74, 2, 1, 74, 74, 74, 3, 1, 1, 2, 74, 2, 74, 1, 4, 74, 74, 2, 2, 74, 74, 4, 3, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 2, 1, 1, 2, 1, 2, 2, 74, 3, 74, 1, 1, 1, 1, 74,  3, 1, 4, 74, 1, 2, 2, 1, 4, 74, 2, 2, 1, 2, 2, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 74, 1, 1, 1, 1, 74, 74, 1, 2, 1, 74, 74, 2, 2, 74,  1, 4, 2, 4, 1, 74, 1, 74, 3, 74, 74, 74, 74, 74, 74, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 1, 74, 2, 74, 3, 1, 2, 74, 2, 2, 1, 1, 1, 74, 1, 1, 2, 1, 74, 74, 2, 1, 74, 2, 74, 1, 3, 4, 74, 3, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 1, 2, 2, 1, 2, 3, 1, 74, 1, 1, 74, 74, 74, 74, 2,  2, 1, 74, 74, 2, 2, 74, 1, 2, 74, 74, 74, 2, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 2, 1, 1, 2, 74, 1, 2, 3, 1, 2, 1, 2, 1, 74, 1,  1, 1, 2, 2, 2, 1, 1, 2, 1, 2, 1, 2, 1, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 3, 1, 2, 74, 2, 74, 2, 1, 74, 74, 74, 3, 1, 1, 2, 74, 2, 74, 1, 4, 74, 74, 2, 2, 74, 74, 4, 3, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 2, 1, 1, 2, 1, 2, 2, 74, 3, 74, 1, 1, 1, 1, 74,  3, 1, 4, 74, 1, 2, 2, 1, 4, 74, 2, 2, 1, 2, 2, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 74, 1, 1, 1, 1, 74, 74, 1, 2, 1, 74, 74, 2, 2, 74,  1, 4, 2, 4, 1, 74, 1, 74, 3, 74, 74, 74, 74, 74, 74, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 1, 74, 2, 74, 3, 1, 2, 74, 2, 2, 1, 1, 1, 74, 1, 1, 2, 1, 74, 74, 2, 1, 74, 2, 74, 1, 3, 4, 74, 3, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 1, 2, 2, 1, 2, 3, 1, 74, 1, 1, 74, 74, 74, 74, 2,  2, 1, 74, 74, 2, 2, 74, 1, 2, 74, 74, 74, 2, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 2, 1, 1, 2, 74, 1, 2, 3, 1, 2, 1, 2, 1, 74, 1,  1, 1, 2, 2, 2, 1, 1, 2, 1, 2, 1, 2, 1, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 3, 1, 2, 74, 2, 74, 2, 1, 74, 74, 74, 3, 1, 1, 2, 74, 2, 74, 1, 4, 74, 74, 2, 2, 74, 74, 4, 3, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 2, 1, 1, 2, 1, 2, 2, 74, 3, 74, 1, 1, 1, 1, 74,  3, 1, 4, 74, 1, 2, 2, 1, 4, 74, 2, 2, 1, 2, 2, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 74, 1, 1, 1, 1, 74, 74, 1, 2, 1, 74, 74, 2, 2, 74,  1, 4, 2, 4, 1, 74, 1, 74, 3, 74, 74, 74, 74, 74, 74, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 1, 74, 2, 74, 3, 1, 2, 74, 2, 2, 1, 1, 1, 74, 1, 1, 2, 1, 74, 74, 2, 1, 74, 2, 74, 1, 3, 4, 74, 3, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 1, 2, 2, 1, 2, 3, 1, 74, 1, 1, 74, 74, 74, 74, 2,  2, 1, 74, 74, 2, 2, 74, 1, 2, 74, 74, 74, 2, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 2, 1, 1, 2, 74, 1, 2, 3, 1, 2, 1, 2, 1, 74, 1,  1, 1, 2, 2, 2, 1, 1, 2, 1, 2, 1, 2, 1, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 3, 1, 2, 74, 2, 74, 2, 1, 74, 74, 74, 3, 1, 1, 2, 74, 2, 74, 1, 4, 74, 74, 2, 2, 74, 74, 4, 3, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 2, 1, 1, 2, 1, 2, 2, 74, 3, 74, 1, 1, 1, 1, 74,  3, 1, 4, 74, 1, 2, 2, 1, 4, 74, 2, 2, 1, 2, 2, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 74, 1, 1, 1, 1, 74, 74, 1, 2, 1, 74, 74, 2, 2, 74,  1, 4, 2, 4, 1, 74, 1, 74, 3, 74, 74, 74, 74, 74, 74, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 1, 74, 2, 74, 3, 1, 2, 74, 2, 2, 1, 1, 1, 74, 1, 1, 2, 1, 74, 74, 2, 1, 74, 2, 74, 1, 3, 4, 74, 3, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 1, 2, 2, 1, 2, 3, 1, 74, 1, 1, 74, 74, 74, 74, 2,  2, 1, 74, 74, 2, 2, 74, 1, 2, 74, 74, 74, 2, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 73, 2, 1, 1, 2, 74, 1, 2, 3, 1, 2, 1, 2, 1, 74, 1,  1, 1, 2, 2, 2, 1, 1, 2, 1, 2, 1, 2, 1, 74, 1, 75,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,162, 91, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92,  92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 93,  162, 73,  1,  2,  1,  2,  1, 2,
    1,  2,  1,  2,  1,  2, 75,181,200,200,200,200,200,200,200,200,200,200,200,200,200,200,  200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,183, 73,  1,  2,  1, 2, 1, 2,
    1,  2,  1,  2,  1,  2, 94, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,  56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 95,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,
    1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2
};

unsigned int LEVEL_DATA_B_MAP[] =
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,145,200,200,200,200,200,200,200,200,200,200,200,200,200,200,  200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,147, 0,  0,  0,  0, 0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  198,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,199,200,200,201,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,198,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,145,200,200,200,147,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,162,236,237,238,162,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,198,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,181,200,200,200,183,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,198,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,198,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,145,  200,200,200,147,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0, 0,
    0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,162,  218,219,220,162,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,162,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,181,200,200,200,200,200,200,200,200,200,200,200,200,200,200,  200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,183,  0,  0,  0,  0, 0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};




LevelB::~LevelB()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    delete [] m_state.star;
    delete    m_state.bomb;
    Mix_FreeChunk(m_state.bomb_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelB::initialise()
{
    GLuint texture_id = Utility::load_texture("assets/images/tilemap.png");
    m_state.decoration = new Map(DECORATION_WIDTH, DECORATION_HEIGHT, LEVEL_DATA_B_DECOR, texture_id, 1.0f, 18, 14);
    m_state.map = new Map(DECORATION_WIDTH, DECORATION_HEIGHT, LEVEL_DATA_B_MAP, texture_id, 1.0f, 18, 14);
    

    
    // Player
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(5.0f);
    m_state.player->m_texture_id = Utility::load_texture("assets/images/player.png");
    
    // Walking Animation
    m_state.player->m_walking[m_state.player->LEFT]  = new int[3] { 15, 16, 17 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[3] { 27, 28, 29 };
    m_state.player->m_walking[m_state.player->UP]    = new int[3] { 39, 40, 41 };
    m_state.player->m_walking[m_state.player->DOWN]  = new int[3] {  3,  4,  5 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->UP];
    m_state.player->m_animation_frames = 3;
    m_state.player->m_animation_index  = 0;
    m_state.player->m_animation_time   = 0.0f;
    m_state.player->m_animation_cols   = 12;
    m_state.player->m_animation_rows   = 8;
    m_state.player->set_height(1.0f);
    m_state.player->set_width(0.8f);
    
    m_state.player->alive = true;
    m_state.player->star_get = 0;
    
    
    //Enemy
    GLuint enemy_texture_id = Utility::load_texture("assets/images/AI.png");
    
    m_state.enemies = new Entity[ENEMY_COUNT];
    for(int i = 0; i < ENEMY_COUNT; ++i){
        m_state.enemies[i].set_entity_type(ENEMY);
        m_state.enemies[i].set_ai_type(GUARD);
        m_state.enemies[i].set_ai_state(IDLE);
        m_state.enemies[i].m_texture_id = enemy_texture_id;
        m_state.enemies[i].set_movement(glm::vec3(0.0f));
        m_state.enemies[i].set_speed(2.0f);
        
        m_state.enemies[i].m_walking[ m_state.enemies[i].LEFT]  = new int[3] { 12, 13, 14 };
        m_state.enemies[i].m_walking[ m_state.enemies[i].RIGHT] = new int[3] { 24, 25, 26 };
        m_state.enemies[i].m_walking[ m_state.enemies[i].UP]    = new int[3] { 36, 37, 38 };
        m_state.enemies[i].m_walking[ m_state.enemies[i].DOWN]  = new int[3] {  0,  1,  2 };

        m_state.enemies[i].m_animation_indices = m_state.enemies[i].m_walking[m_state.enemies[i].DOWN];
        m_state.enemies[i].m_animation_frames = 3;
        m_state.enemies[i].m_animation_index  = 0;
        m_state.enemies[i].m_animation_time   = 0.0f;
        m_state.enemies[i].m_animation_cols   = 12;
        m_state.enemies[i].m_animation_rows   = 8;
        m_state.enemies[i].set_height(1.0f);
        m_state.enemies[i].set_width(1.0f);
        m_state.enemies[i].alive = true;
        while(true){
            int random1 = rand() % 16;
            int random2 = rand() % 16;
            int sign1 = rand() % 2;
            int sign2 = rand() % 2;
            if(random1 > 4 || random2 > 4){
                if(sign1 == 1){
                    random1 = -random1;
                }
                if(sign2 == 1){
                    random2 = -random2;
                }
                m_state.enemies[i].set_position(glm::vec3(random1, random2, 0.0f));
                break;
            }
        }
    }
//    m_state.enemies[0].set_position(glm::vec3(-2.0f, 4.0f, 0.0f));
//    m_state.enemies[1].set_position(glm::vec3(6.0f, -6.0f, 0.0f));
//    m_state.enemies[2].set_position(glm::vec3(-10.0f, -6.0f, 0.0f));
//    m_state.enemies[3].set_position(glm::vec3(2.0f, 12.0f, 0.0f));
//    m_state.enemies[4].set_position(glm::vec3(8.0f, -10.0f, 0.0f));
    
    //Star
    m_state.star = new Entity[STAR_COUNT];
    GLuint star_texture_id = Utility::load_texture("assets/images/star.png");
    for(int i = 0; i < STAR_COUNT; ++i){
        m_state.star[i].set_entity_type(STAR);
        m_state.star[i].set_movement(glm::vec3(0.0f));
        m_state.star[i].set_speed(0.0f);
        m_state.star[i].m_texture_id = star_texture_id;
    }
    m_state.star[0].set_position(glm::vec3(rand() % 16, rand() % 16, 0.0f));
    m_state.star[1].set_position(glm::vec3(rand() % 16, rand() % 16, 0.0f));
    m_state.star[2].set_position(glm::vec3(-rand() % 16, rand() % 16, 0.0f));
    m_state.star[3].set_position(glm::vec3(rand() % 16, -rand() % 16, 0.0f));
    m_state.star[4].set_position(glm::vec3(rand() % 16, -rand() % 16, 0.0f));
    m_state.star[5].set_position(glm::vec3(-rand() % 16, -rand() % 16, 0.0f));
    m_state.star[6].set_position(glm::vec3(-rand() % 16, -rand() % 16, 0.0f));
    
    //Bomb
    m_state.bomb = new Entity();
    GLuint bomb_texture_id = Utility::load_texture("assets/images/bomb.png");
    m_state.bomb->set_entity_type(BOMB);
    m_state.bomb->m_is_active = false;
    m_state.bomb->set_movement(glm::vec3(0.0f));
    m_state.bomb->set_speed(0.0f);
    m_state.bomb->m_texture_id = bomb_texture_id;
    m_state.bomb->set_position(glm::vec3(-1000.0f, -1000.0f, 0.0f));
    m_state.bomb->bomb_count = BOMB_COUNT;
    
    //Mud
    m_state.mud = new Entity[BOMB_COUNT];
    GLuint mud_texture_id = Utility::load_texture("assets/images/mud.png");
    for(int i = 0; i < BOMB_COUNT; ++i){
        m_state.mud[i].set_entity_type(MUD);
        m_state.mud[i].set_movement(glm::vec3(0.0f));
        m_state.mud[i].set_speed(0.0f);
        m_state.mud[i].m_texture_id = mud_texture_id;
        m_state.mud[i].m_appear = false;
        m_state.mud[i].set_position(glm::vec3(1000.0f, 1000.0f, 0.0f));
    }
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
       
    m_state.bomb_sfx = Mix_LoadWAV("assets/audio/mixkit-vide-game-time-bomb-explosion-2807.wav");
    
}

void LevelB::update(float delta_time)
{

    //player update
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);

    //AI updste
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map);
        if(glm::distance(m_state.enemies[i].get_position(), m_state.bomb->get_position()) < 4.0f && m_state.bomb->bomb_timer == 0){
            m_state.enemies[i].alive = false;
            m_state.enemies[i].disappear();
        }
    }
    
    //bomb update
    m_state.bomb->update(delta_time, NULL, 0, 0, m_state.map);
   
    //lose if the player is injured by bomb
    if(glm::distance(m_state.player->get_position(), m_state.bomb->get_position()) < 4.0f && m_state.bomb->bomb_timer == 0){
        m_state.player->alive = false;
        m_state.player->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    
    //lose if the player uses up all bombs
    if(m_state.bomb->bomb_count == 0){
        m_state.player->alive = false;
        m_state.player->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    
    //star update
    for (int i = 0; i < STAR_COUNT; i++)
    {
        if((glm::distance(m_state.star[i].get_position(), m_state.bomb->get_position())< 4.0f) && m_state.bomb->bomb_timer == 0){
            m_state.star[i].m_appear = true;
        }
        m_state.star[i].update(delta_time, m_state.player, NULL, 0, NULL);
    }
    
    //mud update
    if(m_state.bomb->bomb_timer == 0){
        m_state.mud[m_state.bomb->bomb_count].set_position(m_state.bomb->get_position());
        m_state.mud[m_state.bomb->bomb_count].update(delta_time, NULL, NULL, 0, NULL);
    }
}


void LevelB::render(ShaderProgram *program)
{
    if(m_state.decoration){
        m_state.decoration->render(program);
    }
    if(m_state.map){
        m_state.map->render(program);
    }
    for (int i = 0; i < BOMB_COUNT; i++)
    {
        m_state.mud[i].render(program);
    }
    for (int i = 0; i < STAR_COUNT; i++)
    {
        if(m_state.star[i].m_appear){
            m_state.star[i].render(program);
        }
    }
    if(m_state.map){
        m_state.map->render(program);
    }
    if(m_state.bomb->m_is_active){
        m_state.bomb->render(program);
    }
    
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
    
}
