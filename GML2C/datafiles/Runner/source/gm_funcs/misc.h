extern float view0_camXPos;
extern float view0_camYPos;
extern float view0_camWidth;
extern float view0_camHeight;
extern int view_camera[9];
extern char* CurrentRoom;
extern float room_width;
extern float room_height;

#pragma once
#define os_3ds "3DS"
#define os_wiiu "WIIU"
#define os_windows "WINDOWS"


#ifdef __3DS__
    #define os_type os_3ds
#endif

//camera
void camera_set_view_pos(int viewID, float _x, float _y);
float camera_get_view_width(int viewID);
float camera_get_view_height(int viewID);
float camera_get_view_x(int viewID);
float camera_get_view_y(int viewID);

//room
void room_goto(char* room);