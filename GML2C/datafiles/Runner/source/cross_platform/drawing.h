#include <stdio.h>
#include <string.h>

//3DS
#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>
    extern C2D_SpriteSheet spriteSheet;

    //funcs
    void draw_sprite(int sprite_index, int subimg, float draw_x, float draw_y);
    void drawing_init();
    void scr_startframe(C3D_RenderTarget* screen_target);
    void scr_endframe();
#endif

//WIIU
#ifdef __WIIU__
    //funcs
    void ExitApplication();
#endif
