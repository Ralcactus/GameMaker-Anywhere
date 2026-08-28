#include <iostream>
#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <sys/stat.h>
#include "../helpers/renderer.hpp"
#include "../helpers/meta.hpp"
using namespace std;

const char* ProjectYYP = "";
char RuntimePath[512];

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



    //Get the yyp
    COMDLG_FILTERSPEC filters[] = {{ L"GameMaker Project", L"*.yyp" }};
    ProjectYYP = GetFileUI(filters, ARRAYSIZE(filters));
    printf("Project path: %s\n", ProjectYYP);   

    if (stat("C:/GamemakerAnywhere", &sb) == 0)
        return true;
    else
        return false;
}

//Start the project compilation
void RunCompiler(){
    if (InitCompiler() == false)
        ShowError("FAILED TO INIT COMPILER!\nCHECK LOG FOR MORE INFO!");

    printf("Nothing here folks!\n");
}

