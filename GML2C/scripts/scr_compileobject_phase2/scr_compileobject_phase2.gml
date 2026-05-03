function scr_compileobject_phase2(spr_name, create_code, step_code, draw_code){
	var safe_name = sanitize_filename(yyfile.name);
	var file = file_text_open_write(destination + "source\\objects\\" + safe_name + ".cpp");
	init_builtin_variables(spr_name);
	
	file_text_write_string(file, "// Object: " + yyfile.name + "\n\n");
	file_text_write_string(file, "#include <stdbool.h>\n");
	file_text_write_string(file, "#include <stdio.h>\n");
	file_text_write_string(file, "#include <string.h>\n");
	file_text_write_string(file, "#include \"../sprite_toid.h\"\n");
	file_text_write_string(file, "#include \"../gm_funcs/drawing.h\"\n");
	file_text_write_string(file, "#include \"../gm_funcs/input.h\"\n");
	file_text_write_string(file, "#include \"../gm_funcs/input.h\"\n");
	file_text_write_string(file, "#include \"../room_toid.h\"\n");
	file_text_write_string(file, "#include \"../gm_funcs/misc.h\"\n");
	file_text_write_string(file, "#include \"../gm_funcs/collision.h\"\n");
	file_text_write_string(file, "#include \"../gm_funcs/audio.h\"\n");
	file_text_write_string(file, "#include \"../custom_funcs/customfuncs.h\"\n");
	file_text_write_string(file, "#include \"../get_spriteinfo.h\"\n");
	file_text_write_string(file, "#include <variant>\n\n");
	//define variables
	file_text_write_string(file, "static bool initialized[8] = {false};\n");
	file_text_write_string(file, "static int " + safe_name + "_call_index = 0;\n");
	file_text_write_string(file, "static int objectid_collided = 0;\n");
	
	
	
	//other stuff
	scr_write_builtinvariables(file);
	var caught = variable_clone(var_names); //caugt means we've already declared the variable
	file_text_write_string(file, get_variables(caught, create_code));
	file_text_write_string(file, get_variables(caught, step_code));
	file_text_write_string(file, get_variables(caught, draw_code));
		
	#region EVENTS
	//create
	file_text_write_string(file, "void " + safe_name + "_create() {\n");
	file_text_write_string(file, create_code + "\n");
	file_text_write_string(file, "}\n\n");
	//step
	file_text_write_string(file, "void " + safe_name + "_step() {\n");
	file_text_write_string(file, step_code + "\n" + "objectid_collided = get_current_object_touching(x, y, image_xscale, image_yscale, sprite_index, (int)id);\n");
	file_text_write_string(file, "}\n");
	//draw
	file_text_write_string(file, "void " + safe_name + "_draw() {\n");
	if (draw_code == "")
		file_text_write_string(file, "draw_sprite_ext(sprite_index, 0, x, y, image_xscale, image_yscale, 0, 0, 1);\n"); //change this to draw_self() later
	else
		file_text_write_string(file, draw_code + "\n");
	file_text_write_string(file, "}\n\n");
	#endregion
	
	//pre-create 
	file_text_write_string(file, "void " + safe_name + "_precreate(float NEWX, float NEWY, float NEWXSCALE, float NEWYSCALE, float NEWID) {\n");
	file_text_write_string(file, "x = NEWX;\n");
	file_text_write_string(file, "y = NEWY;\n");
	file_text_write_string(file, "image_xscale = NEWXSCALE;\n");
	file_text_write_string(file, "image_yscale = NEWYSCALE;\n");
	file_text_write_string(file, "id = NEWID;\n");
	file_text_write_string(file, "}\n\n");
	
	//reset frame
	file_text_write_string(file, "void " + safe_name + "_reset_frame() {\n");
	file_text_write_string(file, "	" + safe_name + "_call_index = 0;\n");
	file_text_write_string(file, "}\n\n");
	
	//the event runner
	file_text_write_string(file, "void " + safe_name + "_runevents(float NEWX, float NEWY, float NEWXSCALE, float NEWYSCALE, float NEWID) {\n");
	
	//RUN THE EVENTS
	file_text_write_string(file, "	//printf(\"RUNNING OBJECT: " + safe_name + "\\n\");\n");
	file_text_write_string(file, "	int i = " + safe_name + "_call_index++;\n\n");
	file_text_write_string(file, "	if (!initialized[i]){\n");
	file_text_write_string(file, "		initialized[i] = true;\n");
	file_text_write_string(file, "		" + safe_name + "_precreate(NEWX, NEWY, NEWXSCALE, NEWYSCALE, NEWID);\n");
	file_text_write_string(file, "		" + safe_name + "_create();\n");
	
	file_text_write_string(file, "	} else {\n");
	for (var i = 0; i < array_length(var_names); i++) {
		var vn = var_names[i];
		file_text_write_string(file, "		" + vn + " = saved_" + vn + "[i];\n");
	}
	file_text_write_string(file, "	}\n\n");
	
	file_text_write_string(file, "	" + "otherobject_x[(int)id - 100000] = x;" + "\n");
	file_text_write_string(file, "	" + "otherobject_y[(int)id - 100000] = y;" + "\n");
	file_text_write_string(file, "	" + "otherobject_xscale[(int)id - 100000] = image_xscale;" + "\n");
	file_text_write_string(file, "	" + "otherobject_yscale[(int)id - 100000] = image_yscale;" + "\n");
	file_text_write_string(file, "	" + "otherobject_sprite[(int)id - 100000] = sprite_index;" + "\n\n");
	
	file_text_write_string(file, "	" + safe_name + "_step();\n");
	file_text_write_string(file, "	" + safe_name + "_draw();\n\n");
	
	for (var i = 0; i < array_length(var_names); i++) {
		var vn = var_names[i];
		file_text_write_string(file, "	saved_" + vn + "[i] = " + vn + ";\n");
	}

	file_text_write_string(file, "}\n\n");
	file_text_close(file);
}