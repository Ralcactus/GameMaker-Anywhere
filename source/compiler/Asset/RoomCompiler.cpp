#include <iostream>
#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <sys/stat.h>
#include "../../helpers/renderer.hpp"
#include "../../helpers/meta.hpp"
#include "../compiler_main.hpp"
#include <json/json.h>

void scr_compilerooms(Json::Value yyfile, int i, Json::Value yyp_json){
    printf("\nCompiling Room...\n");
    
    //Print room name
    const char* RoomName = yyfile["name"].asString().c_str();
    printf("Room Name: %s\n", RoomName);
}