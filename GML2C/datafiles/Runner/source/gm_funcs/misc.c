//these don't deserve their own file lol

//camera funcs
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "misc.h"

float view0_camXPos = 0;
float view0_camYPos = 0;
float view0_camWidth = 1366;
float view0_camHeight = 768;

#pragma region //camera funcs
void camera_set_view_pos(int viewID, float _x, float _y){
    view0_camXPos = _x;
    view0_camYPos = _y;
}

float camera_get_view_width(int viewID){
    return view0_camWidth;
}

float camera_get_view_height(int viewID){
    return view0_camHeight;
}

float camera_get_view_x(int viewID){
    return view0_camXPos;
}

float camera_get_view_y(int viewID){
    return view0_camYPos;
}

#pragma endregion

#pragma region //window stuff
void window_set_caption(char* caption){
    //No
}
int window_device(){
    //No
    return 0;
}
int window_handle(char* caption){
    //No
    return 0;
}
bool window_has_focus(){
    //No
    return true;
}
void window_post_message(){
    //No
}
void window_center(){
    //No
}
bool window_get_fullscreen(){
    //No
    return true;
}
void window_set_fullscreen(bool fullscreen){
    //No
}
#pragma endregion

#pragma region //rooms
void room_goto(int room_id){
    room = room_id;
}
void room_goto_next(){
    room += 1;
}
void room_goto_previous(){
    room -= 1;
}

int room_next(int room_id){
    return room_id + 1;
}

int room_previous(int room_id){
    return room_id - 1;
}

void room_set_height(int room_id, float height){
    room_height = height;
}
void room_set_width(int room_id, float width){
    room_width = width;
}

#pragma endregion

#pragma region //game_ funcs
void game_end(){
    MarkedForClose = true;
}
void game_restart(){
    //so empty...
}
#pragma endregion


#pragma region //these don't even deserve their own region lmao
bool code_is_compiled(){
    return true;
}

float display_get_width(){
    return 400;
}
float display_get_height(){
    return 240;
}

char* string(float value){
    static char str[64];
    snprintf(str, sizeof(str), "%f", value);
    return str;
}

#pragma endregion

//3ds
#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>

    void show_message(char* message){
        errorConf error;
        errorInit(&error, ERROR_TEXT, CFG_LANGUAGE_EN);
        errorText(&error, message);
        errorDisp(&error);
    }
#endif

//wii u
#ifdef __WIIU__
    //so empty...
#endif