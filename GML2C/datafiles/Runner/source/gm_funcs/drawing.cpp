#include <stdio.h>
#include <string.h>
#include "drawing.h"
#include "misc.h"
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string>
#include "../helpers/get_spriteinfo.h"
#include "../helpers/var_in_object_running.h"
#include "../helpers/get_spriteinfo.h"
#include "../gm_funcs/misc.h"

unsigned int drawcolor = c_white;

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
        float scaleX = 400.0f / view0_camWidth;
        float scaleY = 240.0f / view0_camHeight;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;

        C2D_ViewScale(scale, scale);
        C2D_ViewTranslate(-view0_camXPos + (400.0f/scale - view0_camWidth) / 2.0f, -view0_camYPos + (240.0f/scale - view0_camHeight) / 2.0f);
    }

    void scr_endframe(){
        C2D_ViewReset();
        //Gui drawing would go here

        C3D_FrameEnd(0);
    }

    //subimg, color and alpha are stubs!
    //Also rotation is a little weird idk how to fix it
    void draw_sprite(int draw_sprite, float subimg, float draw_x, float draw_y){
        draw_sprite_ext(draw_sprite, subimg, draw_x, draw_y, 1, 1, 0, c_white, 1);
    }
    
    void draw_sprite_ext(int draw_sprite, float subimg, float draw_x, float draw_y, float scale_x, float scale_y, float rotation, float color, float alpha){
        if (draw_sprite == -1)
            return;

        C2D_Sprite sprite;
        C2D_SpriteSetCenter(&sprite, sprite_get_xoffset(draw_sprite)/(float)sprite_get_width(draw_sprite), sprite_get_yoffset(draw_sprite)/(float)sprite_get_height(draw_sprite));
        C2D_SpriteFromSheet(&sprite, spriteSheet, draw_sprite-round(subimg));
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

#endif

//gamecube and wii
#if defined(__gamecube__) || defined(__wii__)
    #include <gccore.h>
    #include <ogc/tpl.h>
    #include "textures_tpl.h"
    #include "textures.h"

    #define DEFAULT_FIFO_SIZE	(256*1024)
    static void *frameBuffer[2] = { NULL, NULL};
    static GXRModeObj *rmode;
    GXTexObj texObj;
    TPLFile spriteTPL;
    static u32 fb = 0;

    void drawing_init(){
        f32 yscale;
        u32 xfbHeight;
        Mtx44 perspective;
        Mtx GXmodelView2D;
        void *gp_fifo = NULL;

        TPL_OpenTPLFromMemory(&spriteTPL, (void *)textures_tpl, textures_tpl_size);
        VIDEO_Init();

        rmode = VIDEO_GetPreferredMode(NULL);

        // allocate 2 framebuffers for double buffering
        frameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
        frameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

        VIDEO_Configure(rmode);
        VIDEO_SetNextFramebuffer(frameBuffer[fb]);
        VIDEO_SetBlack(FALSE);
        VIDEO_Flush();
        VIDEO_WaitVSync();

        if(rmode->viTVMode&VI_NON_INTERLACE)
            VIDEO_WaitVSync();


        // setup the fifo and then init the flipper
        gp_fifo = memalign(32,DEFAULT_FIFO_SIZE);
        memset(gp_fifo,0,DEFAULT_FIFO_SIZE);

        GX_Init(gp_fifo,DEFAULT_FIFO_SIZE);

        // other gx setup
        GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
        yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
        xfbHeight = GX_SetDispCopyYScale(yscale);
        GX_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
        GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
        GX_SetDispCopyDst(rmode->fbWidth,xfbHeight);
        GX_SetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
        GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

        if (rmode->aa)
            GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
        else{
            GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
        }

        GX_SetCullMode(GX_CULL_NONE);
        GX_CopyDisp(frameBuffer[fb],GX_TRUE);
        GX_SetDispCopyGamma(GX_GM_1_0);

        // setup the vertex descriptor
        // tells the flipper to expect direct data
        GX_ClearVtxDesc();
        GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
        GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

        GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
        GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

        GX_SetNumChans(1);
        GX_SetNumTexGens(1);
        GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
        GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GX_InvalidateTexAll();

        guOrtho(perspective,0,479,0,639,0,300);
        GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);

        GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
        guMtxIdentity(GXmodelView2D);
        guMtxTransApply (GXmodelView2D, GXmodelView2D, 0.0F, 0.0F, -5.0F);
        GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);

        GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GX_SetAlphaUpdate(GX_TRUE);
        GX_SetColorUpdate(GX_TRUE);
    }

    void scr_startframe(){
        //bg colour
        GXColor background = {(bgcolor >>  8) & 0xFF, (bgcolor >> 16) & 0xFF, (bgcolor >> 24) & 0xFF};
        GX_SetCopyClear(background, GX_MAX_Z24);

        Mtx44 perspective;               //x pos                                             //y pos                                            //i don't know
        guOrtho(perspective,             0+view0_camYPos,view0_camHeight+view0_camYPos,      0+view0_camXPos,view0_camWidth+view0_camXPos,       0,300);
        GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);

    }

    void scr_endframe(){
        GX_CopyDisp(frameBuffer[fb], GX_TRUE);
        VIDEO_SetNextFramebuffer(frameBuffer[fb]);
        VIDEO_Flush();
        VIDEO_WaitVSync();
        fb ^= 1;
    }

    void draw_sprite(int draw_sprite, float subimg, float draw_x, float draw_y){
        draw_sprite_ext(draw_sprite, subimg, draw_x, draw_y, 1, 1, 0, c_white, 1);
    }
    
    void draw_sprite_ext(int draw_sprite, float subimg, float draw_x, float draw_y, float scale_x, float scale_y, float rotation, float color, float alpha){ 
        GXTexObj localTex;
        if (TPL_GetTexture(&spriteTPL, draw_sprite-round(subimg), &localTex) != 0)
            return;

        GX_LoadTexObj(&localTex, GX_TEXMAP0);
        GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);

        int realsprite_width = sprite_get_width(draw_sprite)*scale_x;
        int realsprite_height = sprite_get_height(draw_sprite)*scale_y;

        //Draw top Left
        GX_Position2f32(draw_x-sprite_get_xoffset(draw_sprite)*scale_x, draw_y-sprite_get_yoffset(draw_sprite)*scale_y);
        GX_TexCoord2f32(0, 0);

        //Draw top Right
        GX_Position2f32(draw_x+realsprite_width-sprite_get_xoffset(draw_sprite)*scale_x, draw_y-sprite_get_yoffset(draw_sprite)*scale_y);
        GX_TexCoord2f32(1, 0);

        //Draw bottom Left
        GX_Position2f32(draw_x-sprite_get_xoffset(draw_sprite)*scale_x, draw_y+realsprite_height-sprite_get_yoffset(draw_sprite)*scale_y);
        GX_TexCoord2f32(0, 1);

        //Draw bottom Right
        GX_Position2f32(draw_x+realsprite_width-sprite_get_xoffset(draw_sprite)*scale_x, draw_y+realsprite_height-sprite_get_yoffset(draw_sprite)*scale_y); 
        GX_TexCoord2f32(1, 1);

        GX_End();
    }

    void draw_text(float _x, float _y, const char* text){
        //So empty...
    }
#endif

#pragma region //global funcs
void draw_set_color(int color){
    drawcolor = color;
}

void draw_set_colour(int color){
    draw_set_color(color);
}


int sprite_get_width(int draw_sprite){
    return SpriteWidths[draw_sprite];
}
int sprite_get_height(int draw_sprite){
    return SpriteHeights[draw_sprite];
}

int sprite_get_xoffset(int draw_sprite){
    return SpriteOriginX[draw_sprite];
}
int sprite_get_yoffset(int draw_sprite){
    return SpriteOriginY[draw_sprite];
}

void draw_self(){
    draw_sprite_ext(sprite_index, image_index, x, y, image_xscale, image_yscale, image_angle, image_blend, image_alpha);
}

//tex filtering
void gpu_set_texfilter(bool yesno){
    interpolate_pixels = yesno;
}
void gpu_set_tex_filter(bool yesno){
    gpu_set_texfilter(yesno);
}
bool gpu_get_texfilter(){
    return interpolate_pixels;
}
bool gpu_get_tex_filter(){
    return gpu_get_texfilter();
}

int sprite_get_number(int sprite){
    return SpriteFrameCount[sprite];
}

#pragma endregion
