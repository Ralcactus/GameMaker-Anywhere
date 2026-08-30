#include <iostream>
#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <sys/stat.h>
#include <vector>
#include <string>
#include <fstream>
#include "../../helpers/renderer.hpp"
#include "../../helpers/meta.hpp"
#include "../compiler_main.hpp"
#include <json/json.h>
#include <cstring>

char SpriteName[256] = "";

//Write to a sprite list (EG: SCF, T3S)
void WriteSpriteList(std::string frame_name, int i){
    //T3S sprite list
    if (strcmp(ExportMode, "3DSX") == 0 || strcmp(ExportMode, "CIA") == 0){
        char SpriteFile[256];
        snprintf(SpriteFile, sizeof(SpriteFile), "%s.png\n", frame_name.c_str());
        File_WriteEnd("C:/GamemakerAnywhere/Runtime/gfx/sprites.t3s", SpriteFile);
    }

    //SCF sprite list
    if (strcmp(ExportMode, "GAMECUBE") == 0 || strcmp(ExportMode, "WII") == 0){
        char TextureLine[512];
        snprintf(TextureLine, sizeof(TextureLine), "<filepath=%s.png id=\"%sFSDSDFFDGIOJHDFIOHEFAMILYGUY2DDD%d%d\" colfmt=6 />\n", frame_name.c_str(), SpriteName, rand() % 100, i); //for the sprite name, were just gonna use the custom defines so i just made it strange
        File_WriteEnd("C:/GamemakerAnywhere/Runtime/gfx/textures.scf", TextureLine);
    }
}

void CopyImageFile(std::string frame_name){
    char ProjectDir[256];
    char ImagePath[256];
    char CopyCommand[512];

    snprintf(ProjectDir, sizeof(ProjectDir), "%s", ProjectYYP);
    *strrchr(ProjectDir, '\\') = '\0';
    snprintf(ImagePath, sizeof(ImagePath), "%s/sprites/%s/%s.png", ProjectDir, SpriteName, frame_name.c_str());
    snprintf(CopyCommand, sizeof(CopyCommand), "powershell -Command \"Copy-Item -Path '%s' -Destination 'C:/GamemakerAnywhere/Runtime/gfx'\"", ImagePath);
    system(CopyCommand);
}

void scr_compilesprites(Json::Value yyfile){
    printf("Compiling Sprite...\n");

    //Init
    #pragma region 
    //Print sprite name
    snprintf(SpriteName, sizeof(SpriteName), "%s", yyfile["name"].asString().c_str());
    printf("Sprite Name: %s\n", SpriteName);

    for (int i = 0; i < ARRAYSIZE(yyfile["frames"]); i++) {
        std::string frame_name = yyfile["frames"][i]["name"].asString();
		currentsprite_count++;
        
        CopyImageFile(frame_name);
        
		printf("Saving sprite info...\n");
		//scr_savesprite_info(frame_name);
		printf("Saved!\n");
		
        //Write to a sprite list (EG: SCF, T3S)
        WriteSpriteList(frame_name, i);
    }
	currentsprite_count--;
	
	const char* spriteto_idh = "C:/GamemakerAnywhere/Runtime/source/helpers/asset_toid.h";
    char SpriteDef[256];
    snprintf(SpriteDef, sizeof(SpriteDef), "#define %s %i //Sprite\n", SpriteName, currentsprite_count);
	File_WriteEnd(spriteto_idh, SpriteDef);
	
    currentsprite_count++;
}
