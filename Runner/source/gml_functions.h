#pragma once
#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>

    //inputs
    #define gp_start KEY_START
    #define gp_select KEY_SELECT
    #define gp_face1 KEY_B
    #define gp_face2 KEY_A
    #define gp_face3 KEY_Y
    #define gp_face4 KEY_X
    #define gp_padl KEY_DLEFT
    #define gp_padr KEY_DRIGHT
    #define gp_padu KEY_DUP
    #define gp_padd KEY_DDOWN
#endif

#include <stdbool.h>
#include <stddef.h>

//max sprites that can be drawn at a time
#define MAX_SPRITES 1000


extern u32 g_keysDown;
extern u32 g_keysHeld;
extern u32 g_keysUp;

typedef struct
{
    C2D_Sprite spr;
} Sprite;

extern Sprite sprites[MAX_SPRITES];
extern size_t SpriteCount;




//input
bool gamepad_button_check_pressed(u32 Button);
bool gamepad_button_check(u32 Button);
bool gamepad_button_check_released(u32 Button);
bool runner_sprite_is_object(int index);

//debugging
void show_debug_message(const char* Printor);

//drawing
void draw_sprite(int spritenumber, C2D_SpriteSheet sheet, float x, float y);