#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include <float.h>
#include "defs.h"
#include "player.h"

typedef struct Ray {
  float rayAngle;
  float wallHitX;
  float wallHitY;
  float distance;
  bool wasHitVertical;
  int wallHitContent;
} ray_t;

extern ray_t rays[NUM_RAYS];

float normalizeAngle(float angle);
float distanceBetweenPoints(float x1, float y1, float x2, float y2);
void castRay(float rayAngle, int stripId);
void castAllRays();
void renderRays();


#endif
