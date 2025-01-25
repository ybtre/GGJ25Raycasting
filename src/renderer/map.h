#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "graphics.h"

#define MAP_NUM_ROWS 55
#define MAP_NUM_COLS 6

bool mapHasWallAt(float x, float y);
int getMapContent(float x, float y);
bool isInsideMap(float x, float y);
void renderMap(void);
int getMapAt(int i, int j);

#endif
