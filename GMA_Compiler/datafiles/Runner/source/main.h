#include "cJSON.h"
#include <stdbool.h>

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif


#ifdef __3DS__
    #include <3ds.h>
#endif

#ifdef __RAYLIB__

    #include <raylib.h>
    typedef struct {
        char name[128];
        Texture2D tex;
        bool loaded;
    } SpriteCache;

    Texture2D GetSpriteTexture(const cJSON* root, const char* spriteName);
#endif

extern const char* CurrentRoom;
extern char* data_json;
extern bool EndGame;
extern float cam_x;
extern float cam_y;
extern float cam_w;
extern float cam_h;

int GetSpriteNumberByName(const cJSON* root, const char* sprite_name);
void InitCurrentRoom(const char* json_text);
