#include <stdio.h>
#include <string.h>

#pragma once
#define c_black 4278190080
#define c_white 4294967295
#define c_red 4294901760
#define c_lime 4278255360
#define c_blue 4278190335
#define c_yellow 4294967040
#define c_aqua 4278255615
#define c_fuchsia 4294902015
#define c_maroon 4286578688
#define c_green 4278222848
#define c_navy 4278190208
#define c_olive 4286611456
#define c_teal 4278222976
#define c_purple 4286578816
#define c_orange 4294944000
#define c_gray 4286611584
#define c_dkgray 4282400832
#define c_ltgray 4290822336
#define c_silver 4290822336

//variables
extern int bgcolor;
extern int drawcolor;


//funcs
//draw stuff
void draw_sprite(int sprite_index, int subimg, float draw_x, float draw_y);
void draw_sprite_ext(int sprite_index, int subimg, float draw_x, float draw_y, float scale_x, float scale_y, float rotation, float color, float alpha);
void drawing_init();
void scr_endframe();
void draw_text(float _x, float _y, const char* text);
void draw_set_colour(int color);
void draw_set_color(int color);
int sprite_get_width(int sprite_index);
int sprite_get_height(int sprite_index);
int sprite_get_xoffset(int sprite_index);
int sprite_get_yoffset(int sprite_index);

//3DS
#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>
    extern C2D_SpriteSheet spriteSheet;
    void scr_startframe(C3D_RenderTarget* screen_target);

#endif

//GAMECUBE
#ifdef __gamecube__
    #include <gccore.h>
    #include <ogc/tpl.h>

    extern TPLFile spriteTPL;
    #define DEFAULT_FIFO_SIZE	(256*1024)

    void scr_startframe();
#endif

//WII
#ifdef __wii__
    #include <gccore.h>
    #include <ogc/tpl.h>

    extern TPLFile spriteTPL;
    #define DEFAULT_FIFO_SIZE	(256*1024)

    void scr_startframe();
#endif
