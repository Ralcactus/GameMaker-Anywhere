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
int view_camera[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
bool MarkedForClose = false;

float fps = 0;

#ifdef __3DS__
    u64 lastTick = 0;
#endif

//room
int room = -1; //DO NOT CHANGE THIS! GM CHECKS FOR "-1" TO EDIT IT TO THE FIRST ROOM!!!
int room_first = -1; //asigned in main func
float room_width = 0;
float room_height = 0;

int main(){
    #ifdef __3DS__
        gfxInitDefault();
        C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
        C2D_Prepare();
        romfsInit();
        consoleInit(GFX_BOTTOM, NULL);
        lastTick = svcGetSystemTick();
        C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    #endif
    drawing_init();

    //variable init
    room_first = room;
    
    printf("Hello World!\n");

    //main loop
    #ifdef __3DS__
    while (aptMainLoop()){
    #endif    
    #ifdef __WIIU__
    while (true){
    #endif
        scr_startframe(top);

        #ifdef __3DS__
            u64 now = svcGetSystemTick();
            fps = (float)SYSCLOCK_ARM11 / (float)(now - lastTick);
            lastTick = now;
        #endif

        gamepad_scanner();
        scr_handleroom(room);

        scr_endframe();

        if(MarkedForClose)
            break;
    }

    ExitApplication();
    return 0;
}

