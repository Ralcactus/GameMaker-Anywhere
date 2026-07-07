#include <stdio.h>
#include <string.h>
#include "drawing.h"
#include "misc.h"
#include "../helpers/get_spriteinfo.h"
#include "../helpers/var_in_object_running.h"


//helper funcs
void draw_boundbox(){
    #ifdef __3DS__
        if (sprite_index < 0)
            return;

        //draw bounding box
        int line_thick = 6;
        int boundingbox_left = x + (SpriteCollideLEFT[sprite_index] - sprite_get_xoffset(sprite_index)) * (double)image_xscale;
        int boundingbox_right = x + (SpriteCollideRIGHT[sprite_index] - sprite_get_xoffset(sprite_index)) * (double)image_xscale;
        int boundingbox_top = y + (SpriteCollideTOP[sprite_index] - sprite_get_yoffset(sprite_index)) * (double)image_yscale;
        int boundingbox_bottom = y + (SpriteCollideBOTTOM[sprite_index] - sprite_get_yoffset(sprite_index)) * (double)image_yscale;
        C2D_DrawLine(boundingbox_left, boundingbox_top, C2D_Color32(255, 0, 0, 255), boundingbox_right, boundingbox_top, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
        C2D_DrawLine(boundingbox_right, boundingbox_top, C2D_Color32(255, 0, 0, 255), boundingbox_right, boundingbox_bottom, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
        C2D_DrawLine(boundingbox_right, boundingbox_bottom, C2D_Color32(255, 0, 0, 255), boundingbox_left, boundingbox_bottom, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
        C2D_DrawLine(boundingbox_left, boundingbox_bottom, C2D_Color32(255, 0, 0, 255), boundingbox_left, boundingbox_top, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
    #endif
}

//the gm funcs
bool place_meeting(float _x, float _y, int object){
    return false;
}

bool place_empty(float _x, float _y, int object){
    return true;
}

bool place_free(float _x, float _y){
    return true;
}

bool position_empty(float _x, float _y){
    return true;
}

bool position_meeting(float _x, float _y, int object){
    return false;
}

void position_destroy(float _x, float _y){
    //so empty...
}

int instance_place(float _x, float _y, int object){
    return 0;
}

int instance_position(float _x, float _y, int object){
    return 0;
}
