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

struct InstanceOut {
    std::string object;
    float x, y, scaleX, scaleY, rotation;
};

struct LayerOut {
    Json::Value type;
    int depth;
    std::vector<AssetOut> assets;
    std::vector<InstanceOut> instance;
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

		if (_layer.type == "GMRInstanceLayer") {
		    std::string cname = std::string(RoomName) + "_asset_" + std::to_string(k);

            char InstanceLayerStruct[256] = "";
            snprintf(InstanceLayerStruct, sizeof(InstanceLayerStruct), "static LayerInstances %s_data[] = {\n", cname.c_str());
		    File_WriteEnd(RoomFilePath_CPP, InstanceLayerStruct);

                for (int j = 0; j < (int)_layer.instance.size(); j++) {
                    InstanceOut& instance = _layer.instance[j];

                    File_WriteEnd(RoomFilePath_CPP, "    { ");
                    File_WriteEnd(RoomFilePath_CPP, (std::to_string(instance.x) + ", ").c_str());
                    File_WriteEnd(RoomFilePath_CPP, (std::to_string(instance.y) + ", ").c_str());
                    File_WriteEnd(RoomFilePath_CPP, (std::to_string(instance.rotation) + ", ").c_str());
                    File_WriteEnd(RoomFilePath_CPP, (std::to_string(instance.scaleX) + ", ").c_str());
                    File_WriteEnd(RoomFilePath_CPP, (std::to_string(instance.scaleY) + ", ").c_str());
                    File_WriteEnd(RoomFilePath_CPP, (instance.object + ", ").c_str());
                    File_WriteEnd(RoomFilePath_CPP, (std::to_string(100000+currentobject_count) + " },\n").c_str());
                }

		    File_WriteEnd(RoomFilePath_CPP, "};\n\n");

		    int count = (int)_layer.instance.size();

		    File_WriteEnd(RoomFilePath_CPP, ("static GMLayerInstance " + cname + " = {\n").c_str());
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

void CreateLayerStruct_INSTANCE(Json::Value _layer, LayerOut& layer_out){
    if (_layer["resourceType"] == "GMRInstanceLayer"){
        for (int j = 0; j < (int)_layer["instances"].size(); j++){
            Json::Value instlayer = _layer["instances"][j];

            layer_out.instance.push_back({
                .object = instlayer["objectId"]["name"].asString(),
                .x = instlayer["x"].asFloat(),
                .y = instlayer["y"].asFloat(),
                .scaleX = instlayer["scaleX"].asFloat(),
                .scaleY = instlayer["scaleY"].asFloat(),
                .rotation = instlayer["rotation"].asFloat(),
            });
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
        CreateLayerStruct_INSTANCE(_layer, layer_out); //Instance layer
        packed_layers.push_back(layer_out);
    }

    //Write the layers to the room file
    WriteLayers();
}

void BuildObjectRunEvents(std::string& got_funcs, std::string& object_scripts){
    got_funcs = "";
    object_scripts = "";
    std::vector<std::string> unique_objects;

    for (int k = 0; k < (int)packed_layers.size(); k++){
        if (packed_layers[k].type == "GMRInstanceLayer"){
            std::string cname = std::string(RoomName) + "_asset_" + std::to_string(k);
            std::vector<InstanceOut>& instances = packed_layers[k].instance;

            for (int j = 0; j < (int)instances.size(); j++){
                std::string datastruct = cname + "_data[" + std::to_string(j) + "]";
                std::string obj = instances[j].object;

                object_scripts += "	" + obj + "_runevents(" +
                    datastruct + ".x, " +
                    datastruct + ".y, " +
                    datastruct + ".scaleX, " +
                    datastruct + ".scaleY, " +
                    datastruct + ".id" + ");\n";

                bool already_seen = false;
                for (int m = 0; m < (int)unique_objects.size(); m++){
                    if (unique_objects[m] == obj){
                        already_seen = true;
                        break;
                    }
                }

                if (!already_seen){
                    unique_objects.push_back(obj);
                    got_funcs += "extern void " + obj + "_runevents(float, float, float, float, float);\n";
                    got_funcs += "extern void " + obj + "_reset_frame();\n";
                    object_scripts = "\t" + obj + "_reset_frame();\n" + object_scripts;
                }
            }
        }
    }
}

//Writes the rooms actual room function
//For example: void scr_runroom_ROOMNAME()
void WriteRoom_Runner(Json::Value yyfile){
    std::string got_funcs, object_scripts;
    BuildObjectRunEvents(got_funcs, object_scripts);

    //Write extern decls before the function
    File_WriteEnd(RoomFilePath_CPP, ("\n" + got_funcs + "\n").c_str());

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

    //Set room settings (cam size, room size, etc)
    File_WriteEnd(RoomFilePath_CPP, ("   if (" + std::string(RoomName) + "_views[0].visible == 1){\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, ("       view0_camWidth = " + std::string(RoomName) + "_views[0].camWidth;\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, ("       view0_camHeight = " + std::string(RoomName) + "_views[0].camHeight;\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, "   }\n   else{\n");
    File_WriteEnd(RoomFilePath_CPP, ("       view0_camWidth = " + std::string(RoomName) + "_INFO.width;\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, ("       view0_camHeight = " + std::string(RoomName) + "_INFO.height;\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, "   }\n\n");
    File_WriteEnd(RoomFilePath_CPP, ("   room_width = " + std::string(RoomName) + "_INFO.width;\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, ("   room_height = " + std::string(RoomName) + "_INFO.height;\n").c_str());

    File_WriteEnd(RoomFilePath_CPP, object_scripts.c_str());

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

//Write viewport struct to the room file
void WriteViewportStruct(Json::Value yyfile){
	//Viewports
	std::string view_array_name = std::string(RoomName) + "_views";

	//Views exist?
	int view_count = 0;
    view_count = yyfile["views"].size();

	//Write array
	File_WriteEnd(RoomFilePath_CPP, ("static GMViewPorts " + view_array_name + "[] = {\n").c_str());
	const char* coma = ",";

	for (int k = 0; k < view_count; k++){
		Json::Value v = yyfile["views"][k];

		//Safe object reference
        std::string objref = "0";
		objref = v["objectId"]["name"].asString();
		
		if(k == view_count - 1)
            coma = "";

        File_WriteEnd(RoomFilePath_CPP, "    {\n");
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["xview"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["yview"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["wview"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["hview"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["xport"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["yport"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["wport"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["hport"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["hborder"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["vborder"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["hspeed"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["vspeed"].asInt()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["inherit"].asBool()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::to_string(v["visible"].asBool()) + ",\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (objref + "\n").c_str());
        File_WriteEnd(RoomFilePath_CPP, (std::string("    }") + coma + "\n").c_str());
	}

	File_WriteEnd(RoomFilePath_CPP, "};\n\n");
}

//Write room info like room size, viewport size, persistent, etc
void WriteRoom_Metadata(Json::Value yyfile, int i){
    File_WriteEnd(RoomFilePath_CPP, ("GMRoom " + std::string(RoomName) + "_INFO = {\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(i) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, ("\"" + yyfile["name"].asString() + "\",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["roomSettings"]["Width"].asInt()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["roomSettings"]["Height"].asInt()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["roomSettings"]["persistent"].asBool()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["roomSettings"]["inheritRoomSettings"].asBool()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["viewSettings"]["enableViews"].asBool()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["viewSettings"]["clearDisplayBuffer"].asBool()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["viewSettings"]["clearViewBackground"].asBool()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["viewSettings"]["inheritViewSettings"].asBool()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(ARRAYSIZE(yyfile["views"])) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["physicsSettings"]["inheritPhysicsSettings"].asBool()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["physicsSettings"]["PhysicsWorld"].asBool()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["physicsSettings"]["PhysicsWorldGravityX"].asInt()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["physicsSettings"]["PhysicsWorldGravityY"].asInt()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, (std::to_string(yyfile["physicsSettings"]["PhysicsWorldPixToMetres"].asInt()) + ",\n").c_str());
    File_WriteEnd(RoomFilePath_CPP, "};\n");
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

    //Write viewport struct to the room file
    WriteViewportStruct(yyfile);

    //Compiles the rooms layers into a struct
    CompileRoomLayers(yyfile);

    //Write room info like room size, viewport size, persistent, etc
    WriteRoom_Metadata(yyfile, i);

    //Writes the rooms actual room function
    //For example: void scr_runroom_ROOMNAME()
    WriteRoom_Runner(yyfile);

    //Add to the room runner
    AddTo_RoomHandler();

    //Add to the asset ID list
    AssetID_Room();

    //Set the room the game starts in (and increase the room counter)
    SetDefaultRoom();
}

