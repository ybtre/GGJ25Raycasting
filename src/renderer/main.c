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
#include "sprite.h"
#include "upng.h"


bool isGameRunning = false;
int ticksLastFrame;

float sanity_meter = .5;

void
darkenColorBy(color_t* color, float factor)
{
    color_t a = (*color & 0xFF000000);
    color_t b = (*color & 0x00FF0000) * factor;
    color_t g = (*color & 0x0000FF00) * factor;
    color_t r = (*color & 0x000000FF) * factor;

    *color = a | (b & 0x00FF0000) | (g & 0x0000FF00) | (r & 0x0000FF);
}


/////////////////////////////////////////////////////
/// GAMEPLAY FUNCTION DECLARATIONS
/////////////////////////////////////////////////////
void
set_sanity_meter(float value)
{
  sanity_meter = value;
}


void
setup()
{
  //asks uPNG lib to decode all PNG files and loads the wallTextures array
  loadTextures();
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

  int player_grid_x = player.x / TILE_SIZE;
  int player_grid_y = player.y / TILE_SIZE;

  // printf("%i %i \n", player_grid_x, player_grid_y);

  if(player_grid_x == 3 && player_grid_y == 1)
  {
    set_sanity_meter(.2);
  }

  if(player_grid_x == 4 && player_grid_y == 2)
  {
    set_sanity_meter(.8); 
  }

  castAllRays();
}


void
renderWallProjection()
{
  for(int x = 0; x < NUM_RAYS; x++)
  {
    //calculate the perpendicular distance to avoid fish-eye distortion
    float perpDistance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle);

    //calcualte the projected wall height
    float wallHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;

    //find the wall top Y value
    int wallTopY = (WINDOW_HEIGHT/2) - ((int)wallHeight / 2);
    wallTopY = wallTopY < 0 ? 0 : wallTopY;

    //find the wall bottom Y value
    int wallBottomY = (WINDOW_HEIGHT/2) + ((int)wallHeight/2);
    wallBottomY = wallBottomY > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomY;

    //Draw the celing
    for(int y = 0; y < wallTopY; y++)
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

    int texture_width = upng_get_width(textures[texNum]);
    int texture_height = upng_get_height(textures[texNum]);

    // render the wall from wallTopY to wallBottomY
    for (int y = wallTopY; y < wallBottomY; y++) {
      // calculate texture offset Y
      int distanceFromTop = y + ((int)wallHeight / 2) - (WINDOW_HEIGHT / 2);
      int textureOffsetY = distanceFromTop * ((float)texture_height / wallHeight);

      // set the color of the wall based on the color from the texture
      color_t* wallTextureBuffer = (color_t*)upng_get_buffer(textures[texNum]);
      color_t texelColor = wallTextureBuffer[(texture_width * textureOffsetY) + textureOffsetX];

      if(rays[x].wallHitContent == 8)
      {
        darkenColorBy(&texelColor, sanity_meter);
      }

      drawPixel(x, y, texelColor);
    }

    //draw the floor
    for(int y = wallBottomY; y < WINDOW_HEIGHT; y++)
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
  renderSpriteProjection();

  //display the minimap
  renderMap();
  renderRays();
  renderMapSprites();
  renderPlayer();

  renderColorBuffer();
}

void
releaseResources(void)
{
  freeTextures();
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
