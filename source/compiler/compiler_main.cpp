#include <iostream>
#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <sys/stat.h>
#include "../helpers/renderer.hpp"
#include "../helpers/meta.hpp"
#include "compiler_main.hpp"
#include "Variables/HandleVariables.hpp"
#include <json/json.h>

//Assets
#include "Asset/RoomCompiler.hpp"
#include "Asset/SpriteCompiler.hpp"

using namespace std;
const char* ProjectYYP = "";
char RuntimePath[512];
const char* ExportMode = "GAMECUBE";
int currentsprite_count = 0;

//Start the project compilation
void RunCompiler(){
    if (InitCompiler() == false)
        ShowError("FAILED TO INIT COMPILER!\nCHECK LOG FOR MORE INFO!");
        
	//Create the t3s texture list
	if (ExportMode == "3DSX" || ExportMode == "CIA"){
        system("powershell -Command \"New-Item -Path 'C:/GamemakerAnywhere/Runtime/gfx/sprites.t3s' -Force\"");
        FILE* T3S = fopen("C:/GamemakerAnywhere/Runtime/gfx/sprites.t3s", "w");
		fprintf(T3S, "--atlas\n");
        fclose(T3S);
	}

    //Create the scf texture list
	if (ExportMode == "GAMECUBE" || ExportMode == "WII"){
        system("powershell -Command \"New-Item -Path 'C:/GamemakerAnywhere/Runtime/gfx/textures.scf' -Force\"");
	}

    //Parse the yyp
    printf("Parsing YYP...\n");
    Json::Value yyp_json = ParseJSON(ProjectYYP);
    printf("Parsed YYP...\n");

    //Print the project name
    printf("Project Name: %s\n", yyp_json["name"].asCString());

    //The asset compile loop!!
    CompileAssets(yyp_json);
}

//Setup compiler (copy runtime, rest vars, etc)
bool InitCompiler(){
    //SETUP
    SDL_snprintf(RuntimePath, sizeof(RuntimePath), "%sRuntime", SDL_GetBasePath());
    struct stat sb;

    //Delete the old build if it exists
    if (stat("C:/GamemakerAnywhere", &sb) == 0){
        printf("Deleting old build...\n");
        system("powershell -Command \"Remove-Item -LiteralPath \"C:/GamemakerAnywhere\" -Recurse -Force -ErrorAction SilentlyContinue\"");
        
        while (stat("C:/GamemakerAnywhere", &sb) == 0){
            printf("Folder still not deleted...");
        }
    }

    //Copy the runtime folder
    printf("\nCopying runtime folder...\n");
    char CopyCommand[256];
    snprintf(CopyCommand, sizeof(CopyCommand), "powershell -Command \"Copy-Item -Path '%s' -Destination 'C:/GamemakerAnywhere' -Recurse\"", RuntimePath);
    printf("%s\n", CopyCommand);
    system("mkdir \"C:/GamemakerAnywhere\"");
    system(CopyCommand);

    //Create other folders
    system("mkdir \"C:/GamemakerAnywhere/Runtime/source/rooms\"");
    system("mkdir \"C:/GamemakerAnywhere/Runtime/source/objects\"");
    system("mkdir \"C:/GamemakerAnywhere/Runtime/source/sprites\"");
    system("mkdir \"C:/GamemakerAnywhere/Runtime/output\"");
    system("mkdir \"C:/GamemakerAnywhere/Runtime/gfx\"");

    //Rest vars
    currentsprite_count = 0;

    //Get the yyp
    COMDLG_FILTERSPEC filters[] = {{ L"GameMaker Project", L"*.yyp" }};
    ProjectYYP = GetFileUI(filters, ARRAYSIZE(filters));
    printf("Project path: %s\n", ProjectYYP);   

    //GMS vars
    VarBuiltIn_Init();
    VarBuiltIn_Write();

    if (stat("C:/GamemakerAnywhere", &sb) == 0)
        return true;
    else
        return false;
}

void scr_compileobjects(Json::Value yyfile){
    printf("Compiling Object...\n");
}
void scr_compilescripts(Json::Value yyfile){
    printf("Compiling Script...\n");
}
void scr_compilesounds(Json::Value yyfile){
    printf("Compiling Sound...\n");
}
void scr_compilefonts(Json::Value yyfile){
    printf("Compiling Font...\n");
}

void CompileAssets(Json::Value yyp_json){
    std::string yypDir = std::string(ProjectYYP);
    yypDir = yypDir.substr(0, yypDir.find_last_of("/\\"));

    for (int i = 0; i < yyp_json["resources"].size(); i++){
        //List Assets
        //printf(yyp_json["resources"][i]["id"]["name"].asCString());
        
        Json::Value _id = yyp_json["resources"][i]["id"];
        Json::Value yyfile = ParseJSON((yypDir + "/" + _id["path"].asString()).c_str());
        std::string type = yyfile["resourceType"].asString();

        if (type == "GMRoom")   scr_compilerooms(yyfile, i, yyp_json); //COMPILE ROOM
        if (type == "GMSprite") scr_compilesprites(yyfile);            //COMPILE SPRITE
        if (type == "GMObject") scr_compileobjects(yyfile);            //COMPILE OBJECTS
        if (type == "GMScript") scr_compilescripts(yyfile);            //COMPILE SCRIPTS
        if (type == "GMSound")  scr_compilesounds(yyfile);             //COMPILE SOUNDS
        if (type == "GMFont")   scr_compilefonts(yyfile);              //COMPILE FONTS
    }
}

