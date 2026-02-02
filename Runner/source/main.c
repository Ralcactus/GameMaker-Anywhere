#include <3ds.h>
#include <citro2d.h>
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

float cam_x = 0;
float cam_y = 0;
float cam_w = 400;
float cam_h = 240;
u32 g_keysDown = 0;
u32 g_keysHeld = 0;
u32 g_keysUp = 0;

C2D_SpriteSheet spriteSheet;
Sprite sprites[MAX_SPRITES];
size_t SpriteCount = 0;
const char* CurrentRoom = "";

int  sprite_object_id[MAX_SPRITES];
static bool sprite_is_object[MAX_SPRITES];

#pragma region //Return values from the data.win
//get the sprite via its t3s slot
static int GetSpriteNumberByName(const cJSON* root, const char* sprite_name)
{
	const cJSON* sprites = cJSON_GetObjectItemCaseSensitive(root, "Sprites");
	const cJSON* all = cJSON_GetObjectItemCaseSensitive(sprites, "all_sprites");
	const cJSON* garry = NULL;
	
	cJSON_ArrayForEach(garry, all)
	{
		const cJSON* name = cJSON_GetObjectItemCaseSensitive(garry, "name");
		if (strcmp(name->valuestring, sprite_name) == 0)
		{
			return (int)cJSON_GetObjectItemCaseSensitive(garry, "SpriteNumber")->valuedouble;
		}
	}

	//stop compiler complaining
	return -1;
}

//return the room set to be the first loaded
static cJSON* GetFirstRoomName(const char* json_text)
{
    cJSON* root = cJSON_Parse(json_text);
    cJSON* Firstroom_name = cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(root, "FirstRoom"), "name");
    cJSON* output = cJSON_CreateString(Firstroom_name->valuestring);
    cJSON_Delete(root);
	
    return output;
}

#pragma endregion

#pragma region //Get room info (bg, width, height, ect)
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

//return the current rooms size
static float GetCurrentRoomSize(const char* json_text, const char* mode)
{
	float output = 0;

	
	cJSON* root = cJSON_Parse(json_text);
	cJSON* all_rooms = cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(root, "Rooms"), "all_rooms");
	cJSON* room = NULL;

	cJSON_ArrayForEach(room, all_rooms)
	{
		cJSON* roomname = cJSON_GetObjectItemCaseSensitive(room, "name");
		
		if (!roomname || strcmp(roomname->valuestring, CurrentRoom) != 0)
			continue;

		if (strcmp(mode, "height") == 0 || strcmp(mode, "Height") == 0)
			output = (float)cJSON_GetObjectItemCaseSensitive(room, "height")->valuedouble;

		if (strcmp(mode, "width") == 0 || strcmp(mode, "Width") == 0)
			output = (float)cJSON_GetObjectItemCaseSensitive(room, "width")->valuedouble;

		break;
	}

	cJSON_Delete(root);
	return output;
}

#pragma endregion

#pragma region //Create resources

//create the assets for the room
static void CreateCurrentRoomAssets(const char* json_text)
{
	//parse data.win/json
	cJSON* root = cJSON_Parse(json_text);
	cJSON* rooms = cJSON_GetObjectItemCaseSensitive(root, "Rooms");
	cJSON* all_rooms = cJSON_GetObjectItemCaseSensitive(rooms, "all_rooms");
	cJSON* room = NULL;
	cJSON* layer = NULL;
	cJSON* asset = NULL;
	
	cJSON_ArrayForEach(room, all_rooms)
	{
		cJSON* layers = cJSON_GetObjectItemCaseSensitive(room, "layers");
		cJSON_ArrayForEach(layer, layers)
		{
			cJSON* assets = cJSON_GetObjectItemCaseSensitive(layer, "assets");
			cJSON_ArrayForEach(asset, assets)
			{
				//stop if the max sprite limit is reached
				if (SpriteCount >= MAX_SPRITES)
					break;

				//Get sprite info
				cJSON* spr = cJSON_GetObjectItemCaseSensitive(asset, "sprite");
				cJSON* x = cJSON_GetObjectItemCaseSensitive(asset, "x");
				cJSON* y = cJSON_GetObjectItemCaseSensitive(asset, "y");
				cJSON* scale_x = cJSON_GetObjectItemCaseSensitive(asset, "scaleX");
				cJSON* scale_y = cJSON_GetObjectItemCaseSensitive(asset, "scaleY");
				cJSON* rot = cJSON_GetObjectItemCaseSensitive(asset, "rotation");
				float float_x = (float)x->valuedouble;
				float float_y = (float)y->valuedouble;
				float float_scalex = (float)scale_x->valuedouble;
				float float_scaley = (float)scale_y->valuedouble;
				float float_rot = (float)rot->valuedouble * (float)M_PI / -180;

				//draw the sprite
				Sprite* sp = &sprites[SpriteCount];
				C2D_SpriteFromSheet(&sp->spr, spriteSheet, GetSpriteNumberByName(root, spr->valuestring));
				C2D_SpriteSetPos(&sp->spr, float_x, float_y);
				C2D_SpriteSetScale(&sp->spr, float_scalex, float_scaley);
				C2D_SpriteSetRotation(&sp->spr, float_rot);

				//set these to not have an object id
				sprite_is_object[SpriteCount] = false;
				sprite_object_id[SpriteCount] = 0;

				//sprite count increase
				SpriteCount++;
			}
		}
	}
	cJSON_Delete(root);
}

bool runner_sprite_is_object(int index)
{
	if (index < 0 || index >= (int)SpriteCount) 
		return false;

	return sprite_is_object[index];
}

//create the objects for the room
static void CreateCurrentRoomObjects(const char* json_text)
{
	//parse data.win/json
	cJSON* root = cJSON_Parse(json_text);
	cJSON* all_rooms = cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(root, "Rooms"), "all_rooms");
	cJSON* room = NULL;
	cJSON* layer = NULL;
	cJSON* object = NULL;

	cJSON_ArrayForEach(room, all_rooms)
	{
		if (strcmp(cJSON_GetObjectItemCaseSensitive(room, "name")->valuestring, CurrentRoom) != 0)
			continue;

		cJSON* layers = cJSON_GetObjectItemCaseSensitive(room, "layers");
		cJSON_ArrayForEach(layer, layers)
		{
			cJSON* instances = cJSON_GetObjectItemCaseSensitive(layer, "instances");
			if (cJSON_IsArray(instances))
			{
				cJSON_ArrayForEach(object, instances)
				{
					//stop if the max sprite limit is reached
					if (SpriteCount >= MAX_SPRITES)
						break;

					//get object sprite name
					const char* spriteName = NULL;
					int defIndex = -1;

					cJSON* instObjName = cJSON_GetObjectItemCaseSensitive(object, "object");

					cJSON* objs = cJSON_GetObjectItemCaseSensitive(root, "Objects");
					cJSON* all_objs = cJSON_GetObjectItemCaseSensitive(objs, "all_objects");

					int idx = 0;
					cJSON* odef = NULL;

					cJSON_ArrayForEach(odef, all_objs)
					{
						cJSON* oname = cJSON_GetObjectItemCaseSensitive(odef, "name");
						if (!oname || !cJSON_IsString(oname)) { idx++; continue; }

						if (strcmp(oname->valuestring, instObjName->valuestring) == 0)
						{
							cJSON* os = cJSON_GetObjectItemCaseSensitive(odef, "sprite");
							if (os && cJSON_IsString(os))
								spriteName = os->valuestring;

							defIndex = idx;
							break;
						}
						idx++;
					}

					if (!spriteName || defIndex < 0)
						continue;



					cJSON* x = cJSON_GetObjectItemCaseSensitive(object, "x");
					cJSON* y = cJSON_GetObjectItemCaseSensitive(object, "y");
					cJSON* scale_x = cJSON_GetObjectItemCaseSensitive(object, "scaleX");
					cJSON* scale_y = cJSON_GetObjectItemCaseSensitive(object, "scaleY");
					cJSON* rot = cJSON_GetObjectItemCaseSensitive(object, "rotation");
					float float_x = (float)x->valuedouble;
					float float_y = (float)y->valuedouble;
					float float_scalex = (float)scale_x->valuedouble;
					float float_scaley = (float)scale_y->valuedouble;
					float float_rot = (float)rot->valuedouble * (float)M_PI / -180;

					Sprite* sp = &sprites[SpriteCount];
					C2D_SpriteFromSheet(&sp->spr, spriteSheet, GetSpriteNumberByName(root, spriteName));
					C2D_SpriteSetPos(&sp->spr, float_x, float_y);
					C2D_SpriteSetScale(&sp->spr, float_scalex, float_scaley);
					C2D_SpriteSetRotation(&sp->spr, float_rot);

					//mark as an object and give an id
					sprite_is_object[SpriteCount] = true;
					sprite_object_id[SpriteCount] = defIndex;

					//sprite count increase
					SpriteCount++;
				}
			}
		}
	}
	cJSON_Delete(root);
}

#pragma endregion

//Init the current room (create assets, objects, run creation code, ect)
static void InitCurrentRoom(const char* json_text)
{
	CreateCurrentRoomAssets(json_text);
	CreateCurrentRoomObjects(json_text);
}



int main()
{
	#pragma region //init stuff
	// Init libs
	gfxInitDefault();
	romfsInit();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);
	printf("\x1b[45m");

	//load the data.win
	FILE* datawin = fopen("romfs:/data.win", "rb");
	fseek(datawin, 0, SEEK_END);
	long size = ftell(datawin);
	fseek(datawin, 0, SEEK_SET);
	char* data_json = (char*)malloc((size_t)size + 1);
	fread(data_json, 1, (size_t)size, datawin);
	data_json[size] = '\0';
	fclose(datawin);

	//set the current room
	cJSON* first = GetFirstRoomName(data_json);
	CurrentRoom = strdup(first->valuestring);
	cJSON_Delete(first);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	//load the first room
	InitCurrentRoom(data_json);
	//set room size
	cam_w = GetCurrentRoomSize(data_json, "width");
	cam_h = GetCurrentRoomSize(data_json, "height");
		
	cJSON* root = cJSON_Parse(data_json);
	GML_SetRoot(root);

	#pragma endregion

	#pragma region //debug stuff
	//debug draw:
	//bees!!!!?!??!!?!!?!
	printf("\x1b[5;10HHope you like bees!\n");
	printf("room name=%s\n", GetFirstRoomName(data_json)->valuestring);
	printf("room width=%.1f\n", GetCurrentRoomSize(data_json, "width"));
	printf("room height=%.1f\n", GetCurrentRoomSize(data_json, "height"));

	//check current platform
	if (is_running3DS())
		printf("Running on 3DS!!!!\n");

	if (is_runningWiiU())
		printf("Running on Wii U!!!!!!\n");

	//sily cat :D
	//draw_sprite(0, spriteSheet, 50, 50);
	#pragma endregion




	//"Step" event
	while (aptMainLoop())
	{
		//scan for inputs
		hidScanInput();
		g_keysDown = hidKeysDown();
		g_keysHeld = hidKeysHeld();
		g_keysUp = hidKeysUp();

		//run the gml interpreter
		RunGML();

		//Quit force quit app
		if (gamepad_button_check(gp_start))
			break;

		//render frames
		if (is_running3DS())
			scr_renderframe_3DS(top, cam_x, cam_y, cam_w, cam_h, SpriteCount, data_json, CurrentRoom);
	}



	#pragma region //end app
	//Delete graphics
	C2D_SpriteSheetFree(spriteSheet);

	//Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	free(data_json);
	cJSON_Delete(root);
	return 0;
	#pragma endregion
}
