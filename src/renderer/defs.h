#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

#define PI 3.14159265
#define TWO_PI 6.28318530

#define TILE_SIZE 64

#define NUM_TEXTURES 8

#define MINIMAP_SCALE_FACTOR 0.05

//16:10 aspect ratio
#define WINDOW_WIDTH (160 * 2)
#define WINDOW_HEIGHT (100 *2)

#define FOV_ANGLE (120 * (PI / 180))

//NOTE: interesting effect when divided by 2
#define NUM_RAYS (WINDOW_WIDTH/1)

#define DIST_PROJ_PLANE ((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2))

#define FPS 60
#define FRAME_TIME_LENGTH_MS (1000 / FPS)

#endif
