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

#pragma region //Structs
struct AssetOut {
    Json::Value type;
    std::string sprite;
    float x, y, scaleX, scaleY, rotation;
};

struct LayerOut {
    Json::Value type;
    int depth;
    std::vector<AssetOut> assets;
};

struct RoomOut {
    std::string name;
    int roomwidth;
    int roomheight;
    bool viewsenabled;
    int viewport0_width;
    int viewport0_height;
    std::vector<LayerOut> layers;
};
#pragma endregion

char RoomFilePath_CPP[256] = "";
char RoomFilePath_HPP[256] = "";
char RoomName[256] = "";
int roomid_count = 0;
std::vector<LayerOut> packed_layers = {};
std::vector<RoomOut> all_rooms;

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

//Write the layers to the room file
void WriteLayers(){
    //Write to the room file layer structs
	for (int k = 0; k < (int)packed_layers.size(); k++) {
		LayerOut& _layer = packed_layers[k];

		if (_layer.type == "GMRAssetLayer") {
		    std::string cname = std::string(RoomName) + "_asset_" + std::to_string(k);

            char AssetLayerStruct[256] = "";
            snprintf(AssetLayerStruct, sizeof(AssetLayerStruct), "static LayerAssets %s_data[] = {\n", cname.c_str());
		    File_WriteEnd(RoomFilePath_CPP, AssetLayerStruct);

                for (int j = 0; j < (int)_layer.assets.size(); j++) {
                    AssetOut& asset = _layer.assets[j];

                    File_WriteEnd(RoomFilePath_CPP, "    { ");
                    File_WriteEnd(RoomFilePath_CPP, (std::to_string(asset.x) + ", ").c_str());
                    File_WriteEnd(RoomFilePath_CPP, (std::to_string(asset.y) + ", ").c_str());
                    File_WriteEnd(RoomFilePath_CPP, (std::to_string(asset.rotation) + ", ").c_str());
                    File_WriteEnd(RoomFilePath_CPP, (std::to_string(asset.scaleX) + ", ").c_str());
                    File_WriteEnd(RoomFilePath_CPP, (std::to_string(asset.scaleY) + ", ").c_str());
                    File_WriteEnd(RoomFilePath_CPP, (asset.sprite + " },\n").c_str());
                }

		    File_WriteEnd(RoomFilePath_CPP, "};\n\n");

		    int count = (int)_layer.assets.size();

		    File_WriteEnd(RoomFilePath_CPP, ("static GMLayerAsset " + cname + " = {\n").c_str());
			File_WriteEnd(RoomFilePath_CPP, (cname + "_data,\n").c_str());
			File_WriteEnd(RoomFilePath_CPP, (std::to_string(count) + "\n").c_str());
			File_WriteEnd(RoomFilePath_CPP, "};\n\n");
		}
	}
}

//Create the struct for a asset layer index
void CreateLayerStruct_ASSET(Json::Value _layer, LayerOut& layer_out){
    if (_layer["resourceType"] == "GMRAssetLayer"){
        for (int j = 0; j < (int)_layer["assets"].size(); j++){
            Json::Value assetlayer = _layer["assets"][j];

            if (assetlayer["resourceType"] == "GMRSpriteGraphic") {
                layer_out.assets.push_back({
                    .type = assetlayer["resourceType"],
                    .sprite = assetlayer["spriteId"]["name"].asString(),
                    .x = assetlayer["x"].asFloat(),
                    .y = assetlayer["y"].asFloat(),
                    .scaleX = assetlayer["scaleX"].asFloat(),
                    .scaleY = assetlayer["scaleY"].asFloat(),
                    .rotation = assetlayer["rotation"].asFloat(),
                });
            }
        }
    }
}

//Compiles the rooms layers into a struct
void CompileRoomLayers(Json::Value yyfile){
    //Write to the struct
    for (int i = 0; i < (int)yyfile["layers"].size(); i++){
        printf("Compiling layer %i\n", i);
		Json::Value _layer = yyfile["layers"][i];
        LayerOut layer_out = {_layer["resourceType"], 0};

        //Create the structs the current layer index
        CreateLayerStruct_ASSET(_layer, layer_out); //Asset layer
        packed_layers.push_back(layer_out);
    }

    //Write the layers to the room file
    WriteLayers();
}

//Writes the rooms actual room function
//For example: void scr_runroom_ROOMNAME()
void WriteRoom_Runner(){
    //Create the func def
    char FuncLine[256] = "";
    snprintf(FuncLine, sizeof(FuncLine), "void scr_runroom_%s()", RoomName);

    //Begin the func
    File_WriteEnd(RoomFilePath_CPP, FuncLine);
    File_WriteEnd(RoomFilePath_CPP, "{\n");

    //Write header def
    File_WriteEnd(RoomFilePath_HPP, FuncLine);
    File_WriteEnd(RoomFilePath_HPP, ";\n");

    //Debug running room text
    char printf_name[256] = "";
    snprintf(printf_name, sizeof(printf_name), "    printf(\"RUNNING ROOM: %s\\n\");\n", RoomName);
    File_WriteEnd(RoomFilePath_CPP, printf_name);

    //Draw asset layer
    std::vector<int> asset_layer_index;
    for (int k = 0; k < (int)packed_layers.size(); k++){
        if (packed_layers[k].type == "GMRAssetLayer"){
            asset_layer_index.push_back(k);
        }
    }

    for (int i = 0; i < (int)asset_layer_index.size(); ++i) {
        char asset_layer[256] = "";
        snprintf(asset_layer, sizeof(asset_layer), "%s_asset_%i", RoomName, i+1);

        char ForLoop[256] = "";
        snprintf(ForLoop, sizeof(ForLoop), "	for (int i = 0; i < %s.assetCount; i++)\n", asset_layer);

        char DrawExt[256] = "";
        snprintf(DrawExt, sizeof(DrawExt), "        draw_sprite_ext(%s_data[i].sprite, 0, %s_data[i].x, %s_data[i].y, %s_data[i].scaleX, %s_data[i].scaleY, %s_data[i].rotation, 0, 1);\n\n", asset_layer, asset_layer, asset_layer, asset_layer, asset_layer, asset_layer);

        File_WriteEnd(RoomFilePath_CPP, ForLoop);
        File_WriteEnd(RoomFilePath_CPP, DrawExt);
    }

    //End the funcs
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

//Add to the asset ID list
void AssetID_Room(){
	const char* roomto_idh = "C:/GamemakerAnywhere/Runtime/source/helpers/asset_toid.h";
    char RoomDef[256];
    snprintf(RoomDef, sizeof(RoomDef), "#define %s %i //Room\n", RoomName, roomid_count);
	File_WriteEnd(roomto_idh, RoomDef);
}

//Set the room the game starts in (and increase the room counter)
void SetDefaultRoom(){
    if (roomid_count == 0){
        char NewStartRoom[256];
        snprintf(NewStartRoom, sizeof(NewStartRoom), "int room = %s;", RoomName);
        File_ReplaceLine("C:/GamemakerAnywhere/Runtime/source/main.cpp", "int room = -1; //DO NOT CHANGE THIS! GM CHECKS FOR \"-1\" TO EDIT IT TO THE FIRST ROOM!!!",NewStartRoom);
    }

    //Increase the room count!!
	roomid_count += 1;
}

//The actual room compiler
void scr_compilerooms(Json::Value yyfile, int i, Json::Value yyp_json){
    printf("\nCompiling Room...\n");

    //Init
    #pragma region 
    packed_layers = {};
    
    //Print room name
    snprintf(RoomName, sizeof(RoomName), "%s", yyfile["name"].asString().c_str());
    printf("Room Name: %s\n", RoomName);

    //Assign the room cpp/hpp file paths
    AssignRoomFilePath();

    //Create the file where the room data will be written to
    CreateRoomFile(RoomName);
    #pragma endregion

    //Writes the rooms includes
    WriteRoomHeader();

    //Compiles the rooms layers into a struct
    CompileRoomLayers(yyfile);

    //Writes the rooms actual room function
    //For example: void scr_runroom_ROOMNAME()
    WriteRoom_Runner();

    //Add to the room runner
    AddTo_RoomHandler();

    //Add to the asset ID list
    AssetID_Room();

    //Set the room the game starts in (and increase the room counter)
    SetDefaultRoom();
}

