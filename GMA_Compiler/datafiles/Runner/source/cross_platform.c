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
                    u32 gm = (u32)color->valuedouble;
                    out.r = (gm) & 0xFF;
                    out.g = (gm >>  8) & 0xFF;
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
        Vector2 camoffset = {camerax - camera_width / 2, cameray - camera_height / 2};
        BeginMode2D((Camera2D){camoffset, (Vector2){0, 0}, 0.0f, 400 / camera_width});

        //draw sprites
        for (size_t i = 0; i < currentsprite_count; i++)
            DrawTextureRec(sprites[i].texture, (Rectangle){0, 0, (float)sprites[i].texture.width, (float)sprites[i].texture.height}, (Vector2){sprites[i].texture.width / 2.0f, sprites[i].texture.height / 2.0f}, WHITE);

        EndMode2D();
        EndTextureMode();
    }

    void scr_drawroom_assets(size_t currentsprite_count, Texture2D Spritesheet, float sprite_x, float sprite_y, float sprite_scalex, float sprite_scaley, float sprite_rot, cJSON* root, cJSON* spr){
        sprites[currentsprite_count].texture = Spritesheet;
    }
#endif