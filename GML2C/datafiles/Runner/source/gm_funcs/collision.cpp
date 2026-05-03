#include <stdio.h>
#include <string.h>
#include "drawing.h"
#include "misc.h"
#include "get_spriteinfo.h"

//helper funcs
void get_current_object_touching(int x, int y, float xscale, float yscale, int sprite_index){
    if (sprite_index < 0)
        return;

    int boundingbox_left = x + (SpriteCollideLEFT[sprite_index] - sprite_get_xoffset(sprite_index)) * xscale;
    int boundingbox_right = x + (SpriteCollideRIGHT[sprite_index] - sprite_get_xoffset(sprite_index)) * xscale;
    int boundingbox_top = y + (SpriteCollideTOP[sprite_index] - sprite_get_yoffset(sprite_index)) * yscale;
    int boundingbox_bottom = y + (SpriteCollideBOTTOM[sprite_index] - sprite_get_yoffset(sprite_index)) * yscale;

    float line_thick = 6.0f;
    C2D_DrawLine(boundingbox_left,  boundingbox_top, C2D_Color32(255, 0, 0, 255), boundingbox_right, boundingbox_top, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
    C2D_DrawLine(boundingbox_right, boundingbox_top, C2D_Color32(255, 0, 0, 255), boundingbox_right, boundingbox_bottom, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
    C2D_DrawLine(boundingbox_right, boundingbox_bottom, C2D_Color32(255, 0, 0, 255), boundingbox_left,  boundingbox_bottom, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
    C2D_DrawLine(boundingbox_left,  boundingbox_bottom, C2D_Color32(255, 0, 0, 255), boundingbox_left, boundingbox_top, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
}

//the gm funcs
bool place_meeting(float x, float _y, int object){
    return false;
}

bool place_empty(float x, float _y, int object){
    return true;
}

bool place_free(float x, float _y){
    return true;
}

bool position_empty(float x, float _y){
    return true;
}

bool position_meeting(float x, float _y, int object){
    return false;
}

void position_destroy(float x, float _y){
    //so empty...
}

int instance_place(float x, float _y, int object){
    return 0;
}

int instance_position(float x, float _y, int object){
    return 0;
}
