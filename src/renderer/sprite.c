#include "sprite.h"
#include "defs.h"

#define NUM_SPRITES 3

static sprite_t sprites[NUM_SPRITES] = {
  { .x = 4, .y = 2, .texture = 8 }, // barrel
  { .x = 2, .y = 5, .texture = 9 }, // light
  { .x = 2, .y = 10, .texture = 11 } // guard
};

float
wtog(float val)
{
  return floor(val / TILE_SIZE);
}

float
gtow(float val)
{
  return floor(val * TILE_SIZE);
}

void 
renderSpriteProjection(void)
{
  sprite_t visibleSprites[NUM_SPRITES];
  int numVisibleSprites = 0;

  //find sprites that are visible (inside FOV)
  for (int i = 0; i < NUM_SPRITES; i++)
  {
    float angleSpritePlayer = player.rotationAngle - atan2(gtow(sprites[i].y) - player.y, gtow(sprites[i].x) - player.x);

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
      sprites[i].distance = distanceBetweenPoints(gtow(sprites[i].x), gtow(sprites[i].y), player.x, player.y);
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

    float perpDistance = spr.distance * cos(spr.angle);

    float sprHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;
    float sprWidith = sprHeight;

    float sprTopY = ((float)WINDOW_HEIGHT / 2) - (sprHeight / 2);
    sprTopY = (sprTopY < 0) ? 0 : sprTopY;

    float sprBottomY = ((float)WINDOW_HEIGHT / 2) + (sprHeight / 2);
    sprBottomY = (sprBottomY > WINDOW_HEIGHT) ? WINDOW_HEIGHT : sprBottomY;

    float spriteAngle = atan2(gtow(spr.y) - player.y, gtow(spr.x) - player.x) - player.rotationAngle;
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
      gtow(sprites[i].x) * MINIMAP_SCALE_FACTOR, 
      gtow(sprites[i].y) * MINIMAP_SCALE_FACTOR,
      1, 1, 
      (sprites[i].visible) ? 0xFF00FFFF : 0xFF444444);
  }
}
