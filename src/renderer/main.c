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
int morale = 0;
//0-neutral [1,3] good [-1 -3] bad >-3 radical

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
//chapter3 vars
int CH3 = 0;
bool ch3title = false;
bool ch3main = false;
int ch3choice = 0;
bool key4=false;
/////////////////////////
//chapter4 vars
int CH4 = 0;
bool ch4title = false;
bool ch4main = false;
int ch4choice = 0;
bool key5=false;
/////////////////////////
//chapter5 vars
int CH5 = 0;
bool ch5title = false;
bool ch5main = false;
int ch5choice = 0;
bool key6=false;
/////////////////////////
//chapter6 vars
int CH6 = 0;
bool ch6title = false;
bool ch6main = false;
int ch6choice = 0;
bool key7=false;
/////////////////////////
//chapter7/FINALE
int CH7 = 0;
bool ch7title = false;
bool ch7main = false;
int ch7choice = 0;
bool key8=false;
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
        morale++;
      }
      if(ch2main==true&&x_pressed == true)
      {
        ch2choice = 1;
        morale++;
      }
      if(ch3main==true&&x_pressed == true)
      {
        ch3choice = 1;
        morale++;
      }
      if(ch4main==true&&x_pressed == true)
      {
        ch4choice = 1;
        morale++;
      }
      if(ch5main==true&&x_pressed == true)
      {
        ch5choice = 1;
        morale++;
      }
      if(ch6main==true&&x_pressed == true)
      {
        ch6choice = 1;
        morale++;
      }
    }
    if (event.key.keysym.sym == SDLK_c)
      c_pressed = true;
      if(ch1main==true&&c_pressed == true)
      {
        ch1choice = 2;
        morale--;
      }
      if(ch2main==true&&c_pressed == true)
      {
        ch2choice = 2;
        morale--;
      }
      if(ch3main==true&&c_pressed == true)
      {
        ch3choice = 2;
        morale--;
      }
      if(ch4main==true&&c_pressed == true)
      {
        ch4choice = 2;
        morale--;
      }
      if(ch5main==true&&c_pressed == true)
      {
        ch5choice = 2;
        morale--;
      }
      if(ch6main==true&&c_pressed == true)
      {
        ch6choice = 2;
        morale--;
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

{//CHAPTER 2
  if(CH2 == 1)
  {
    if(player_grid_x == 4 && player_grid_y == 12)
    {
      ch2title=true;
    } else{
      ch2title=false;
    }
    if(ch2title == true) {
        setMainText("Chapter 2: The Mirror of Reflection.");
        setChoice1Text(" ");
        setChoice2Text(" ");
    }else if(ch2title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }

     if(player_grid_x == 3 && player_grid_y == 15)
    {
      ch2main=true;
    } else{
      ch2main=false;
    }
    if(ch2main == true && ch2choice == 0) {
        setMainText("You enter a room filled with mirrors, each one distorting your image in unsettling ways. Some show you looking tired, others angry, sad, or even hopeless. One large mirror in the center of the room draws your attention. It reflects you more clearly, but its surface is cracked and warped, like the reflections in your mind.Look at what you've become, a voice echoes from the mirrors. It feels like an accusation, but also a plea for understanding. You can almost hear your own voice, trapped in the glass, trying to break free.");
        setChoice1Text("[X] Stare into the mirror.");
        setChoice2Text("[C] Turn away from the mirror.");
    } else if(ch2main == true && ch2choice == 1)
    {
      setMainText("You confront the mirror, seeing every flaw, every bit of pain reflected back at you. It is painful, but in that pain, you find a strange clarity. The reflection is harsh, but it forces you to see the truth about where you are. There is discomfort in accepting it, but also a small sense of grounding. You do not feel better, but you feel more present."); 
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch2main == true && ch2choice == 2)
    {
      setMainText("You can not bear the sight. You turn away, feeling the weight of the decision. As you walk away, there is a faint sense of regret, as if avoiding the reflection means avoiding an important truth. But the feeling of denial is comforting, a shield from the harshness of reality. You do not feel better, but you feel a brief moment of relief.");
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch2title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }
  }

  if(ch2choice > 0)
  {
    activateSprite(8);
  }

  if(player_grid_x == 4 && player_grid_y == 17 && ch2choice > 0)
  {
    key3=true;
  }
    
  if(key3==true)
  {
    // printf("%i\n",getMapContent(4*TILE_SIZE,6*TILE_SIZE));
    setMapContent(4, 18, 0);
    removeSprite(8);
    CH2 = 2;
    CH3 = 1;
  }
}

{//CHAPTER 3
  if(CH3 == 1)
  {
    if(player_grid_x == 4 && player_grid_y == 18)
    {
      ch3title=true;
    } else{
      ch3title=false;
    }
    if(ch3title == true) {
        setMainText("Chapter 3: The Corridor of Time.");
        setChoice1Text(" ");
        setChoice2Text(" ");
    }else if(ch3title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }

     if(player_grid_x == 2 && player_grid_y == 23)
    {
      ch3main=true;
    } else{
      ch3main=false;
    }
    if(ch3main == true && ch3choice == 0) {
        setMainText("You walk through a long corridor. The walls are covered in old clocks, each ticking at different speeds, some fast, others slow. The sound of ticking fills your ears, a constant reminder of the passage of time. Each clock represents a moment of your life: the past, the future, and the present. Some of the clocks have stopped altogether. Time does not wait for you, the voice says, lingering in the air. It marches on, whether you keep up or not. You stop before a large clock, its hands frozen in place, stuck in an unknown moment.");
        setChoice1Text("[X] Move past the frozen clock.");
        setChoice2Text("[C] Stare at the frozen clock.");
    } else if(ch3main == true && ch3choice == 1)
    {
      setMainText("You choose not to linger on the past. You move past the clock, and as you do, the ticking sound seems to fade into the background. The pressure on your chest lifts slightly, but it is replaced by a quiet sadness. The present moment feels distant, out of reach. You have stopped dwelling on the past, but it still follows you like a shadow."); 
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch3main == true && ch3choice == 2)
    {
      setMainText("You linger in front of the clock, staring at the frozen hands. Time feels suspended, and for a moment, you feel trapped in the past. The weight of everything you have lost seems unbearable. But in the stillness, you find a kind of peace, a temporary pause from the rush of everything. The sadness remains, but it is no longer as suffocating. The moment of reflection offers a strange sense of control.");
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch3title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }
  }

  if(ch3choice > 0)
  {
    activateSprite(9);
  }

  if(player_grid_x == 4 && player_grid_y == 23 && ch3choice>0)
  {
    key4=true;
  }
    
  if(key4==true)
  {
    // printf("%i\n",getMapContent(4*TILE_SIZE,6*TILE_SIZE));
    setMapContent(4, 24, 0);
    removeSprite(9);
    CH3 = 2;
    CH4 = 1;
  }
}

{//CHAPTER 4
  if(CH4 == 1)
  {
    if(player_grid_x == 4 && player_grid_y == 24)
    {
      ch4title=true;
    } else{
      ch4title=false;
    }
    if(ch4title == true) {
        setMainText("Chapter 4: The Door to the Unknown");
        setChoice1Text(" ");
        setChoice2Text(" ");
    }else if(ch4title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }

     if(player_grid_x == 2 && player_grid_y == 27)
    {
      ch4main=true;
    } else{
      ch4main=false;
    }
    if(ch4main == true && ch4choice == 0) {
        setMainText("You approach a door, glowing faintly, beckoning you forward. It feels like a promise of something new, a chance to escape the maze. But you hesitate. The door feels both inviting and ominous. What lies beyond? Can you trust it? The voice murmurs in your ear. You know what is on the other side. But do you dare to see it? The door remains, but you are not sure if it is an escape or just another way to get lost.");
        setChoice1Text("[X] Open the door.");
        setChoice2Text("[C] Leave the door and walk away.");
    } else if(ch4main == true && ch4choice == 1)
    {
      setMainText("You push the door open. Beyond it, there is nothing but darkness, a vast void stretching ahead. The air feels colder, the maze behind you fading into the distance. You are not sure where you are going, but there is a sense of freedom in this unknown space. The journey is still unclear, but for now, it feels like a step forward. Even if it is uncertain, you have made a choice to move."); 
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch4main == true && ch4choice == 2)
    {
      setMainText("You turn away from the door, unsure of what lies beyond. Walking away is not easy, but there is a sense of comfort in the familiar. The maze may still be uncertain, but it is something you know. You have chosen not to risk what might be on the other side, holding onto what you already have. It is not a clear victory, but there is something solid in choosing not to leap into the unknown.");
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch4title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }
  }

  if(ch4choice > 0)
  {
    activateSprite(5);
  }

  if(player_grid_x == 4 && player_grid_y == 29)
  {
    key5=true;
  }
    
  if(key5==true)
  {
    // printf("%i\n",getMapContent(4*TILE_SIZE,6*TILE_SIZE));
    setMapContent(4, 30, 0);
    removeSprite(5);
    CH4 = 2;
    CH5 = 1;
  }
}

{//CHAPTER 5
  if(CH5 == 1)
  {
    if(player_grid_x == 4 && player_grid_y == 30)
    {
      ch5title=true;
    } else{
      ch5title=false;
    }
    if(ch5title == true) {
        setMainText("Chapter 5: The Reflection of Others");
        setChoice1Text(" ");
        setChoice2Text(" ");
    }else if(ch5title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }

     if(player_grid_x == 2 && player_grid_y == 33)
    {
      ch5main=true;
    } else{
      ch5main=false;
    }
    if(ch5main == true && ch5choice == 0) {
        setMainText("You come to a room filled with shadows, moving in strange, fluid patterns. You see glimpses of people, some familiar, some strangers, but they do not speak. They do not acknowledge you. They are just... there. You reach out to touch one of them, but it passes through your hand as if they are nothing more than illusions. You are alone in this, the voice echoes. There is no one else. There never has been.");
        setChoice1Text("[X] Reach out to the shadows.");
        setChoice2Text("[C] Ignore the shadows.");
    } else if(ch5main == true && ch5choice == 1)
    {
      setMainText("You reach out, feeling the presence of the figures around you. There is no connection, no response, but the act of reaching out gives you a small sense of belonging, even if it is just an illusion. The loneliness feels less sharp, if only for a moment. You do not feel less alone, but you feel as though you have made an effort."); 
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch5main == true && ch5choice == 2)
    {
      setMainText("You choose not to acknowledge the figures. The loneliness is undeniable, but there is something freeing in not seeking connection. You walk away, the feeling of isolation hanging over you like a heavy cloak. It is painful, but it is a kind of acceptance. You do not need anyone else. Not right now.");
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch5title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }
  }

  if(ch5choice > 0)
  {
    activateSprite(5);
  }

  if(player_grid_x == 4 && player_grid_y == 35)
  {
    key6=true;
  }
    
  if(key6==true)
  {
    // printf("%i\n",getMapContent(4*TILE_SIZE,6*TILE_SIZE));
    setMapContent(4, 36, 0);
    removeSprite(5);
    CH5 = 2;
    CH6 = 1;
  }
}

{//CHAPTER 6
  if(CH6 == 1)
  {
    if(player_grid_x == 4 && player_grid_y == 36)
    {
      ch6title=true;
    } else{
      ch6title=false;
    }
    if(ch6title == true) {
        setMainText("Chapter 6: The Final Path");
        setChoice1Text(" ");
        setChoice2Text(" ");
    }else if(ch6title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }

     if(player_grid_x == 2 && player_grid_y == 39)
    {
      ch6main=true;
    } else{
      ch6main=false;
    }
    if(ch6main == true && ch6choice == 0) {
        setMainText("The maze feels endless now. The walls seem to close in, the space narrowing. You feel the weight of every choice you have made, the moments you have turned away, the ones you have embraced. The final door looms ahead of you. It is smaller than you expected, almost unremarkable, but it feels significant. This is the end, the voice whispers. What will you do now?");
        setChoice1Text("[X] Step through the final door.");
        setChoice2Text("[C] Walk away from the door.");
    } else if(ch6main == true && ch6choice == 1)
    {
      setMainText("You open the door. On the other side is a small, quiet room. There is no light, no sound, just an emptiness that feels strangely comforting. There is a sense of peace, but it is not an escape. It is a quiet acceptance of where you are. You have made it this far, and though the maze has not disappeared, you have found a way to exist within it. The journey continues, but you have found a small sense of solace."); 
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch6main == true && ch6choice == 2)
    {
      setMainText("You leave the door behind, uncertain of what lies beyond. You continue to walk the maze, but with a new sense of determination. The path is still unclear, and the maze is still shifting, but you have made a decision not to seek the easy escape. There is still hope in movement, even when you do not know where you are going. You have chosen to keep going, even if the end is not in sight.");
      setChoice1Text(" ");
      setChoice2Text(" ");
    } else if(ch6title==false){
      setMainText(" ");
      setChoice1Text(" ");
      setChoice2Text(" ");
    }
  }

  if(ch6choice > 0)
  {
    activateSprite(5);
  }

  if(player_grid_x == 4 && player_grid_y == 41)
  {
    key7=true;
  }
    
  if(key7==true)
  {
    // printf("%i\n",getMapContent(4*TILE_SIZE,6*TILE_SIZE));
    setMapContent(4, 42, 0);
    removeSprite(5);
    CH6 = 2;
    CH7 = 1;
  }
}

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