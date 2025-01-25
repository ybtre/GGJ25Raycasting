#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "defs.h"
#include "textures.h"
#include "graphics.h"
#include "map.h"
#include "player.h"
#include "ray.h"


bool isGameRunning = false;
int ticksLastFrame;

void
setup()
{
  //asks uPNG lib to decode all PNG files and loads the wallTextures array
  loadWallTextures();
}


void
processInput()
{
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_QUIT: {
    isGameRunning = false;
    break;
  }
  case SDL_KEYDOWN: {
    if (event.key.keysym.sym == SDLK_ESCAPE)
      isGameRunning = false;
    if (event.key.keysym.sym == SDLK_UP)
      player.walkDirection = +1;
    if (event.key.keysym.sym == SDLK_DOWN)
      player.walkDirection = -1;
    if (event.key.keysym.sym == SDLK_RIGHT)
      player.turnDirection = +1;
    if (event.key.keysym.sym == SDLK_LEFT)
      player.turnDirection = -1;
    break;
  }
  case SDL_KEYUP: {
    if (event.key.keysym.sym == SDLK_UP)
      player.walkDirection = 0;
    if (event.key.keysym.sym == SDLK_DOWN)
      player.walkDirection = 0;
    if (event.key.keysym.sym == SDLK_RIGHT)
      player.turnDirection = 0;
    if (event.key.keysym.sym == SDLK_LEFT)
      player.turnDirection = 0;
    break;
  }
  }
}

void
update()
{
  int timeToWait = FRAME_TIME_LENGTH_MS - (SDL_GetTicks() - ticksLastFrame);
  if(timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH_MS)
  {
    SDL_Delay(timeToWait);
  }

  float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

  ticksLastFrame = SDL_GetTicks();
  // printf("%f\n",deltaTime);

  movePlayer(deltaTime);
  castAllRays();
}

typedef struct RGB {
    int r;
    int g;
    int b;
} rgb_t;

typedef struct HEX {
    uint32_t val;
} hex_t;

rgb_t
HEXToRGB(hex_t hex)
{
  rgb_t rgbColor;
  rgbColor.b = ((hex.val >> 16) & 0xFF) / 255.0;  // Extract the BB byte
  rgbColor.g = ((hex.val >> 8) & 0xFF) / 255.0;   // Extract the GG byte
  rgbColor.r = ((hex.val) & 0xFF) / 255.0;        // Extract the RR byte

  return rgbColor;
}

hex_t
RGBToHEX(rgb_t rgb)
{
    hex_t result;
    result.val = ((rgb.b & 0xff) << 16) + ((rgb.g & 0xff) << 8) + (rgb.r & 0xff);
    return result;
}

rgb_t
tintRGB(rgb_t from, rgb_t to)
{
    rgb_t new;
    new.r = from.r + (to.r - from.r) * 1;
    new.g = from.g + (to.g - from.g) * 1;
    new.b = from.b + (to.b - from.b) * 1;
    return new;
}

void
renderWallProjection()
{
  for(int x = 0; x < NUM_RAYS; x++)
  {
    float perpDistance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle);
    float projectedWallHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;

    int wallStripHeight = projectedWallHeight;

    int wallTopPixel = (WINDOW_HEIGHT/2) - (wallStripHeight / 2);
    wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

    int wallBottomPixel = (WINDOW_HEIGHT/2) + (wallStripHeight/2);
    wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

    uint8_t tint = 80 / perpDistance;
    hex_t v = {.val = 0xFF21103A};
    rgb_t ceiling = HEXToRGB(v);
    rgb_t to = {.r = tint, .g = tint, .b = tint};
    rgb_t tinted = tintRGB(ceiling, to);
    hex_t tintedCeiling = RGBToHEX(tinted);

    //set the color of the ceiling
    for(int y = 0; y < wallTopPixel; y++)
    {
      drawPixel(x, y, 0xFF0b0023);
    }

    // calculate texture offset X
    int textureOffsetX;
    if (rays[x].wasHitVertical)
        textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
    else
        textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;

    // get the correct texture ID number from the map content
    int texNum = rays[x].wallHitContent - 1;

    int texture_width = wallTextures[texNum].width;
    int texture_height = wallTextures[texNum].height;

    // render the wall from wallTopPixel to wallBottomPixel
    for (int y = wallTopPixel; y < wallBottomPixel; y++) {
        // calculate texture offset Y
        int distanceFromTop = y + (wallStripHeight / 2) - (WINDOW_HEIGHT / 2);
        int textureOffsetY = distanceFromTop * ((float)texture_height / wallStripHeight);

        // set the color of the wall based on the color from the texture
        uint32_t texelColor = wallTextures[texNum].texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
        drawPixel(x, y, texelColor);
    }

    //set the color of the floor
    for(int y = wallBottomPixel; y < WINDOW_HEIGHT; y++)
    {
      drawPixel(x, y, 0xFF231938);
    }
  }
}

void
render()
{
  // clearColorBuffer(0xFF000000);

  renderWallProjection();

  //display the minimap
  renderMap();
  renderRays();
  renderPlayer();

  renderColorBuffer();
}

void
releaseResources(void)
{
  freeWallTextures();
  destroyWindow();
}

int
main(int argc, char* argv[])
{
  isGameRunning = initializeWindow();

  setup();

  while (isGameRunning) {
    processInput();
    update();
    render();
  }

  releaseResources();

  return 0;
}
