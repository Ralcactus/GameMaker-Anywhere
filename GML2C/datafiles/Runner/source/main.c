#include <stdio.h>
#include <string.h>
#include "cross_platform/input.h"
#include "cross_platform/other.h"
#include "cross_platform/file_manage.h"
#include "cross_platform/drawing.h"
#include "room_handler.h"
char* CurrentRoom = "NULL"; //DO NOT CHANGE THIS! GM CHECKS FOR "NULL" TO EDIT IT!!!

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
    printf("Hold select to see cool picture!\n");
    printf("First room: %s\n", CurrentRoom);

    //main loop
    #ifdef __3DS__
    while (aptMainLoop()){
    #endif    
    #ifdef __WIIU__
    while (true){
    #endif
        scr_startframe(top);
        if (gamepad_button_check_pressed(0, gp_start))
            break;

        if (gamepad_button_check(0, gp_select))
            draw_sprite(0, 0, 0, 0);
        scr_endframe();

        //do the room stuff
        scr_handleroom(CurrentRoom);
    }

    ExitApplication();
    return 0;
}

