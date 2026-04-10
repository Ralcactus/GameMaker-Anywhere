#include <stdio.h>
#include <string.h>
#include "drawing.h"

//3ds
#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>
    C2D_SpriteSheet spriteSheet;

    void drawing_init(){
        spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    }

    void scr_startframe(C3D_RenderTarget* screen_target){
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(screen_target, C2D_Color32(0, 0, 0, 255));
        C2D_SceneBegin(screen_target);
    }

    void scr_endframe(){
        C3D_FrameEnd(0);
    }

    void draw_sprite(int sprite_index, int subimg, float draw_x, float draw_y){
        C2D_Sprite sprite;
        C2D_SpriteFromSheet(&sprite, spriteSheet, sprite_index);
        C2D_SpriteSetPos(&sprite, draw_x, draw_y);
        C2D_DrawSprite(&sprite);
    }
    
#endif

//wii u
#ifdef __WIIU__

    void draw_sprite()
    {
        //so empty...
    }

#endif