#include <iostream>
#include <windows.h>
#include <shobjidl.h>
#include <SDL3/SDL.h>
#include <json/json.h>
#include <fstream>
#include <vector>
#include "../../helpers/meta.hpp"

std::vector<const char*> VarNameArray;
std::vector<const char*> VarDefaultArray;

//Add variable to the array
void add_variable(const char* VarName, const char* DefaultValue){
    VarNameArray.push_back(VarName);
    VarDefaultArray.push_back(DefaultValue);
}

void VarBuiltIn_Init(){
    VarNameArray.clear();
    VarDefaultArray.clear();

    //Movement And Position
    add_variable("x", "0");
    add_variable("y", "0");

    //Sprite Properties
    add_variable("sprite_index", "-4");
    add_variable("image_xscale", "1");
    add_variable("image_yscale", "1");
    add_variable("sprite_xoffset", "0");
    add_variable("sprite_yoffset", "0");
    add_variable("image_index", "0");

    //STUBS
    //(you can read and write to these but they do nothing)

    //General Variables
    add_variable("id", "-4");
    add_variable("visible", "false");
    add_variable("solid", "false");
    add_variable("persistent", "false");
    add_variable("depth", "0");
    add_variable("layer", "-4");
    add_variable("on_ui_layer", "false");
    add_variable("collision_space", "-4");

    //Movement And Position
    add_variable("direction", "0");
    add_variable("friction", "0");
    add_variable("gravity", "0");
    add_variable("gravity_direction", "0");
    add_variable("hspeed", "0");
    add_variable("vspeed", "0");
    add_variable("speed", "0");
    add_variable("xstart", "0");
    add_variable("ystart", "0");
    add_variable("xprevious", "0");
    add_variable("yprevious", "0");
    add_variable("object_index", "0");

    //Sprite Properties
    add_variable("sprite_width", "0");
    add_variable("sprite_height", "0");
    add_variable("image_alpha", "0");
    add_variable("image_angle", "0");
    add_variable("image_blend", "0");
    add_variable("image_number", "0");
    add_variable("image_speed", "0");

    //Alarm
    add_variable("alarm", "-1");
}

void VarBuiltIn_Write(){
	//add to variable helper
	const char* VarHandle_Path = "C:/GamemakerAnywhere/Runtime/source/variable_handler.h";
	const char* VarObject_Path = "C:/GamemakerAnywhere/Runtime/source/helpers/var_in_object_running.h";

	char VarCreate[256];

	for (int i = 0; i < VarNameArray.size(); i++){
		snprintf(VarCreate, sizeof(VarCreate), "inline VarNode globVar_%s = {%d, %s};\n", VarNameArray[i], i, VarDefaultArray[i]);
		File_WriteEnd(VarHandle_Path, VarCreate);

		snprintf(VarCreate, sizeof(VarCreate), "#define varId_%s globVar_%s.vId\n", VarNameArray[i], VarNameArray[i]);
		File_WriteEnd(VarHandle_Path, VarCreate);
	}
	
	//add to var_in_object_running (for other script files like custom scripts or funcs that need variables in them like draw_self)
	for (int i = 0; i < VarNameArray.size(); i++){
		snprintf(VarCreate, sizeof(VarCreate), "#define %s CurrentObjectRunning->GetVar(varId_%s)\n", VarNameArray[i], VarNameArray[i]);
		File_WriteEnd(VarObject_Path, VarCreate);
	}
}