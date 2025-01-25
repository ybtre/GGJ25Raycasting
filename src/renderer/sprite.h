#ifndef SPRITE_H
#define SPRITE_H

#include "defs.h"
#include "utils.h"
#include "textures.h"
#include "graphics.h"
#include "player.h"
#include "ray.h"
#include "upng.h"


typedef struct {
  float x;
  float y;
  float distance;
  float angle;
  bool visible;
  int texture;
} sprite_t;

void renderSpriteProjection(void);
void removeSprite(int spriteID);
void renderMapSprites(void);

#endif
