#include "sprite.h"
#include "defs.h"

#define NUM_SPRITES 14

static sprite_t sprites[NUM_SPRITES] = {
  { .x = (32 + (53 * (1))), .y = (14 + (56 * (5))), .texture = 15, .active = true  }, // intro1 text
  { .x = (32 + (53 * (3))), .y = (14 + (56 * (6))), .texture = 15, .active = true  }, // intro2 text
  { .x = (32 + (53 * (5))), .y = (14 + (56 * (6))), .texture = 13, .active = true  }, // intro key
  { .x = (32 + (53 * (5))), .y = (14 + (56 * (7))), .texture = 15, .active = true  }, // ch1 title
  { .x = (32 + (53 * (2))), .y = (14 + (56 * (11))), .texture = 16, .active = true }, // ch1 main
  { .x = (32 + (53 * (5))), .y = (14 + (56 * (13))), .texture = 13, .active = false }, // ch1 key
  { .x = (32 + (53 * (3))), .y = (14 + (56 * (18))), .texture = 15, .active = true }, // ch2 main
  { .x = (32 + (53 * (3))), .y = (14 + (56 * (19))), .texture = 19, .active = true }, // ch2 mirror
  { .x = (32 + (53 * (5))), .y = (14 + (56 * (20))), .texture = 13, .active = false }, // ch2 key
  { .x = (32 + (53 * (5))), .y = (14 + (56 * (27))), .texture = 13, .active = false }, // ch3 key
  { .x = (64 + (53 * (2))), .y = (14 + (56 * (27))), .texture = 15, .active = true }, // ch3 main
  { .x = (32 + (53 * (3))), .y = (14 + (56 * (34))), .texture = 13, .active = false }, // ch4 key 1
  { .x = (32 + (53 * (5))), .y = (14 + (56 * (34))), .texture = 13, .active = false }, // ch4 key 2
  { .x = (64 + (53 * (2))), .y = (14-28 + (56 * (34))), .texture = 15, .active = true }, // ch4 main
};

void
removeSprite(int spriteID)
{
  sprites[spriteID].active = false;
}

void
activateSprite(int spriteID)
{
  sprites[spriteID].active = true;
}

void 
renderSpriteProjection(void)
{
  sprite_t visibleSprites[NUM_SPRITES];
  int numVisibleSprites = 0;

  //find sprites that are visible (inside FOV)
  for (int i = 0; i < NUM_SPRITES; i++)
  {
    float angleSpritePlayer = player.rotationAngle - atan2((sprites[i].y) - player.y, (sprites[i].x) - player.x);

    if(angleSpritePlayer > PI)
    {
      angleSpritePlayer -= TWO_PI;
    }
    if(angleSpritePlayer < -PI)
    {
      angleSpritePlayer += TWO_PI;
    }
    angleSpritePlayer = fabs(angleSpritePlayer);
    const float EPSILON = 0.4;
    if(angleSpritePlayer < (FOV_ANGLE / 2) + EPSILON)
    {
      sprites[i].visible = true;
      sprites[i].angle = angleSpritePlayer;
      sprites[i].distance = distanceBetweenPoints((sprites[i].x), (sprites[i].y), player.x, player.y);
      visibleSprites[numVisibleSprites] = sprites[i];
      numVisibleSprites++;
    } else {
      sprites[i].visible = false;
    }
  }

  //sort sprites by distance for drawing
  for(int i = 0; i < numVisibleSprites - 1; i++)
  {
    for(int j = i + 1; j < numVisibleSprites; j++)
    {
      if(visibleSprites[i].distance < visibleSprites[j].distance)
      {
        sprite_t tmp = visibleSprites[i];
        visibleSprites[i] = visibleSprites[j];
        visibleSprites[j] = tmp;
      }
    }
  }

  //render all visible sprites
  for(int i = 0; i < numVisibleSprites; i++)
  {
    sprite_t spr = visibleSprites[i];

    if(!spr.active) continue;

    float perpDistance = spr.distance * cos(spr.angle);

    float sprHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;
    float sprWidith = sprHeight;

    float sprTopY = ((float)WINDOW_HEIGHT / 2) - (sprHeight / 2);
    sprTopY = (sprTopY < 0) ? 0 : sprTopY;

    float sprBottomY = ((float)WINDOW_HEIGHT / 2) + (sprHeight / 2);
    sprBottomY = (sprBottomY > WINDOW_HEIGHT) ? WINDOW_HEIGHT : sprBottomY;

    float spriteAngle = atan2((spr.y) - player.y, (spr.x) - player.x) - player.rotationAngle;
    float spriteScreenPosX = tan(spriteAngle) * DIST_PROJ_PLANE;

    float sprLeftX = (WINDOW_WIDTH / 2) + spriteScreenPosX - (sprWidith / 2);
    float sprRightX = sprLeftX + sprWidith;

    int texWidth = upng_get_width(textures[spr.texture]);
    int texHeight = upng_get_height(textures[spr.texture]);

    for(int x = sprLeftX; x < sprRightX; x++)
    {
      float texelWidth = (texWidth / sprWidith);
      int texOffsetX = (x - sprLeftX) * texelWidth;

      for(int y = sprTopY; y < sprBottomY; y++)
      {
        if (x> 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT)
        {
          int distanceFromTop = y + (sprHeight / 2) - (WINDOW_HEIGHT / 2);
          int texOffsetY = distanceFromTop * (texHeight / sprHeight);

          color_t * spriteTexBuffer = (color_t*)upng_get_buffer(textures[spr.texture]);
          color_t texelColor = spriteTexBuffer[(texWidth * texOffsetY) + texOffsetX];
  
          if(spr.distance < rays[x].distance && texelColor != 0xFFFF00FF)
          {
            drawPixel(x, y, texelColor);
          }
        }
      }
    }
  }
}


void
renderMapSprites(void)
{
  for (int i = 0; i < NUM_SPRITES; i++)
  {
    drawRect(
      (sprites[i].x) * MINIMAP_SCALE_FACTOR, 
      (sprites[i].y) * MINIMAP_SCALE_FACTOR,
      1, 1, 
      (sprites[i].visible) ? 0xFF00FFFF : 0xFF444444);
  }
}
