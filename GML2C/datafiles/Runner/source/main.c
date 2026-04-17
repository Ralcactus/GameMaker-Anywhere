#include <stdio.h>
#include <string.h>
#include "gm_funcs/input.h"
#include "cross_platform/other.h"
#include "cross_platform/file_manage.h"
#include "gm_funcs/drawing.h"
#include "gm_funcs/misc.h"
#include "room_handler.h"
#include "sprite_toid.h"
#include "room_toid.h"

int bgcolor = 0xFF000000;
float room_width = 0;
float room_height = 0;
int room = -1; //DO NOT CHANGE THIS! GM CHECKS FOR "-1" TO EDIT IT TO THE FIRST ROOM!!!
int view_camera[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
bool MarkedForClose = false;

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

    //main loop
    #ifdef __3DS__
    while (aptMainLoop()){
    #endif    
    #ifdef __WIIU__
    while (true){
    #endif
        scr_startframe(top);
        
        gamepad_scanner();
        scr_handleroom(room);

        scr_endframe();

        if(MarkedForClose)
            break;
    }

    ExitApplication();
    return 0;
}

