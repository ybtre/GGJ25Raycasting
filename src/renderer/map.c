#include "map.h"
#include "defs.h"


static const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
    {7, 7, 7},
    {7, 0, 7},
    {7, 0, 6},
    {7, 0, 5},
    {7, 0, 7},
    {7, 0, 4},
    {7, 0, 4},
    {7, 0, 4},
    {7, 0, 7},
    {7, 0, 7},
    {7, 0, 7},
    {7, 0, 7},
    {7, 7, 7}
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
      int tileX = j * TILE_SIZE;
      int tileY = i * TILE_SIZE;
      uint32_t tileColor = map[i][j] != 0 ? 0xFFFFFFFF : 0x00000000;

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
