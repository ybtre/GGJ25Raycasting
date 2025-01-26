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


#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_ttf.h"

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

TTF_Font* font;
SDL_Surface* text_surface;
SDL_Texture* text_texture;
SDL_Surface* choice1_surface;
SDL_Texture* choice1_texture;
SDL_Surface* choice2_surface;
SDL_Texture* choice2_texture;

bool x_pressed = 0;
bool c_pressed = 0;

void
setMainText(const char* newText)
{
    text_surface = TTF_RenderText_Blended_Wrapped(
    font, 
    newText,
    (SDL_Color){255,255,255,255},
    1000);
  if(text_surface == NULL)
  {
    printf("surface creation failed %s\n", TTF_GetError());
  }
  text_texture = SDL_CreateTextureFromSurface(
    renderer,
    text_surface);
}

void
setChoice1Text(const char* newText)
{
  
  choice1_surface = TTF_RenderText_Blended_Wrapped(
    font, 
    newText,
    (SDL_Color){255,255,255,255},
    500);
  choice1_texture = SDL_CreateTextureFromSurface(
    renderer,
    choice1_surface);
}

void
setChoice2Text(const char* newText)
{
  choice2_surface = TTF_RenderText_Blended_Wrapped(
    font, 
    newText,
    (SDL_Color){255,255,255,255},
    500);
  choice2_texture = SDL_CreateTextureFromSurface(
    renderer,
    choice2_surface);
}

void
setup()
{
  //asks uPNG lib to decode all PNG files and loads the wallTextures array
  loadTextures();

  TTF_Init();

  font = TTF_OpenFont("font.ttf", 16);
  if(font == NULL)
  {
    printf("Font loading failed %s\n", TTF_GetError());
  }

  setMainText("You wake up in a dim, cold maze. The walls stretch endlessly upward, and the air is thick with an invisible weight pressing down on your chest. The space feels both vast and claustrophobic, a reflection of your own mind. You can not remember how you got here. There is no clear exit, only the faintest glow of a door ahead of you. You know you have to make choices to move forward, but each choice seems heavy, as if it could tilt the balance between feeling slightly better or just a bit worse.");
  
  setChoice1Text("Press [X] to START");
  setChoice2Text(" ");
}

//introduction room vars
int INTRO = 1;
bool intro1 = false;
bool intro2 = false;
bool intro3 = false;
bool key1=false;
/////////////////////////
//chapter1 vars
int CH1 = 0;
bool ch1title = false;
bool ch1main = false;
int ch1choice = 0;
bool key2=false;
/////////////////////////
//chapter2 vars
int CH2 = 0;
bool ch2title = false;
bool ch2main = false;
int ch2choice = 0;
bool key3=false;
/////////////////////////

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

    if(intro1 == true)
    {
      if (event.key.keysym.sym == SDLK_UP)
        player.walkDirection = +1;
      if (event.key.keysym.sym == SDLK_DOWN)
        player.walkDirection = -1;
      if (event.key.keysym.sym == SDLK_RIGHT)
        player.turnDirection = +1;
      if (event.key.keysym.sym == SDLK_LEFT)
        player.turnDirection = -1;
    }
    if (event.key.keysym.sym == SDLK_x)
    {
      x_pressed = true;
      if(ch1main==true&&x_pressed == true)
      {
        ch1choice = 1;
      }
    }
    if (event.key.keysym.sym == SDLK_c)
      c_pressed = true;
      if(ch1main==true&&c_pressed == true)
      {
        ch1choice = 2;
      }
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
    if (event.key.keysym.sym == SDLK_x)
      x_pressed = false;
    if (event.key.keysym.sym == SDLK_c)
      c_pressed = false;
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

  printf("%i %i \n", player_grid_x, player_grid_y);

{//iNTRODUCTION ROOM
  if(INTRO == 1)
  {
    if(x_pressed && intro1 == false)
    {
      intro1=true;  
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }

    if(player_grid_x == 1 && player_grid_y == 4)
    {
      intro2=true;
    } else{
      intro2=false;
    }
    if(intro2 == true) {
        setMainText("Every decision takes you deeper into the labyrinth. Some choices promise relief, others bring doubt. The voice in your mind whispers, Only through these choices can you find a way forward. But what if the way forward leads to more pain? Or what if you are just walking in circles, never escaping the maze?");
        setChoice1Text(" ");
        setChoice2Text(" ");
    }else if(intro2==false &&intro1==true){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }

    if(player_grid_x == 2 && player_grid_y == 5)
    {
      intro3=true;
    } else {
      intro3=false;
    }

    if(intro3==true)
    {
      setMainText("Each choice you make will either bring you a little closer to clarity or to more confusion. The journey is yours alone. Pickup the key and move to the next chapter.");
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(intro3==false && intro1==true &&intro2==false)
    {
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }

    if(player_grid_x == 4 && player_grid_y == 5)
    {
      key1=true;
    }
      
    if(key1==true)
    {
      // printf("%i\n",getMapContent(4*TILE_SIZE,6*TILE_SIZE));
      setMapContent(4, 6, 0);
      removeSprite(2);
      INTRO = 2;
      CH1 = 1;
    }
  }
}

{//CHAPTER 1
  if(CH1 == 1)
  {
    if(player_grid_x == 4 && player_grid_y == 6)
    {
      ch1title=true;
    } else{
      ch1title=false;
    }
    if(ch1title == true) {
        setMainText("Chapter 1: The Chair of Surrender");
        setChoice1Text(" ");
        setChoice2Text(" ");
    }else if(ch1title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }

     if(player_grid_x == 2 && player_grid_y == 9)
    {
      ch1main=true;
    } else{
      ch1main=false;
    }
    if(ch1main == true && ch1choice == 0) {
        setMainText("The first room is small and unassuming, with a single wooden chair in the center. It seems simple, almost comforting, but you feel a deep, unexplainable pull toward it. The chair represents rest, but also surrender. The voice in your mind urges you to sit, promising you a moment of peace. The walls around you grow tighter as you stand still, as though waiting for something to happen.");
        setChoice1Text("[X] Sit down in the chair.");
        setChoice2Text("[C] Walk away from the chair.");
    } else if(ch1main == true && ch1choice == 1)
    {
      setMainText("You take a seat. The relief is immediate. Your body relaxes, and for a brief moment, the pressure in your chest lightens. But soon, you realize the chair is too comfortable. It is almost too easy. The moment of peace fades into a numbing stillness, and you begin to feel trapped by the comfort, as if you have stopped moving forward. You are not better, just... stalled."); 
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch1main == true && ch1choice == 2)
    {
      setMainText("You resist the temptation to sit. The act of turning your back on the chair feels like a small victory, even though the walls feel colder and more oppressive. The pressure in your chest does not completely disappear, but there's a subtle shift. Moving forward feels slightly easier, though the path remains uncertain. You might not have found rest, but you have kept moving.");
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch1title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }
  }

  if(ch1choice > 0)
  {
    activateSprite(5);
  }

  if(player_grid_x == 4 && player_grid_y == 11)
  {
    key2=true;
  }
    
  if(key2==true)
  {
    // printf("%i\n",getMapContent(4*TILE_SIZE,6*TILE_SIZE));
    setMapContent(4, 12, 0);
    removeSprite(5);
    CH1 = 2;
    CH2 = 1;
  }
}

// {//CHAPTER 2
//   if(CH2 == 1)
//   {
//     if(player_grid_x == 4 && player_grid_y == 6)
//     {
//       ch1title=true;
//     } else{
//       ch1title=false;
//     }
//     if(ch1title == true) {
//         setMainText("Chapter 1: The Chair of Surrender");
//         setChoice1Text(" ");
//         setChoice2Text(" ");
//     }else if(ch1title==false){
//       setMainText(" ");
//       setChoice1Text(" ");
//       setChoice2Text(" ");
//     }

//      if(player_grid_x == 2 && player_grid_y == 9)
//     {
//       ch1main=true;
//     } else{
//       ch1main=false;
//     }
//     if(ch1main == true && ch1choice == 0) {
//         setMainText("The first room is small and unassuming, with a single wooden chair in the center. It seems simple, almost comforting, but you feel a deep, unexplainable pull toward it. The chair represents rest, but also surrender. The voice in your mind urges you to sit, promising you a moment of peace. The walls around you grow tighter as you stand still, as though waiting for something to happen.");
//         setChoice1Text("[X] Sit down in the chair.");
//         setChoice2Text("[C] Walk away from the chair.");
//     } else if(ch1main == true && ch1choice == 1)
//     {
//       setMainText("You take a seat. The relief is immediate. Your body relaxes, and for a brief moment, the pressure in your chest lightens. But soon, you realize the chair is too comfortable. It is almost too easy. The moment of peace fades into a numbing stillness, and you begin to feel trapped by the comfort, as if you have stopped moving forward. You are not better, just... stalled."); 
//       setChoice1Text(" ");
//       setChoice2Text(" ");
//     } else if(ch1main == true && ch1choice == 2)
//     {
//       setMainText("You resist the temptation to sit. The act of turning your back on the chair feels like a small victory, even though the walls feel colder and more oppressive. The pressure in your chest does not completely disappear, but there's a subtle shift. Moving forward feels slightly easier, though the path remains uncertain. You might not have found rest, but you have kept moving.");
//       setChoice1Text(" ");
//       setChoice2Text(" ");
//     } else if(ch1title==false){
//       setMainText(" ");
//       setChoice1Text(" ");
//       setChoice2Text(" ");
//     }
//   }

//   if(ch1choice > 0)
//   {
//     activateSprite(5);
//   }

//   if(player_grid_x == 4 && player_grid_y == 11)
//   {
//     key2=true;
//   }
    
//   if(key2==true)
//   {
//     // printf("%i\n",getMapContent(4*TILE_SIZE,6*TILE_SIZE));
//     setMapContent(4, 12, 0);
//     removeSprite(5);
//     CH1 = 2;
//     CH2 = 1;
//   }
// }

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

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  int fullScreenWidth = display_mode.w/1;
  int fullScreenHeight = display_mode.h/1;

  renderColorBuffer();
  SDL_RenderCopy(renderer, text_texture, NULL,
                 &(SDL_Rect){
                 fullScreenWidth /2   - text_surface->w / 2,
                 (fullScreenHeight / 2) / 4 - (text_surface->h / 2),
                 text_surface->w,
                 text_surface->h
                 });

  SDL_RenderCopy(renderer, choice1_texture, NULL,
                 &(SDL_Rect){
                 fullScreenWidth /2 - choice1_surface->w / 2,
                 (fullScreenHeight /2) *1.7 + (choice1_surface->h / 2),
                 choice1_surface->w,
                 choice1_surface->h
                 });
  SDL_RenderCopy(renderer, choice2_texture, NULL,
                 &(SDL_Rect){
                 fullScreenWidth / 2  - choice2_surface->w / 2,
                 (fullScreenHeight / 2) * 1.8 + (choice2_surface->h / 2),
                 choice2_surface->w,
                 choice2_surface->h
                 });

  SDL_RenderPresent(renderer);
}

void
releaseResources(void)
{
  TTF_CloseFont(font);
  SDL_FreeSurface(text_surface);
  SDL_DestroyTexture(text_texture);
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