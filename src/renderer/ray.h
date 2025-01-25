#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include <float.h>
#include "defs.h"
#include "player.h"
#include "utils.h"

typedef struct Ray {
  float rayAngle;
  float wallHitX;
  float wallHitY;
  float distance;
  bool wasHitVertical;
  int wallHitContent;
} ray_t;

extern ray_t rays[NUM_RAYS];

bool isRayFacingUp(float angle);
bool isRayFacingDown(float angle);
bool isRayFacingLeft(float angle);
bool isRayFacingRight(float angle);
void castRay(float rayAngle, int stripId);
void castAllRays();
void renderRays();


#endif
