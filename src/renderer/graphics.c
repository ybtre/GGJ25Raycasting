#include "graphics.h"
#include "defs.h"
#include <stdint.h>
#include <stdio.h>

static SDL_Window *window     = NULL;
static SDL_Renderer *renderer = NULL;

static color_t* colorBuffer = NULL;
static SDL_Texture* colorBufferTexture = NULL;

bool
initializeWindow(void)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  if(TTF_Init() < 0)
  {
    printf("SDL_ttf could not initialise! ttf_error : %s\n", TTF_GetError());
    return false;
  }

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  int fullScreenWidth = display_mode.w/2;
  int fullScreenHeight = display_mode.h/2;

  window =
      // SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      //                  fullScreenWidth, fullScreenHeight, SDL_WINDOW_BORDERLESS);
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       fullScreenWidth, fullScreenHeight, SDL_WINDOW_BORDERLESS);
  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_SOFTWARE);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  //allocate the total amount of bytes in memory to hold the color buffer
  colorBuffer = (color_t*) malloc(sizeof(color_t) * (color_t)WINDOW_WIDTH * (color_t)WINDOW_HEIGHT);

  // create an SDL_Texture to display the color buffer
  colorBufferTexture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_RGBA32,
    SDL_TEXTUREACCESS_STREAMING,
    WINDOW_WIDTH,
    WINDOW_HEIGHT
  );

  return true;
}

void
destroyWindow(void)
{
  free(colorBuffer);
  SDL_DestroyTexture(colorBufferTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void
clearColorBuffer(color_t color)
{
  // manual color buffer color set
  for(int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
      colorBuffer[i] = color;
  }
}

void
renderColorBuffer(void)
{
  SDL_UpdateTexture(
    colorBufferTexture,
    NULL,
    colorBuffer,
    (int)((color_t)WINDOW_WIDTH * sizeof(color_t))
  );

  SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);

  TTF_Font *font = TTF_OpenFont("../../font.ttf", 64);
 if (!font){
    printf("Failed to load font: %s\n", TTF_GetError());
  }

  // Create surface with rendered text
  SDL_Color textColor = {0, 0, 0, 255}; // black color
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Hello World!", textColor);

  if (!textSurface) {
    printf("Failed to create text surface: %s\n", TTF_GetError());
  }

  // Create texture from surface
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

  if (!textTexture){
    printf("Failed to create text texture: %s\n", SDL_GetError());
  }

  // Render text
  SDL_Rect textRect = {50, 50, textSurface->w, textSurface->h}; // rectangle where the text is drawn 
  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);


  SDL_RenderPresent(renderer);
}

void
drawPixel(int x, int y, color_t color)
{
  colorBuffer[(WINDOW_WIDTH * y) + x] = color;
};

void
drawRect(int x, int y, int width, int height, color_t color)
{
  for (int i = x; i <= (x + width); i++)
  {
    for (int j = y; j <= (y + height); j++)
    {
      drawPixel(i, j, color);
    }
  }
}

void
drawLine(int x0, int y0, int x1, int y1, color_t color)
{
  int deltaX = (x1 - x0);
  int deltaY = (y1 - y0);

  int longestSideLength = (abs(deltaX) >= abs(deltaY)) ? abs(deltaX) : abs(deltaY);

  float xIncrement = deltaX / (float)longestSideLength;
  float yIncrement = deltaY / (float)longestSideLength;

  float currentX = x0;
  float currentY = y0;

  for (int i = 0; i < longestSideLength; i++)
  {
    drawPixel(round(currentX), round(currentY), color);
    currentX += xIncrement;
    currentY += yIncrement;
  }
}
