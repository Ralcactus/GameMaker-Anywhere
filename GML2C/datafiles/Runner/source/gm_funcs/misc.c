//these don't deserve their own file lol

//camera funcs
#include <stdio.h>
#include <string.h>
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

#pragma region //room
void room_goto(char* room){
    CurrentRoom = room;
    printf("room_goto %s\n", room);
}
#pragma endregion







//3ds
#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>

#endif

//wii u
#ifdef __WIIU__
    //so empty...
#endif