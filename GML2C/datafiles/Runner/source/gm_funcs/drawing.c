#include <stdio.h>
#include <string.h>
#include "drawing.h"
#include "misc.h"

int drawcolor = c_white;

//3ds
#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>
    C2D_SpriteSheet spriteSheet;

    void drawing_init(){
        spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
        drawcolor = c_white;
    }

    void scr_startframe(C3D_RenderTarget* screen_target){
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        C2D_TargetClear(screen_target, C2D_Color32((bgcolor >>  0) & 0xFF, (bgcolor >>  8) & 0xFF, (bgcolor >> 16) & 0xFF, (bgcolor >> 24) & 0xFF));
        C2D_SceneBegin(screen_target);

        //size and pos
        C2D_ViewScale(400 / view0_camWidth, 240 / view0_camHeight);
        C2D_ViewTranslate(-view0_camXPos, -view0_camYPos);
    }

    void scr_endframe(){
        C2D_ViewReset();
        //Gui drawing would go here

        C3D_FrameEnd(0);
    }

    //subimg, color and alpha are stubs!
    //Also rotation is a little weird idk how to fix it
    void draw_sprite(int sprite_index, int subimg, float draw_x, float draw_y){
        if (sprite_index == -1)
            return;

        C2D_Sprite sprite;
        C2D_SpriteFromSheet(&sprite, spriteSheet, sprite_index);
        C2D_SpriteSetPos(&sprite, draw_x, draw_y);
        C2D_DrawSprite(&sprite);
    }
    
    void draw_sprite_ext(int sprite_index, int subimg, float draw_x, float draw_y, float scale_x, float scale_y, float rotation, float color, float alpha){
        if (sprite_index == -1)
            return;

        C2D_Sprite sprite;
        C2D_SpriteFromSheet(&sprite, spriteSheet, sprite_index);
        C2D_SpriteSetPos(&sprite, draw_x, draw_y);
        C2D_SpriteSetScale(&sprite, scale_x, scale_y);
        C2D_SpriteSetRotation(&sprite, rotation);
        C2D_DrawSprite(&sprite);
    }

    void draw_text(float _x, float _y, const char* text){
        C2D_Text bleh;
        C2D_TextBuf buffer = C2D_TextBufNew(256);

        C2D_TextParse(&bleh, buffer, text);
        C2D_TextOptimize(&bleh);
        C2D_DrawText(&bleh, C2D_WithColor, _x, _y, 0, 1, 1, C2D_Color32((drawcolor >>  0) & 0xFF, (drawcolor >>  8) & 0xFF, (drawcolor >> 16) & 0xFF, (drawcolor >> 24) & 0xFF));
        C2D_TextBufDelete(buffer);
    }


    void draw_set_color(int color){
        drawcolor = color;
    }

    void draw_set_colour(int color){
        draw_set_color(color);
    }

#endif

//wii u
#ifdef __WIIU__
    //so empty...
#endif