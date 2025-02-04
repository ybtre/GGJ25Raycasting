#include "player.h"
#include "defs.h"
#include "utils.h"

player_t player =
  {
    // .x = (float)MAP_NUM_COLS * TILE_SIZE / 2,
    // .y = (float)MAP_NUM_ROWS * TILE_SIZE / 2,
    .x = 1.5 * TILE_SIZE,
    .y = 1.5 * TILE_SIZE,
    .width = 1,
    .height = 1,
    .turnDirection = 0,
    .walkDirection = 0,
    .rotationAngle = PI / 2.0,
    .walkSpeed = 100,
    .turnSpeed = 180 * (PI / 180),
  };

void
movePlayer(float deltaTime)
{
  player.rotationAngle += player.turnDirection * player.turnSpeed * deltaTime;
  normalizeAngle(&player.rotationAngle);
  float moveStep = player.walkDirection * player.walkSpeed * deltaTime;

  float newPlayerX = player.x + cos(player.rotationAngle) * moveStep;
  float newPlayerY = player.y + sin(player.rotationAngle) * moveStep;

  if (!mapHasWallAt(newPlayerX, newPlayerY)) {
    player.x = newPlayerX;
    player.y = newPlayerY;
  }
}

void
renderPlayer(void)
{
  drawRect(
    player.x * MINIMAP_SCALE_FACTOR,
    player.y * MINIMAP_SCALE_FACTOR,
    player.width * MINIMAP_SCALE_FACTOR,
    player.height * MINIMAP_SCALE_FACTOR,
    0xFFFFFFFF
  );
}
