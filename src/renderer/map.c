#include "map.h"
#include "defs.h"
#include <stdio.h>


static int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
  //////////////////////// STARTING ROOM
    {8, 8, 8, 8, 8, 8},//0
    {8, 0, 0, 8, 8, 8},//1
    {8, 0, 0, 8, 0, 8},//2
    {8, 0, 8, 8, 0, 8},//3
    {8, 0, 8, 8, 0, 8},//4
    {8, 0, 0, 0, 0, 8},//5
    {8, 8, 8, 8, 7, 8},//6
  ////////////////////////room1
    {8, 0, 0, 0, 0, 8},//7
    {5, 0, 0, 0, 0, 8},//8
    {8, 0, 0, 0, 0, 8},//9
    {5, 0, 0, 0, 0, 8},//10
    {8, 0, 0, 0, 0, 8},//11
    {8, 8, 5, 8, 7, 8},//12
  ////////////////////////room2
    {8, 18, 0, 0, 0, 8},//13
    {8, 0, 0, 0, 0, 18},//14
    {19, 0, 0, 0, 0, 18},//15
    {8, 18, 0, 0, 0, 18},//16
    {8, 0, 0, 0, 0, 8},//17
    {8, 18, 19, 8, 7, 5},//18
  ////////////////////////room3
    {22, 0, 0, 0, 0, 8},//19
    {8, 0, 5, 23, 21, 8},//20
    {23, 0, 0, 0, 22, 8},//21
    {8, 21, 8, 0, 5, 8},//22
    {8, 23, 0, 0, 0, 8},//23
    {8, 8, 8, 5, 7, 8},//24
  ////////////////////////room4
    {8, 28, 0, 0, 0, 8},//25
    {8, 28, 0, 0, 0, 8},//26
    {8, 28, 0, 0, 28, 8},//27
    {8, 28, 0, 0, 28, 8},//28
    {8, 28, 0, 0, 0, 8},//29
    {8, 8, 24, 8, 7, 8},//30
  ////////////////////////room5
    {25, 0, 0, 0, 0, 8},//31
    {26, 0, 0, 0, 0, 28},//32
    {27, 0, 0, 0, 0, 27},//33
    {27, 0, 0, 0, 0, 26},//34
    {26, 0, 0, 0, 0, 27},//35
    {8, 28, 26, 26, 7, 8},//36
  ////////////////////////room6
    {8, 0, 0, 0, 0, 8},//37
    {8, 8, 0, 0, 0, 8},//38
    {8, 0, 0, 0, 8, 8},//39
    {8, 0, 0, 8, 8, 8},//40
    {8, 0, 8, 8, 8, 8},//41
    {8, 31, 8, 31, 8, 8},//42
  ////////////////////////finalroom
    {8, 0, 0, 0, 0, 8},//43
    {8, 0, 0, 0, 0, 8},//44
    {8, 0, 0, 0, 0, 8},//45
    {8, 0, 0, 0, 0, 8},//46
    {8, 0, 0, 0, 0, 8},//47
    {8, 8, 8, 8, 8, 8},//48
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
