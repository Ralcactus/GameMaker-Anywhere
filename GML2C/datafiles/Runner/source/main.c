#include <stdio.h>
#include <string.h>
#include "gm_funcs/input.h"
#include "cross_platform/other.h"
#include "cross_platform/file_manage.h"
#include "gm_funcs/drawing.h"
#include "gm_funcs/misc.h"
#include "room_handler.h"
#include "sprite_toid.h"

int bgcolor = 0xFF000000;
float room_width = 0;
float room_height = 0;
char* CurrentRoom = "NULL"; //DO NOT CHANGE THIS! GM CHECKS FOR "NULL" TO EDIT IT TO THE FIRST ROOM!!!
int view_camera[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

int main(){
    #ifdef __3DS__
        gfxInitDefault();
        C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
        C2D_Prepare();
        romfsInit();
        consoleInit(GFX_BOTTOM, NULL);

        C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    #endif
    drawing_init();

    
    printf("Hello World!\n");
    printf("Loading First room: %s\n", CurrentRoom);

    //main loop
    #ifdef __3DS__
    while (aptMainLoop()){
    #endif    
    #ifdef __WIIU__
    while (true){
    #endif
        scr_startframe(top);
        
        gamepad_scanner();

        if (gamepad_button_check_pressed(0, gp_start))
            break;

        //do the room stuff
        scr_handleroom(CurrentRoom);

        scr_endframe();
    }

    ExitApplication();
    return 0;
}

