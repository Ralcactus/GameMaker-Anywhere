#include <stdbool.h>
#include "../variable_handler.h"
#include <variant>
#include <vector>
#include <cmath>
#include <math.h>

extern float view0_camXPos;
extern float view0_camYPos;
extern float view0_camWidth;
extern float view0_camHeight;
extern int view_camera[9];
extern int room;
extern int room_first;
extern float room_width;
extern float room_height;
extern bool MarkedForClose;
extern float fps;
extern int interpolate_pixels;
extern Object* CurrentObjectRunning;

#pragma once
#define noone -4

//enums
enum AudioEffectType{
    Bitcrusher,
    Delay,
    Gain,
    HPF2,
    LPF2,
    Reverb1,
    Tremolo,
    PeakEQ,
    HiShelf,
    LoShelf,
    EQ,
    Compressor
};

enum AudioLFOType{
    InvSawtooth,
    Sawtooth,
    Sine,
    Square,
    Triangle
};

#define display_landscape 0
#define display_portrait 1
#define display_landscape_flipped 2
#define display_portrait_flipped 3

//OS
//custom
#define os_3ds 100
#define os_gamecube 101
#define os_wii 102

//gm built in
#define os_windows 0
#define os_gxgames 24
#define os_linux 6
#define os_macosx 1
#define os_tvos 20
#define os_ps4 14
#define os_ps5 22
#define os_xboxseriesxs 23
#define os_gdk 23
#define os_switch 21
#define os_switch2 25
#define os_unknown -1

#ifdef __3DS__
    #define os_type os_3ds //3ds
#elif defined(__gamecube__)
    #define os_type os_gamecube //gamecube
#elif defined(__wii__)
    #define os_type os_wii //wii
#else
    #define os_type os_unknown //tf are you running?
#endif

#ifdef __3DS__
    #define LoopOS aptMainLoop()
#elif defined(__gamecube__) || defined(__wii__)
    #define LoopOS SYS_MainLoop()
#endif


//camera_ funcs
void camera_set_view_pos(int viewID, float _x, float _y);
void camera_set_view_size(int viewID, float _width, float _height);
float camera_get_view_width(int viewID);
float camera_get_view_height(int viewID);
float camera_get_view_x(int viewID);
float camera_get_view_y(int viewID);

//window_ funcs
void window_set_caption(char* caption);
int window_device();
int window_handle(char* caption);
bool window_has_focus();
void window_post_message();
bool window_get_fullscreen();
void window_set_fullscreen(bool fullscreen);
void window_set_size(int width, int height);

//game_ funcs
void game_end();
void game_restart();

//room
void room_goto(int room_id);
void room_goto_previous();
void room_goto_next();
int room_next(int room_id);
int room_previous(int room_id);
void room_set_height(int room_id, float height);
void room_set_width(int room_id, float width);
void room_restart();

//other
void show_message(const char* message);
bool show_question(const char* message);
bool code_is_compiled();
float display_get_width();
float display_get_height();
int display_get_orientation();
float display_get_frequency();
const char* string(GMvar value);
void show_debug_message(const char* message);
void show_error(const char* message, bool abort);
float clamp(float value, float min, float max);
float sign(float number);
float lengthdir_y(float len, float dir);
float lengthdir_x(float len, float dir);
float gm_lerp(float value, float to_goto, float speed);
#define lerp gm_lerp
