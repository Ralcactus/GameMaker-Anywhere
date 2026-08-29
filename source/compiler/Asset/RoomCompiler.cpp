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

char RoomFilePath_CPP[256] = "";
char RoomFilePath_HPP[256] = "";
char RoomName[256] = "";
int roomid_count = 0;

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
    printf("Wrote room file header!\n");
}

//Compiles the rooms layers into a struct
void CompileRoomLayers(Json::Value yyfile){
    for (int i = 0; i < ARRAYSIZE(yyfile["layers"]); i++){
        printf("Creating layer %i... Wait this isn't finished??\n", i);
    }
}

//Writes the rooms actual room function
//For example: void scr_runroom_ROOMNAME()
void WriteRoom_Runner(){
    char FuncLine[256] = "";
    snprintf(FuncLine, sizeof(FuncLine), "void scr_runroom_%s()", RoomName);

    //Begin the func
    File_WriteEnd(RoomFilePath_CPP, FuncLine);
    File_WriteEnd(RoomFilePath_CPP, "{\n");

    //Write header def
    File_WriteEnd(RoomFilePath_HPP, FuncLine);
    File_WriteEnd(RoomFilePath_HPP, ";\n");

    char printf_name[256] = "";
    snprintf(printf_name, sizeof(printf_name), "    printf(\"RUNNING ROOM: %s\\n\");\n", RoomName);
    File_WriteEnd(RoomFilePath_CPP, printf_name);

	File_WriteEnd(RoomFilePath_CPP, "}\n");
}

void AddTo_RoomHandler(){
    //Find closing bracket
    int insert_at = -1;
    const char* RoomHandler = "C:/GamemakerAnywhere/Runtime/source/room_handler.cpp";

    std::ifstream in(RoomHandler);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(in, line)){
        lines.push_back(line);
    }

    in.close();

    for (int j = lines.size() - 1; j >= 0; j--){
        if (lines[j] == "}"){
            insert_at = j+1;
            break;
        }
    }

    //Insert room
    char RoomLine[256];

    //Room Header
    snprintf(RoomLine, sizeof(RoomLine), "#include \"../rooms/%s.hpp\"", RoomName);
    File_WriteFirst(RoomHandler, RoomLine);

    //Check which room is running
    snprintf(RoomLine, sizeof(RoomLine), "    if (room == %s){", RoomName);
    File_WriteLine(RoomHandler, insert_at, RoomLine);

    //The room func
    snprintf(RoomLine, sizeof(RoomLine), "        scr_runroom_%s();", RoomName);
    File_WriteLine(RoomHandler, insert_at + 1, RoomLine);

    //End the room check
    File_WriteLine(RoomHandler, insert_at + 2, "        return;");
    File_WriteLine(RoomHandler, insert_at + 3, "    }");
}

//The actual room compiler
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

    //Writes the rooms actual room function
    //For example: void scr_runroom_ROOMNAME()
    WriteRoom_Runner();

    //Add to the room runner
    AddTo_RoomHandler();

    //Add to the asset ID list
	const char* roomto_idh = "C:/GamemakerAnywhere/Runtime/source/helpers/asset_toid.h";
    char RoomDef[256];
    snprintf(RoomDef, sizeof(RoomDef), "#define %s %i //Room\n", RoomName, roomid_count);
	File_WriteEnd(roomto_idh, RoomDef);

    if (roomid_count == 0){
        char NewStartRoom[256];
        snprintf(NewStartRoom, sizeof(NewStartRoom), "int room = %s;", RoomName);
        File_ReplaceLine("C:/GamemakerAnywhere/Runtime/source/main.cpp", "int room = -1; //DO NOT CHANGE THIS! GM CHECKS FOR \"-1\" TO EDIT IT TO THE FIRST ROOM!!!",NewStartRoom);
    }
	roomid_count += 1;
}

