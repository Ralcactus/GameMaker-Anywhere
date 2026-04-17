#include <stdbool.h>

extern float view0_camXPos;
extern float view0_camYPos;
extern float view0_camWidth;
extern float view0_camHeight;
extern int view_camera[9];
extern char* CurrentRoom;
extern float room_width;
extern float room_height;
extern bool MarkedForClose;


#pragma once
#define noone -4

//OS
//the actually useful stuff
#define os_3ds "3DS"
#define os_wiiu "WIIU"

//other
#define os_windows "WINDOWS"
#define os_gxgames "GXGAMES"
#define os_linux "LINUX"
#define os_macosx "MACOSX"
#define os_tvos "TVOS"
#define os_ps4 "PS4"
#define os_ps5 "PS5"
#define os_xboxseriesxs "XBOX"
#define os_gdk "XBOX"
#define os_switch "SWITCH"
#define os_switch2 "SWITCH2"
#define os_unknown "what_are_you_running_on_bro_:sob:"

#ifdef __3DS__
    #define os_type os_3ds
#else
    #define os_type os_unknown
#endif

//camera_ funcs
void camera_set_view_pos(int viewID, float _x, float _y);
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

//game_ funcs
void game_end();
void game_restart();

//room
void room_goto(char* room);

void show_message(char* message);