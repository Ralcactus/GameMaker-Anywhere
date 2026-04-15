function init_builtin_variables(spr_name){
	var_names = [];
	var_defaults = [];

	//Movement And Position
	add_variable("x", "0");
	add_variable("y", "0");
	
	//Sprite Properties
	add_variable("sprite_index", spr_name);	
	add_variable("image_xscale", "1");
	add_variable("image_yscale", "1");
	
	
	//stubs (you can read and write to these but they do nothing)
	//General Variables
	add_variable("id", "0");
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

function scr_writevariables(file){
	//handle the saved variable
	for (var i = 0; i < array_length(var_names); i++){
		var vn = var_names[i];
		var vd = var_defaults[i];
		file_text_write_string(file, "static float saved_" + vn + "[8] = {" + vd + "};\n");
	}
	
	//handle the real variable
	for (var i = 0; i < array_length(var_names); i++)
		file_text_write_string(file, "static float " + var_names[i] + " = " + var_defaults[i] + ";\n");
}



function add_variable(name, value){
	array_push(var_names, name);
	array_push(var_defaults, value);
}