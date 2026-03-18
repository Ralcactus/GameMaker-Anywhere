#include <stdbool.h>

#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>
    #define screen_w 400.0f;
    #define screen_h 240.0f;

u32 GetCurrentRoomBgColor(const char* json_text, const char* room_name);
void scr_renderframe(C3D_RenderTarget* topscreen, float camerax, float cameray, float camera_width, float camera_height, size_t currentsprite_count, char* datajson, const char* my_currentroom);
void scr_drawroom_assets(size_t currentsprite_count, C2D_SpriteSheet Spritesheet, float sprite_x, float sprite_y, float sprite_scalex, float sprite_scaley, float sprite_rot, cJSON* root, cJSON*  spr);

#elif __RAYLIB__
    #include <raylib.h>
    extern int screen_w;
    extern int screen_h;
    
    Color GetCurrentRoomBgColor(const char* json_text, const char* room_name);
    Texture2D SafeLoadSpriteTexture(const cJSON* root, const char* spriteName);
    void scr_renderframe(RenderTexture2D target, float camerax, float cameray, float camera_width, float camera_height, size_t currentsprite_count, char* datajson, const char* my_currentroom);
    void scr_drawroom_assets(size_t currentsprite_count,
                            const cJSON* root,
                            cJSON* spr,
                            float sprite_x,
                            float sprite_y,
                            float sprite_scalex,
                            float sprite_scaley,
                            float sprite_rot);
#endif