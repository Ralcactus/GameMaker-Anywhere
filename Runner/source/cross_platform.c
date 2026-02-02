#include <stdbool.h>
#include "cross_platform.h"
#include "shortcut_functions.h"
#include "main.h"

#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>
#endif



void scr_renderframe_3DS(C3D_RenderTarget* topscreen, float camerax, float cameray, float camera_width, float camera_height, size_t currentsprite_count, char* datajson, char* my_currentroom){
    //Start the frame
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

    //top
    C2D_TargetClear(topscreen, GetCurrentRoomBgColor(datajson, my_currentroom));
    C2D_SceneBegin(topscreen);

    //camera
    C2D_ViewReset();
    C2D_ViewTranslate(-camerax, -cameray);
    C2D_ViewScale(400 / camera_width, 240 / camera_height);

    //draw sprites
    for (size_t i = 0; i < currentsprite_count; i++)
        C2D_DrawSprite(&sprites[i].spr);

    //finish view
    C2D_ViewReset();
    //gui drawing goes here...


    //end frame
    C3D_FrameEnd(0);
}