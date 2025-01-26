#include "map.h"
#include "defs.h"
#include <stdio.h>


static int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
  //////////////////////// STARTING ROOM
    {8, 8, 8, 8, 8, 8},
    {8, 0, 0, 8, 8, 8},
    {8, 0, 0, 8, 0, 8},
    {8, 0, 8, 8, 0, 8},
    {8, 0, 8, 8, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 8, 8, 8, 7, 8},
  ////////////////////////room1
    {8, 0, 0, 0, 0, 8},
    {5, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {5, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 8, 5, 8, 7, 8},
  ////////////////////////room2
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 8, 8, 8, 7, 8},
  ////////////////////////room3
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 8, 8, 8, 7, 8},
  ////////////////////////room4
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 8, 8, 8, 7, 8},
  ////////////////////////room5
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 8, 8, 8, 7, 8},
  ////////////////////////room6
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 8, 8, 8, 7, 8},
  ////////////////////////finalroom
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 0, 0, 0, 0, 8},
    {8, 8, 8, 8, 8, 8},
};

bool
mapHasWallAt(float x, float y)
{
  if (x < 0
    || x >= MAP_NUM_COLS * TILE_SIZE
    || y < 0
    || y >= MAP_NUM_ROWS * TILE_SIZE)
  {
    return true;
  }
  int mapGridIndexX = floor(x / TILE_SIZE);
  int mapGridIndexY = floor(y / TILE_SIZE);

  return map[mapGridIndexY][mapGridIndexX] != 0;
}

int
getMapContent(float x, float y)
{
  int mapGridIndexX = (x / TILE_SIZE);
  int mapGridIndexY = (y / TILE_SIZE);
  // printf("%i, %i \n", mapGridIndexX, mapGridIndexY);
// printf("%i \n", map[mapGridIndexY][mapGridIndexX]);

  return (map[mapGridIndexY][mapGridIndexX]);
}

void
setMapContent(int gridX, int gridY, int newContent)
{
  map[gridY][gridX] = newContent;
}

bool
isInsideMap(float x, float y)
{
  return (x >= 0 && x <= MAP_NUM_COLS * TILE_SIZE && y >= 0 && y <= MAP_NUM_ROWS * TILE_SIZE);
}

void
renderMap(void)
{
  for (int i = 0; i < MAP_NUM_ROWS; i++) {
    for (int j = 0; j < MAP_NUM_COLS; j++) {
      int tileX = j * floor(TILE_SIZE);
      int tileY = i * floor(TILE_SIZE);
      color_t tileColor = map[i][j] != 0 ? 0xFF333333 : 0x00000000;

      drawRect(
        tileX * MINIMAP_SCALE_FACTOR,
        tileY * MINIMAP_SCALE_FACTOR,
        TILE_SIZE * MINIMAP_SCALE_FACTOR,
        TILE_SIZE * MINIMAP_SCALE_FACTOR,
        tileColor
      );
    }
  }
}

int
getMapAt(int i, int j)
{
  return map[i][j];
}
