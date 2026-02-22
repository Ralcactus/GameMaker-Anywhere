#include <3ds.h>
#include "cJSON.h"

extern const char* CurrentRoom;
extern char* data_json;
extern bool EndGame;
extern float cam_x;
extern float cam_y;

int GetSpriteNumberByName(const cJSON* root, const char* sprite_name);
void InitCurrentRoom(const char* json_text);
