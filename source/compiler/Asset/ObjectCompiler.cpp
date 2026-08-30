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

const char* AssetSprite = "";
const char* create_code = "";
const char* step_code = "";
const char* draw_code = "";
char steppath[256];
char createpath[256];
char drawpath[256];

const char* ObjectName = "";

void WriteHeader(const char* ObjectFile){
	File_WriteEnd(ObjectFile, "#include <stdbool.h>\n");
	File_WriteEnd(ObjectFile, "#include <stdio.h>\n");
	File_WriteEnd(ObjectFile, "#include <string.h>\n");
	File_WriteEnd(ObjectFile, "#include <math.h>\n");
	File_WriteEnd(ObjectFile, "#include \"../helpers/asset_toid.h\"\n");
	File_WriteEnd(ObjectFile, "#include \"../gm_funcs/drawing.h\"\n");
	File_WriteEnd(ObjectFile, "#include \"../gm_funcs/input.h\"\n");
	File_WriteEnd(ObjectFile, "#include \"../gm_funcs/input.h\"\n");
	File_WriteEnd(ObjectFile, "#include \"../gm_funcs/misc.h\"\n");
	File_WriteEnd(ObjectFile, "#include \"../gm_funcs/collision.h\"\n");
	File_WriteEnd(ObjectFile, "#include \"../gm_funcs/audio.h\"\n");
	File_WriteEnd(ObjectFile, "#include \"../gm_funcs/filesystem.h\"\n");
	File_WriteEnd(ObjectFile, "#include \"../custom_funcs/customfuncs.h\"\n");
	File_WriteEnd(ObjectFile, "#include \"../variable_handler.h\"\n");
	File_WriteEnd(ObjectFile, "#include \"../helpers/get_spriteinfo.h\"\n");
	File_WriteEnd(ObjectFile, "#include \"../helpers/asset_toid.h\"\n");
    File_WriteEnd(ObjectFile, "#include \"../helpers/var_in_object_running.h\"\n");
	File_WriteEnd(ObjectFile, "#include <variant>\n");
	File_WriteEnd(ObjectFile, "#include <vector>\n\n");
	File_WriteEnd(ObjectFile, "#include <string>\n\n");
}

void WriteConfig(const char* ObjectFile, const char* spr_name, Json::Value yyfile){
    // config
    File_WriteEnd(ObjectFile, (std::string("void ") + ObjectName + "_config() {\n").c_str());
    File_WriteEnd(ObjectFile, (std::string("sprite_index = ") + spr_name + ";\n").c_str());
    File_WriteEnd(ObjectFile, (std::string("visible = ") + yyfile["visible"].asString() + ";\n").c_str());
    File_WriteEnd(ObjectFile, (std::string("solid = ") + yyfile["solid"].asString() + ";\n").c_str());
    File_WriteEnd(ObjectFile, (std::string("persistent = ") + yyfile["persistent"].asString() + ";\n").c_str());
    File_WriteEnd(ObjectFile, "}\n\n");
}

void WriteCreate(const char* ObjectFile){
    File_WriteEnd(ObjectFile, (std::string("void ") + ObjectName + "_create() {\n").c_str());
    File_WriteEnd(ObjectFile, create_code);
    File_WriteEnd(ObjectFile, "\n}\n\n");
}

void WriteStep(const char* ObjectFile){
    File_WriteEnd(ObjectFile, (std::string("void ") + ObjectName + "_step() {\n").c_str());
    File_WriteEnd(ObjectFile, step_code);
    File_WriteEnd(ObjectFile, "\n//draw_boundbox();\n");
    File_WriteEnd(ObjectFile, "}\n");
}

void WriteDraw(const char* ObjectFile){
    File_WriteEnd(ObjectFile, (std::string("void ") + ObjectName + "_draw() {\n").c_str());

    if (draw_code == NULL || strcmp(draw_code, "") == 0)
        File_WriteEnd(ObjectFile, "draw_self();\n");
    else
        File_WriteEnd(ObjectFile, draw_code);

    File_WriteEnd(ObjectFile, "}\n\n");
}

void WritePreCreate(const char* ObjectFile){
    File_WriteEnd(ObjectFile, "#undef x\n");
    File_WriteEnd(ObjectFile, "#undef y\n");
    File_WriteEnd(ObjectFile, "#undef image_xscale\n");
    File_WriteEnd(ObjectFile, "#undef image_yscale\n");
    File_WriteEnd(ObjectFile, "#undef id\n");

    File_WriteEnd(ObjectFile, (std::string("void ") + ObjectName + "_precreate(float NEWX, float NEWY, float NEWXSCALE, float NEWYSCALE, float NEWID) {\n").c_str());

    File_WriteEnd(ObjectFile, "Object inst;\n");

    File_WriteEnd(ObjectFile, "inst.GetVar(varId_x) = NEWX;\n");
    File_WriteEnd(ObjectFile, "inst.GetVar(varId_y) = NEWY;\n");
    File_WriteEnd(ObjectFile, "inst.GetVar(varId_image_xscale) = NEWXSCALE;\n");
    File_WriteEnd(ObjectFile, "inst.GetVar(varId_image_yscale) = NEWYSCALE;\n");
    File_WriteEnd(ObjectFile, "inst.GetVar(varId_id) = NEWID;\n");
}

void WriteEvents(const char* ObjectFile, const char* spr_name, Json::Value yyfile){
    WriteConfig(ObjectFile, spr_name, yyfile);
    WriteCreate(ObjectFile);
    WriteStep(ObjectFile);
    WriteDraw(ObjectFile);
    WritePreCreate(ObjectFile);
}

void scr_compileobject_phase2(Json::Value yyfile, const char* spr_name, const char* create_code, const char* step_code, const char* draw_code){
	ObjectName = yyfile["name"].asCString();

    //Get object file cpp and hpp file
    char ObjectFile[256];
    char ObjectFile_HPP[256];
    snprintf(ObjectFile, sizeof(ObjectFile), "C:/GamemakerAnywhere/Runtime/source/objects/%s.cpp", ObjectName);
    snprintf(ObjectFile_HPP, sizeof(ObjectFile_HPP), "C:/GamemakerAnywhere/Runtime/source/objects/%s.hpp", ObjectName);
	
    //Create the hpp
    char CppCreate[256];
    snprintf(CppCreate, sizeof(CppCreate), "powershell -Command \"New-Item -Path 'C:/GamemakerAnywhere/Runtime/source/objects/%s.cpp' -Force\"", ObjectName);
    char HppCreate[256];
    snprintf(HppCreate, sizeof(HppCreate), "powershell -Command \"New-Item -Path 'C:/GamemakerAnywhere/Runtime/source/objects/%s.hpp' -Force\"", ObjectName);
    system(CppCreate);
    system(HppCreate);

    WriteHeader(ObjectFile);

	File_WriteEnd(ObjectFile, ("std::vector<Object> vector_" + std::string(ObjectName) + ";\n").c_str());
	File_WriteEnd(ObjectFile, "static Object* self; \n");
	File_WriteEnd(ObjectFile, ("static int " + std::string(ObjectName) + "_call_index = 0;\n").c_str());
	File_WriteEnd(ObjectFile, "static int objectid_collided = 0;\n");

    WriteEvents(ObjectFile, spr_name, yyfile);

    /*
    UNCOMMENT WHEN ARRAYS WORK!!

    File_WriteEnd(ObjectFile, "alarm[0] = -1;\n");
    File_WriteEnd(ObjectFile, "alarm[1] = -1;\n");
    File_WriteEnd(ObjectFile, "alarm[2] = -1;\n");
    File_WriteEnd(ObjectFile, "alarm[3] = -1;\n");
    File_WriteEnd(ObjectFile, "alarm[4] = -1;\n");
    File_WriteEnd(ObjectFile, "alarm[5] = -1;\n");
    File_WriteEnd(ObjectFile, "alarm[6] = -1;\n");
    File_WriteEnd(ObjectFile, "alarm[7] = -1;\n");
    File_WriteEnd(ObjectFile, "alarm[8] = -1;\n");
    File_WriteEnd(ObjectFile, "alarm[9] = -1;\n");
    File_WriteEnd(ObjectFile, "alarm[10] = -1;\n");
    File_WriteEnd(ObjectFile, "alarm[11] = -1;\n");
    */

    File_WriteEnd(ObjectFile, (std::string("vector_") + ObjectName + ".push_back(inst);\n").c_str());
    File_WriteEnd(ObjectFile, (std::string("self = &vector_") + ObjectName + ".back();\n").c_str());

    File_WriteEnd(ObjectFile, (std::string(ObjectName) + "_config();\n").c_str());
    File_WriteEnd(ObjectFile, (std::string(ObjectName) + "_create();\n").c_str());

    File_WriteEnd(ObjectFile, "}\n\n");

    // reset frame
    File_WriteEnd(ObjectFile, (std::string("void ") + ObjectName + "_reset_frame() {\n").c_str());
    File_WriteEnd(ObjectFile, (std::string("\t") + ObjectName + "_call_index = 0;\n").c_str());
    File_WriteEnd(ObjectFile, "}\n\n");

    // event runner
    File_WriteEnd(ObjectFile, (std::string("void ") + ObjectName + "_runevents(float NEWX, float NEWY, float NEWXSCALE, float NEWYSCALE, float NEWID) {\n").c_str());
    File_WriteEnd(ObjectFile_HPP, (std::string("void ") + ObjectName + "_runevents(float NEWX, float NEWY, float NEWXSCALE, float NEWYSCALE, float NEWID);\n").c_str());

    // RUN THE EVENTS
    File_WriteEnd(ObjectFile, (std::string("\t//printf(\"RUNNING OBJECT: ") + ObjectName + "\\n\");\n").c_str());

    File_WriteEnd(ObjectFile, "\tbool found = false;\n");

    File_WriteEnd(ObjectFile, (std::string("\tfor(size_t j = 0; j < vector_") + ObjectName + ".size(); j++){\n").c_str());

    File_WriteEnd(ObjectFile, (std::string("\t\tif(vector_") + ObjectName + "[j].GetVar(varId_id) == NEWID){\n").c_str());

    File_WriteEnd(ObjectFile, "\t\t\tfound = true;\n");
    File_WriteEnd(ObjectFile, "\t\t\tbreak;\n");

    File_WriteEnd(ObjectFile, "\t\t}\n");
    File_WriteEnd(ObjectFile, "\t}\n");

    File_WriteEnd(ObjectFile, "\tif(!found)\n");

    File_WriteEnd(
        ObjectFile,
        (std::string("\t\t") + ObjectName + "_precreate(NEWX, NEWY, NEWXSCALE, NEWYSCALE, NEWID);\n").c_str()
    );

    File_WriteEnd(ObjectFile, (std::string("\tfor(size_t j = 0; j < vector_") + ObjectName + ".size(); j++){\n").c_str());

    File_WriteEnd(ObjectFile, (std::string("\t\tself = &vector_") + ObjectName + "[j];\n").c_str());

    File_WriteEnd(ObjectFile, "\t\tCurrentObjectRunning = self;\n");

    File_WriteEnd(ObjectFile, (std::string("\t\t") + ObjectName + "_step();\n").c_str());
    File_WriteEnd(ObjectFile, (std::string("\t\t") + ObjectName + "_draw();\n\n").c_str());

    File_WriteEnd(ObjectFile, "\t}\n");

    // sprite animation
    File_WriteEnd(ObjectFile, "\tif (SpriteAnimSpeedType[sprite_index] == 0){\n");
    File_WriteEnd(ObjectFile, "\t\timage_index+=SpriteAnimTimer[sprite_index]/fps;\n");
    File_WriteEnd(ObjectFile, "\t}\n");

    File_WriteEnd(ObjectFile, "\telse\n");
    File_WriteEnd(ObjectFile, "\t\timage_index+=SpriteAnimTimer[sprite_index];\n");

    File_WriteEnd(ObjectFile, "\tif (image_index >= SpriteFrameCount[sprite_index]){\n");
    File_WriteEnd(ObjectFile, "\t\timage_index = 0;\n");
    File_WriteEnd(ObjectFile, "\t}\n");

    File_WriteEnd(ObjectFile, "}\n\n");

	/*
	var variable_handler = file_text_open_append(destination + "source/variable_handler.h");
	
	//define variables
	//file_text_write_string(variable_handler, "struct " + ObjectName + "_variableholder {\n");
	
	scr_write_variables_custom(variable_handler);
	
	/*file_text_write_string(variable_handler, "};\n");
	file_text_write_string(variable_handler, "extern std::vector<" + ObjectName + "_variableholder> vector_" + ObjectName + ";\n");*/
	//file_text_close(variable_handler);
    
}

void scr_compileobjects(Json::Value yyfile, Json::Value _id){
    printf("Compiling Object...\n");

    if (yyfile["spriteId"].isString())
        AssetSprite = yyfile["spriteId"].asString().c_str();
    else
        AssetSprite = "-1"; //object doesn't have a sprite
    

    char ObjectDir[256];

    snprintf(ObjectDir, sizeof(ObjectDir), "%s", _id["path"].asString().c_str());

    char* LastSlash = strrchr(ObjectDir, '/');

    if (!LastSlash)
        LastSlash = strrchr(ObjectDir, '\\');

    if (LastSlash)
        *LastSlash = '\0';

    snprintf(steppath, sizeof(steppath), "%s/%s/Step_0.gml", File_GetDir(ProjectYYP), ObjectDir);
    snprintf(createpath, sizeof(createpath), "%s/%s/Create_0.gml", File_GetDir(ProjectYYP), ObjectDir);
    snprintf(drawpath, sizeof(drawpath), "%s/%s/Draw_0.gml", File_GetDir(ProjectYYP), ObjectDir);

    create_code = File_ToChar(createpath);
    step_code = File_ToChar(steppath);
    draw_code = File_ToChar(drawpath);
	
	scr_compileobject_phase2(yyfile, AssetSprite, create_code, step_code, draw_code);
	currentobject_count--;
	
    /*
	var spriteidh = file_text_open_append(destination + "source/helpers/asset_toid.h");
	file_text_write_string(spriteidh, "#define " + yyfile.name + " " + string(currentobject_count) + " //Object\n");
	file_text_close(spriteidh);
    */

    currentobject_count++;
}