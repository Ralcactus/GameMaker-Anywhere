#include <stdio.h>
#include <string.h>
#include <math.h>
#include "gm_funcs/input.h"
#include "helpers/other.h"
#include "helpers/file_manage.h"
#include "gm_funcs/drawing.h"
#include "gm_funcs/misc.h"
#include "room_handler.h"
#include "helpers/asset_toid.h"
#include "init_sprites.h"
#include "gm_funcs/audio.h"

#ifdef __wii__
    #include <gccore.h>
    #include <wiiuse/wpad.h>
#endif

#ifdef __3DS__
    #include <opusfile.h>
    #include <3ds.h>
#endif

#if defined(__gamecube__) || defined(__wii__)
    #include <ogc/lwp_watchdog.h>
#endif

unsigned int bgcolor = 0xFF000000;
int view_camera[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
bool MarkedForClose = false;

int interpolate_pixels = 0;
int Interpolate_pixels_enabled = 0;

float fps = 0;

#ifdef __3DS__
    u64 lastTick = 0;
#endif

#if defined(__gamecube__) || defined(__wii__)
    u64 lastTick = 0;
#endif

//room
int room = -1; //DO NOT CHANGE THIS! GM CHECKS FOR "-1" TO EDIT IT TO THE FIRST ROOM!!!
int room_first = -1; //asigned in main func
float room_width = 0;
float room_height = 0;

int main(){
    //variable init
    room_first = room;

    #ifdef __3DS__
        gfxInitDefault();
        C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
        C2D_Prepare();
        romfsInit();
        ndspInit();
        audioInit();
        LightEvent_Init(&s_event, RESET_ONESHOT);
        consoleInit(GFX_BOTTOM, NULL);
        lastTick = svcGetSystemTick();
        C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    #endif

    #ifdef __gamecube__
        PAD_Init();
        lastTick = gettick();
    #endif

    #ifdef __wii__
        WPAD_Init();
        WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
        lastTick = gettick();
    #endif


    drawing_init();
    
    printf("Hello World!\n");

    //main loop
    while (LoopOS){
        #ifdef __3DS__
            scr_startframe(top);

            //interpolate pixels
            if (spriteSheet != nullptr) {
                if (interpolate_pixels == 1 && Interpolate_pixels_enabled == 0){
                    C2D_Image img = C2D_SpriteSheetGetImage(spriteSheet, 0);
                    C3D_TexSetFilter(img.tex, GPU_LINEAR, GPU_LINEAR);
                    Interpolate_pixels_enabled = 1;
                }
                else if (interpolate_pixels != 1 && Interpolate_pixels_enabled == 1){
                    C2D_Image img = C2D_SpriteSheetGetImage(spriteSheet, 0);
                    C3D_TexSetFilter(img.tex, GPU_NEAREST, GPU_NEAREST);
                    Interpolate_pixels_enabled = 0;
                }
            }

            //get fps
            u64 now = svcGetSystemTick();
            fps = round((float)SYSCLOCK_ARM11 / (float)(now - lastTick));
            lastTick = now;
        #endif

        #if defined(__gamecube__) || defined(__wii__)
            scr_startframe();

            u32 now = gettick();
            fps = round((float)(TB_TIMER_CLOCK * 1000) / (float)(now - lastTick));
            lastTick = now;
        #endif

        #ifdef __wii__
            if (gamepad_button_check(0, WPAD_BUTTON_HOME))
                MarkedForClose = true;
        #endif

        gamepad_scanner();
        scr_handleroom(room);
        scr_endframe();

        if (MarkedForClose)
            break;
    }

    ExitApplication();
    return 0;
}

