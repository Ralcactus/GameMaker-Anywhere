#include "cJSON.h"
#include <stddef.h>

extern size_t SpriteCount;
extern int sprite_object_id[MAX_SPRITES];

bool runner_sprite_is_object(int index);
void GML_SetRoot(const cJSON* root);
void RunGML_create(const char* code, int defIndex);
void RunGML_step(const char* code, int defIndex);
void RunGML(void);

