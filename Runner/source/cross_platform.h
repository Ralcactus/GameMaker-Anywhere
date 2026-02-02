#include <stdbool.h>

#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>
#endif

void scr_renderframe_3DS(C3D_RenderTarget* topscreen, float camerax, float cameray, float camera_width, float camera_height, size_t currentsprite_count, char* datajson, char* my_currentroom);
