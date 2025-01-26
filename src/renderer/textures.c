#include "textures.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

upng_t* textures[NUM_TEXTURES];

static const char* textureFileNames[NUM_TEXTURES] = {
    "./images/redbrick.png",    //[1]
    "./images/purplestone.png", //[2]
    "./images/mossystone.png",  //[3]
    "./images/eagle.png",       //[4]
    "./images/wall_insane.png", //[5]
    "./images/door_insane.png", //[6]
    "./images/door_locked.png", //[7]
    "./images/wall.png",        //[8]
    "./images/barrel.png",      //[9]
    "./images/light.png",       //[10]
    "./images/table.png",       //[11]
    "./images/guard.png",       //[12]
    "./images/armor.png",       //[13]
    "./images/key_red.png",     //[14]
    "./images/key_blue.png",    //[15]
    "./images/info.png",        //[16]
    "./images/chair.png",       //[17]
    "./images/mirrorr.png",     //[18]
    "./images/mirrorr2.png",    //[19]
    "./images/mirrorr3.png",    //[20]
    "./images/clock.png",       //[21]
    "./images/clock2.png",      //[22]
    "./images/clock3.png",      //[23]
};

typedef struct {
  float x;
  float y;
  float distance;
  float angle;
  int texture;
} sprite_t;

void
loadTextures()
{
  for (int i = 0; i < NUM_TEXTURES; i++)
  {
    upng_t* upng = upng_new_from_file(textureFileNames[i]);
    if(upng != NULL)
    {
      upng_decode(upng);
      if(upng_get_error(upng) == UPNG_EOK)
      {
        textures[i] = upng;
      } else {
        printf("Error decoding texture file %s \n", textureFileNames[i]);
      }
    } else {
      printf("Error loading texture %s \n", textureFileNames[i]);
    }
  }
}

void
freeTextures()
{
  for(int i = 0; i < NUM_TEXTURES; i++)
  {
    upng_free(textures[i]);
  }
}
