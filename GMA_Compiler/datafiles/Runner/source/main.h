#include "cJSON.h"
#include <stdbool.h>


#ifdef __3DS__
    #include <3ds.h>
#endif

#ifdef __RAYLIB__

    #include <raylib.h>
#include <raylib.h>
Texture2D GetSpriteTexture(const cJSON* root, const char* spriteName);
#endif

extern const char* CurrentRoom;
extern char* data_json;
extern bool EndGame;
extern float cam_x;
extern float cam_y;
extern float cam_w;
extern float cam_h;

extern char* data_json;

int GetSpriteNumberByName(const cJSON* root, const char* sprite_name);
void InitCurrentRoom(const char* json_text);
void InitCurrentRoomObjects(const char* json_text);