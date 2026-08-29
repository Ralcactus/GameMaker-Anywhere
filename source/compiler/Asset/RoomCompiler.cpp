#include <iostream>
#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
#include <sys/stat.h>
#include "../../helpers/renderer.hpp"
#include "../../helpers/meta.hpp"
#include "../compiler_main.hpp"
#include <json/json.h>

char RoomFilePath_CPP[256] = "";
char RoomFilePath_HPP[256] = "";
char RoomName[256] = "";


//Create the file where the room data will be written to (called in scr_compilerooms)
void CreateRoomFile(const char* RoomName){
    //Create the hpp
    char CppCreate[256];
    snprintf(CppCreate, sizeof(CppCreate), "powershell -Command \"New-Item -Path 'C:/GamemakerAnywhere/Runtime/source/rooms/%s.cpp' -Force\"", RoomName);
    char HppCreate[256];
    snprintf(HppCreate, sizeof(HppCreate), "powershell -Command \"New-Item -Path 'C:/GamemakerAnywhere/Runtime/source/rooms/%s.hpp' -Force\"", RoomName);

    system(CppCreate);
    system(HppCreate);
}

//Assign the room cpp/hpp file paths
void AssignRoomFilePath(){
    snprintf(RoomFilePath_CPP, sizeof(RoomFilePath_CPP), "C:/GamemakerAnywhere/Runtime/source/rooms/%s.cpp", RoomName);
    snprintf(RoomFilePath_HPP, sizeof(RoomFilePath_HPP), "C:/GamemakerAnywhere/Runtime/source/rooms/%s.hpp", RoomName);
}

//Writes the rooms includes
void WriteRoomHeader(){
	File_WriteEnd(RoomFilePath_CPP, "#include \"../gml/structs.h\"\n");
	File_WriteEnd(RoomFilePath_CPP, "#include <stdbool.h>\n");
	File_WriteEnd(RoomFilePath_CPP, "#include <stdio.h>\n");
	File_WriteEnd(RoomFilePath_CPP, "#include <string.h>\n");
	File_WriteEnd(RoomFilePath_CPP, "#include \"../helpers/asset_toid.h\"\n");
	File_WriteEnd(RoomFilePath_CPP, "#include \"../gm_funcs/drawing.h\"\n");
	File_WriteEnd(RoomFilePath_CPP, "#include \"../gm_funcs/misc.h\"\n");
	File_WriteEnd(RoomFilePath_CPP, "#include \"../gm_funcs/audio.h\"\n");
	File_WriteEnd(RoomFilePath_CPP, "#include \"../gm_funcs/filesystem.h\"\n");
	File_WriteEnd(RoomFilePath_CPP, "#include \"../helpers/asset_toid.h\"\n\n");    
}

//Compiles the rooms layers into a struct
void CompileRoomLayers(Json::Value yyfile){
    for (int i = 0; i < ARRAYSIZE(yyfile["layers"]); i++){
        printf("Creating layer %i... Wait this isn't finished??\n", i);
    }
}

void scr_compilerooms(Json::Value yyfile, int i, Json::Value yyp_json){
    printf("\nCompiling Room...\n");

    //Init
    #pragma region 
    //Print room name
    snprintf(RoomName, sizeof(RoomName), "%s", yyfile["name"].asString().c_str());
    printf("Room Name: %s\n", RoomName);

    //Assign the room cpp/hpp file paths
    AssignRoomFilePath();

    //Create the file where the room data will be written to
    CreateRoomFile(RoomName);
    #pragma endregion

    //Compiles the rooms layers into a struct
    CompileRoomLayers(yyfile);

	//Writes the rooms includes
    WriteRoomHeader();
}
