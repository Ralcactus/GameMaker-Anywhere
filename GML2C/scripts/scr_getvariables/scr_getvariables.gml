function init_builtin_variables(){
	var_names = [];
	var_defaults = [];

	//Movement And Position
	add_variable("x", "0");
	add_variable("y", "0");
	
	//Sprite Properties
	add_variable("sprite_index", "-4");	
	add_variable("image_xscale", "1");
	add_variable("image_yscale", "1");
	
	//stubs (you can read and write to these but they do nothing)
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
	add_variable("sprite_xoffset", "0");
	add_variable("sprite_yoffset", "0");
	add_variable("image_alpha", "0");	
	add_variable("image_angle", "0");
	add_variable("image_blend", "0");
	add_variable("image_index", "0");
	add_variable("image_number", "0");
	add_variable("image_speed", "0");
}

function scr_write_variables_custom(file){
	//handle the real variable
	for (var i = 0; i < array_length(general_varnames); i++){
		if(array_contains(varname_written, general_varnames[i]))
			continue;
		
		file_text_write_string(file, "inline VarNode globVar_" + general_varnames[i] + " = {" + string(i+array_length(var_names)-2) + ", " + general_vardefaults[i] + "};\n");
		file_text_write_string(file, "#define varId_" + general_varnames[i] + " globVar_" + general_varnames[i] + ".vId\n");
		array_push(varname_written, general_varnames[i]);
	}
	
	//add to VarInObjectRunning (for other script files like custom scripts or funcs that need variables in them like draw_self)
	var VarInObjectRunning = file_text_open_append(destination + "source/Helpers/VarInObjectRunning.h");
	for (var i = 0; i < array_length(general_varnames); i++){
		file_text_write_string(VarInObjectRunning, "#define " + general_varnames[i] + " CurrentObjectRunning->GetVar(varId_" + general_varnames[i] + ")\n");
	}
	file_text_close(VarInObjectRunning);
}

function scr_write_variables_builtin(){
	//add to variable helper
	var variable_handler = file_text_open_append(destination + "source/variable_handler.h");
	for (var i = 0; i < array_length(var_names); i++){
		file_text_write_string(variable_handler, "inline VarNode globVar_" + var_names[i] + " = {" + string(i) + ", " + var_defaults[i] + "};\n");
		file_text_write_string(variable_handler, "#define varId_" + var_names[i] + " globVar_" + var_names[i] + ".vId\n");
	}
	file_text_close(variable_handler);
	
	//add to VarInObjectRunning (for other script files like custom scripts or funcs that need variables in them like draw_self)
	var VarInObjectRunning = file_text_open_append(destination + "source/Helpers/VarInObjectRunning.h");
	for (var i = 0; i < array_length(var_names); i++){
		file_text_write_string(VarInObjectRunning, "#define " + var_names[i] + " CurrentObjectRunning->GetVar(varId_" + var_names[i] + ")\n");
	}
	file_text_close(VarInObjectRunning);
		
}

function scr_write_globalvariable(file){
	//handle the real variable
	for (var i = 0; i < array_length(globalvar_names); i++)
		file_text_write_string(file, "GMvar " + globalvar_names[i] + " = 0;\n");
}

function add_variable(name, value){
	array_push(var_names, name);
	array_push(var_defaults, value);
}

function add_variable_general(name, value){
	array_push(general_varnames, name);
	array_push(general_vardefaults, value);
}

