#include <iostream>
#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include "../helpers/renderer.hpp"
#include "../helpers/meta.hpp"

const char* ProjectYYP = "";

//Start the project compilation
void RunCompiler(){
    //SETUP
    //Get the yyp
    COMDLG_FILTERSPEC filters[] = {{ L"GameMaker Project", L"*.yyp" }};
    ProjectYYP = GetFileUI(filters, ARRAYSIZE(filters));
    printf("Project path: %s\n", ProjectYYP);

    printf("Nothing here folks!\n");
}