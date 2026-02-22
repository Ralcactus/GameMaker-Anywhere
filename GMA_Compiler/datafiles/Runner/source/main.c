#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>
#endif

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cJSON.h"
#include <math.h>
#include "gml_functions.h"
#include "gml_runner.h"
#include "shortcut_functions.h"
#include "cross_platform.h"
#include <stdbool.h>
#include "main.h"

float cam_x = 0;
float cam_y = 0;
float cam_w = 400;
float cam_h = 240;
bool EndGame = false;

#ifdef __DKPRO__
	u32 g_keysDown = 0;
	u32 g_keysHeld = 0;
	u32 g_keysUp = 0;
#endif

#ifdef __3DS__
	C2D_SpriteSheet spriteSheet;
#elif __RAYLIB__
	Texture2D spriteSheet;
#endif

Sprite sprites[MAX_SPRITES];
size_t SpriteCount = 0;
const char* CurrentRoom = "";
char* data_json = "";

int  sprite_object_id[MAX_SPRITES];
static bool sprite_is_object[MAX_SPRITES];

#pragma region //Return values from the data.win
//get the sprite via its t3s slot
int GetSpriteNumberByName(const cJSON* root, const char* sprite_name)
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
		cJSON* roomname = cJSON_GetObjectItemCaseSensitive(room, "name");
		if (!roomname || strcmp(roomname->valuestring, CurrentRoom) != 0)
			continue;

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
				scr_drawroom_assets(SpriteCount, spriteSheet, float_x, float_y, float_scalex, float_scaley, float_rot, root, spr);

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

					#ifdef __3DS__
						Sprite* sp = &sprites[SpriteCount];
						C2D_SpriteFromSheet(&sp->spr, spriteSheet, GetSpriteNumberByName(root, spriteName));
						C2D_SpriteSetPos(&sp->spr, float_x, float_y);
						C2D_SpriteSetScale(&sp->spr, float_scalex, float_scaley);
						C2D_SpriteSetRotation(&sp->spr, float_rot);
					#elif __RAYLIB__
						Texture2D* tex = &sprites[SpriteCount].texture;
						tex->id = GetSpriteNumberByName(root, spriteName);
						tex->x = float_x;
						tex->y = float_y;
						tex->width = 0; //will be set in the drawing function
						tex->height = 0; //will be set in the drawing function
						//raylib doesn't support rotation or scale in the texture struct, so these will be set in the drawing function as well
					#endif

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

//Clear objects and assets
static void Runner_ClearRoomState(void)
{
    SpriteCount = 0;

    memset(sprites, 0, sizeof(sprites));
    memset(sprite_object_id, 0, sizeof(sprite_object_id));
    memset(sprite_is_object, 0, sizeof(sprite_is_object));
}

//Init the current room (create assets, objects, run creation code, ect)
void InitCurrentRoom(const char* json_text)
{
	Runner_ClearRoomState();
	CreateCurrentRoomAssets(json_text);
	CreateCurrentRoomObjects(json_text);
}



int main()
{
	#pragma region //init stuff
	// Init libs
	//3ds
	#ifdef __3DS__
		C3D_RenderTarget* top = NULL;
	#endif

	bool progCond = true;

	#ifdef __3DS__
		gfxInitDefault();
		romfsInit();
		C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
		C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
		C2D_Prepare();
		consoleInit(GFX_BOTTOM, NULL);

		//load the data.win
		FILE* datawin = fopen("romfs:/data.win", "rb");
		fseek(datawin, 0, SEEK_END);
		long size = ftell(datawin);
		fseek(datawin, 0, SEEK_SET);
		data_json = (char*)malloc((size_t)size + 1);
		fread(data_json, 1, (size_t)size, datawin);
		data_json[size] = '\0';
		fclose(datawin);
		
		// Create screens
		top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
		// Load sprite sheet
		spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	#elif __RAYLIB__
		// Load sprite sheet
		spriteSheet = LoadTexture("assets/sprites.png");

		//load the data.win
		FILE* datawin = fopen("assets/data.win", "rb");
		fseek(datawin, 0, SEEK_END);
		long size = ftell(datawin);
		fseek(datawin, 0, SEEK_SET);
		data_json = (char*)malloc((size_t)size + 1);
		fread(data_json, 1, (size_t)size, datawin);
		data_json[size] = '\0';
		fclose(datawin);
	#endif


	printf("\x1b[45m");
	//set the current room
	cJSON* first = GetFirstRoomName(data_json);
	CurrentRoom = strdup(first->valuestring);
	cJSON_Delete(first);

	//load the first room
	InitCurrentRoom(data_json);
	
	//set room size
	cam_w = GetCurrentRoomSize(data_json, "width");
	cam_h = GetCurrentRoomSize(data_json, "height");
		
	cJSON* root = cJSON_Parse(data_json);
	//carry root to gml_runner
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

	if (is_runningRAY())
		printf("Running on Raylib!!!!!!\n");

	//sily cat :D
	//draw_sprite(0, spriteSheet, 50, 50);
	#pragma endregion

	#ifdef __RAYLIB__
		InitWindow(GetCurrentRoomSize(data_json, "width"), GetCurrentRoomSize(data_json, "height"), "GameMaker Anywhere - Dreamcast");
		SetTargetFPS(60);

		if (!IsAudioDeviceReady()) {
			InitAudioDevice();
		}

		progCond = !WindowShouldClose();
	#endif

	//"Step" event
	while (progCond)
	{
		#ifdef __3DS__

			//scan for inputs
			hidScanInput();
			g_keysDown = hidKeysDown();
			g_keysHeld = hidKeysHeld();
			g_keysUp = hidKeysUp();

		#endif

		#ifdef __RAYLIB__
			BeginDrawing();
			ClearBackground(BLACK);
		#endif
		//run the gml interpreter
		RunGML();

		//Quit quit app
		if (EndGame)
			break;

		//render frames
		#ifdef __3DS__
			scr_renderframe(top, cam_x, cam_y, cam_w, cam_h, SpriteCount, data_json, CurrentRoom);
		#elif __RAYLIB__
			scr_renderframe((RenderTexture2D){ .texture = { .id = 0 }, .depth = { .id = 0 } }, cam_x, cam_y, cam_w, cam_h, SpriteCount, data_json, CurrentRoom);

			EndDrawing();
		#endif
	}

	#pragma region //end app
	//3ds
	#ifdef __3DS__
		C2D_SpriteSheetFree(spriteSheet);
		C2D_Fini();
		C3D_Fini();
		gfxExit();
		romfsExit();
		return 0;
	#endif

	free(data_json);
	cJSON_Delete(root);

	#ifdef __RAYLIB__
		CloseAudioDevice();
		CloseWindow();
	#endif

	#pragma endregion
}
