#include "main.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cJSON.h"
#include <math.h>
#include "gml_functions.h"
#include "gml_runner.h"
#include "gml_runner.h"
#include "shortcut_functions.h"
#include "cross_platform.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>

    //return the current rooms background colour
    u32 GetCurrentRoomBgColor(const char* json_text, const char* room_name)
    {

        u32 out = C2D_Color32f(255.0f, 255.0f, 255.0f, 1.0f);

        cJSON* root = cJSON_Parse(json_text);
        cJSON* rooms = cJSON_GetObjectItem(root, "Rooms");
        cJSON* list = cJSON_GetObjectItem(rooms, "all_rooms");

        cJSON* room = NULL;

        if (list) 
            room = list->child;

        for (; room; room = room->next)
        {
            cJSON* roomname = cJSON_GetObjectItem(room, "name");

            if (strcmp(roomname->valuestring, room_name) != 0) 
                continue;

            cJSON* layers = cJSON_GetObjectItem(room, "layers");

            cJSON* layer = NULL;
            if (layers)
                layer = layers->child;

            for (; layer; layer = layer->next)
            {
                //check if this is a background layer
                cJSON* layertype = cJSON_GetObjectItem(layer, "type");
                if (strcmp(layertype->valuestring, "GMRBackgroundLayer") != 0)
                    continue;

                cJSON* bg = cJSON_GetObjectItem(layer, "background");
                cJSON* color  = cJSON_GetObjectItem(bg, "colour");

                if (color && (color->type & cJSON_Number))
                {
                    u32 gm = (u32)color->valuedouble;
                    out = C2D_Color32((gm) & 0xFF, (gm >>  8) & 0xFF, (gm >> 16) & 0xFF, (gm >> 24) & 0xFF);
                }

                cJSON_Delete(root);
                return out;
            }
            
            break;
        }

        cJSON_Delete(root);
        return out;
    }

    void scr_renderframe(C3D_RenderTarget* topscreen, float camerax, float cameray, float camera_width, float camera_height, size_t currentsprite_count, char* datajson, const char* my_currentroom){
        //Start the frame
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        //top
        C2D_TargetClear(topscreen, GetCurrentRoomBgColor(datajson, my_currentroom));
        C2D_SceneBegin(topscreen);

        //camera
        C2D_ViewReset();
        C2D_ViewTranslate(-camerax, -cameray);
        C2D_ViewScale(400 / camera_width, 240 / camera_height);

        //draw sprites
        for (size_t i = 0; i < currentsprite_count; i++)
            C2D_DrawSprite(&sprites[i].spr);

        //finish view
        C2D_ViewReset();
        //gui drawing goes here...


        //end frame
        C3D_FrameEnd(0);
    }

    void scr_drawroom_assets(size_t currentsprite_count, C2D_SpriteSheet Spritesheet, float sprite_x, float sprite_y, float sprite_scalex, float sprite_scaley, float sprite_rot, cJSON* root, cJSON* spr){
        Sprite* sp = &sprites[currentsprite_count];
        C2D_SpriteFromSheet(&sp->spr, Spritesheet, GetSpriteNumberByName(root, spr->valuestring));
        C2D_SpriteSetPos(&sp->spr, sprite_x, sprite_y);
        C2D_SpriteSetScale(&sp->spr, sprite_scalex, sprite_scaley);
        C2D_SpriteSetRotation(&sp->spr, sprite_rot);
    }
#elif __RAYLIB__
    #include <raylib.h>

    // Safely loads a Texture2D from the sprite section of the gad file by sprite name
    Texture2D SafeLoadSpriteTexture(const cJSON* root, const char* spriteName) {
        if (!root || !spriteName) return (Texture2D){0};
        const cJSON* sprites = cJSON_GetObjectItemCaseSensitive(root, "Sprites");
        if (!sprites) return (Texture2D){0};
        const cJSON* all_sprites = cJSON_GetObjectItemCaseSensitive(sprites, "all_sprites");
        if (!all_sprites) return (Texture2D){0};

        const cJSON* sprite = NULL;
        cJSON_ArrayForEach(sprite, all_sprites) {
            const cJSON* name = cJSON_GetObjectItemCaseSensitive(sprite, "name");
            if (!name || !name->valuestring) continue;
            if (strcmp(name->valuestring, spriteName) == 0) {
                const cJSON* dir = cJSON_GetObjectItemCaseSensitive(sprite, "dir");
                const cJSON* frames = cJSON_GetObjectItemCaseSensitive(sprite, "frames");
                if (!dir || !frames || cJSON_GetArraySize(frames) == 0) return (Texture2D){0};
                const cJSON* frame = cJSON_GetArrayItem(frames, 0);
                if (!frame || !frame->valuestring) return (Texture2D){0};
                char path[512];
                snprintf(path, sizeof(path), "%s%s.png", dir->valuestring, frame->valuestring);
                if (FileExists(path)) {
                    return LoadTexture(path);
                } else {
                    printf("Texture file not found: %s\n", path);
                    return (Texture2D){0};
                }
            }
        }
        return (Texture2D){0};
    }
    Color GetCurrentRoomBgColor(const char* json_text, const char* room_name)
    {
        Color out = WHITE;

        cJSON* root = cJSON_Parse(json_text);
        cJSON* rooms = cJSON_GetObjectItem(root, "Rooms");
        cJSON* list = cJSON_GetObjectItem(rooms, "all_rooms");

        cJSON* room = NULL;

        if (list) 
            room = list->child;

        for (; room; room = room->next)
        {
            cJSON* roomname = cJSON_GetObjectItem(room, "name");

            if (strcmp(roomname->valuestring, room_name) != 0) 
                continue;

            cJSON* layers = cJSON_GetObjectItem(room, "layers");

            cJSON* layer = NULL;
            if (layers)
                layer = layers->child;

            for (; layer; layer = layer->next)
            {
                //check if this is a background layer
                cJSON* layertype = cJSON_GetObjectItem(layer, "type");
                if (strcmp(layertype->valuestring, "GMRBackgroundLayer") != 0)
                    continue;

                cJSON* bg = cJSON_GetObjectItem(layer, "background");
                cJSON* color  = cJSON_GetObjectItem(bg, "colour");

                if (color && (color->type & cJSON_Number))
                {
                    #include <stdint.h>

                    uint32_t gm = (uint32_t)color->valuedouble;

                    out.r = (gm) & 0xFF;
                    out.g = (gm >> 8) & 0xFF;
                    out.b = (gm >> 16) & 0xFF;
                    out.a = (gm >> 24) & 0xFF;
                }

                cJSON_Delete(root);
                return out;
            }
            
            break;
        }

        cJSON_Delete(root);
        return out;
    }

    void scr_renderframe(RenderTexture2D target, float camerax, float cameray, float camera_width, float camera_height, size_t currentsprite_count, char* datajson, const char* my_currentroom){
        BeginTextureMode(target);
        ClearBackground(GetCurrentRoomBgColor(datajson, my_currentroom));

        //camera
        Camera2D cam = {0};
        cam.target = (Vector2){camerax, cameray};
        cam.offset = (Vector2){0, 0};
        cam.rotation = 0.0f;
        cam.zoom = 1.0f;

        BeginMode2D(cam);

        //draw sprites
        for (size_t i = 0; i < currentsprite_count; i++)
        {
            Texture2D tex = sprites[i].texture;

            DrawTexturePro(
                tex,
                (Rectangle){0, 0, (float)tex.width, (float)tex.height},
                (Rectangle){
                    sprites[i].x,
                    sprites[i].y,
                    tex.width * sprites[i].scale_x,
                    tex.height * sprites[i].scale_y
                },
                (Vector2){0, 0},
                -sprites[i].rotation,
                WHITE
            );
        }
        EndMode2D();
        EndTextureMode();
    }

    void scr_drawroom_assets(size_t currentsprite_count,
                            const cJSON* root,
                            cJSON* spr,
                            float sprite_x,
                            float sprite_y,
                            float sprite_scalex,
                            float sprite_scaley,
                            float sprite_rot) {
        Sprite* sp = &sprites[currentsprite_count];
        // Load the texture for this sprite using the sprite name from spr
        if (spr && spr->valuestring) {
            sp->texture = SafeLoadSpriteTexture(root, spr->valuestring);
        } else {
            sp->texture = (Texture2D){0};
        }
        sp->x = sprite_x;
        sp->y = sprite_y;
        sp->scale_x = sprite_scalex;
        sp->scale_y = sprite_scaley;
        sp->rotation = sprite_rot;
    }

#endif