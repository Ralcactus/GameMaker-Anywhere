#include <stdio.h>
#include <string.h>
#include "drawing.h"
#include "misc.h"
#include "get_spriteinfo.h"

//helper funcs
int get_current_object_touching(int x, int y, float xscale, float yscale, int sprite_index, int selfid){
    if (sprite_index < 0)
        return 0;

    selfid -= 100000;
    int boundingbox_self_left = x + (SpriteCollideLEFT[sprite_index] - sprite_get_xoffset(sprite_index)) * xscale;
    int boundingbox_self_right = x + (SpriteCollideRIGHT[sprite_index] - sprite_get_xoffset(sprite_index)) * xscale;
    int boundingbox_self_top = y + (SpriteCollideTOP[sprite_index] - sprite_get_yoffset(sprite_index)) * yscale;
    int boundingbox_self_bottom = y + (SpriteCollideBOTTOM[sprite_index] - sprite_get_yoffset(sprite_index)) * yscale;

    for (int i = 0; i < object_count; i++){
        if (otherobject_id[i] == selfid) 
            continue;
        
        int boundingbox_other_left = otherobject_x[i] + (SpriteCollideLEFT[otherobject_sprite[i]] - sprite_get_xoffset(otherobject_sprite[i]))* otherobject_xscale[i];
        int boundingbox_other_right = otherobject_x[i] + (SpriteCollideRIGHT[otherobject_sprite[i]] - sprite_get_xoffset(otherobject_sprite[i]))* otherobject_xscale[i];
        int boundingbox_other_top = otherobject_y[i] + (SpriteCollideTOP[otherobject_sprite[i]] - sprite_get_yoffset(otherobject_sprite[i]))* otherobject_yscale[i];
        int boundingbox_other_bottom = otherobject_y[i] + (SpriteCollideBOTTOM[otherobject_sprite[i]] - sprite_get_yoffset(otherobject_sprite[i]))* otherobject_yscale[i];

        if (boundingbox_self_right >= boundingbox_other_left  && boundingbox_self_left <= boundingbox_other_right && boundingbox_self_bottom >= boundingbox_other_top && boundingbox_self_top <= boundingbox_other_bottom){
            printf("Object %d is touching object %d\n", selfid, otherobject_id[i]);
            return otherobject_id[i];
        }
    }


    /*
    int line_thick = 6;
    int boundingbox_left = x + (SpriteCollideLEFT[sprite_index] - sprite_get_xoffset(sprite_index)) * xscale;
    int boundingbox_right = x + (SpriteCollideRIGHT[sprite_index] - sprite_get_xoffset(sprite_index)) * xscale;
    int boundingbox_top = y + (SpriteCollideTOP[sprite_index] - sprite_get_yoffset(sprite_index)) * yscale;
    int boundingbox_bottom = y + (SpriteCollideBOTTOM[sprite_index] - sprite_get_yoffset(sprite_index)) * yscale;
    C2D_DrawLine(boundingbox_left, boundingbox_top, C2D_Color32(255, 0, 0, 255), boundingbox_right, boundingbox_top, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
    C2D_DrawLine(boundingbox_right, boundingbox_top, C2D_Color32(255, 0, 0, 255), boundingbox_right, boundingbox_bottom, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
    C2D_DrawLine(boundingbox_right, boundingbox_bottom, C2D_Color32(255, 0, 0, 255), boundingbox_left, boundingbox_bottom, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
    C2D_DrawLine(boundingbox_left, boundingbox_bottom, C2D_Color32(255, 0, 0, 255), boundingbox_left, boundingbox_top, C2D_Color32(255, 0, 0, 255), line_thick, 0.0f);
    */

    return noone;
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
