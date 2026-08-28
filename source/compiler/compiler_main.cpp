#include <iostream>
#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <sys/stat.h>
#include "../helpers/renderer.hpp"
#include "../helpers/meta.hpp"
#include "compiler_main.hpp"
#include <json/json.h>

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
        system("powershell -Command \"New-Item -Path 'C:\\GamemakerAnywhere\\Runtime\\gfx\\sprites.t3s' -Force\"");
        FILE* T3S = fopen("C:/GamemakerAnywhere/Runtime/gfx/sprites.t3s", "w");
		fprintf(T3S, "--atlas\n");
        fclose(T3S);
	}

    //Create the scf texture list
	if (ExportMode == "GAMECUBE" || ExportMode == "WII"){
        system("powershell -Command \"New-Item -Path 'C:\\GamemakerAnywhere\\Runtime\\gfx\\textures.scf' -Force\"");
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

    if (stat("C:/GamemakerAnywhere", &sb) == 0)
        return true;
    else
        return false;
}

void CompileAssets(Json::Value yyp_json){
    for (int i = 0; i < yyp_json["resources"].size(); i++){
        //List Assets
        //printf(yyp_json["resources"][i]["id"]["name"].asCString());
    }
}