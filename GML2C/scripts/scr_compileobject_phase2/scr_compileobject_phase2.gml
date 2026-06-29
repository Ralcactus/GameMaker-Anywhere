function scr_compileobject_phase2(spr_name, create_code, step_code, draw_code){
	var safe_name = sanitize_filename(yyfile.name);
	var file = file_text_open_write(destination + "source/objects/" + safe_name + ".cpp");
	
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
	file_text_write_string(file, "#include \"../variable_handler.h\"\n");
	file_text_write_string(file, "#include <variant>\n");
	file_text_write_string(file, "#include <vector>\n\n");
	/*file_text_write_string(file, "std::vector<" + safe_name + "_variableholder> vector_" + safe_name + ";\n");
	file_text_write_string(file, "static " + safe_name + "_variableholder* self; \n");*/
	file_text_write_string(file, "std::vector<Object> vector_" + safe_name + ";\n");
	file_text_write_string(file, "static Object* self; \n");
	
	file_text_write_string(file, "static int " + safe_name + "_call_index = 0;\n");
	file_text_write_string(file, "static int objectid_collided = 0;\n");
		
	for (var i = 0; i < array_length(var_names); i += 1)
		file_text_write_string(file, "#define " + var_names[i] + " " + "self->GetVar(varId_" + string(var_names[i]) + ")\n");
	
	#region EVENTS
	//config
	file_text_write_string(file, "void " + safe_name + "_config() {\n");
	file_text_write_string(file, "sprite_index = " + spr_name +";\n");
	file_text_write_string(file, "visible = " + string(yyfile.visible) +";\n");
	file_text_write_string(file, "solid = " + string(yyfile.solid) +";\n");
	file_text_write_string(file, "persistent = " + string(yyfile.persistent) +";\n");
	file_text_write_string(file, "}\n\n");
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
	file_text_write_string(file, "#undef x\n");
	file_text_write_string(file, "#undef y\n");
	file_text_write_string(file, "#undef image_xscale\n");
	file_text_write_string(file, "#undef image_yscale\n");
	file_text_write_string(file, "#undef id\n");
	
	file_text_write_string(file, "void " + safe_name + "_precreate(float NEWX, float NEWY, float NEWXSCALE, float NEWYSCALE, float NEWID) {\n");
	//file_text_write_string(file, safe_name + "_variableholder inst;\n");
	file_text_write_string(file, "Object inst;\n");
	file_text_write_string(file, "inst.GetVar(varId_x) = NEWX;\n");
	file_text_write_string(file, "inst.GetVar(varId_y) = NEWY;\n");
	file_text_write_string(file, "inst.GetVar(varId_image_xscale) = NEWXSCALE;\n");
	file_text_write_string(file, "inst.GetVar(varId_image_yscale) = NEWYSCALE;\n");
	file_text_write_string(file, "inst.GetVar(varId_id) = NEWID;\n");
	file_text_write_string(file, "vector_" + safe_name + ".push_back(inst);\n");
	file_text_write_string(file, "self = &vector_" + safe_name + ".back();\n");
	file_text_write_string(file, safe_name + "_config();\n");
	file_text_write_string(file, safe_name + "_create();\n");
	file_text_write_string(file, "}\n\n");
	
	//reset frame
	file_text_write_string(file, "void " + safe_name + "_reset_frame() {\n");
	file_text_write_string(file, "	" + safe_name + "_call_index = 0;\n");
	file_text_write_string(file, "}\n\n");
	
	//the event runner
	file_text_write_string(file, "void " + safe_name + "_runevents(float NEWX, float NEWY, float NEWXSCALE, float NEWYSCALE, float NEWID) {\n");
	
	//RUN THE EVENTS
	file_text_write_string(file, "	//printf(\"RUNNING OBJECT: " + safe_name + "\\n\");\n");
	file_text_write_string(file, "	bool found = false;\n");
	file_text_write_string(file, "	CurrentObjectRunning = self;\n");
	file_text_write_string(file, "	for(size_t j = 0; j < vector_"+safe_name+".size(); j++){\n");
	file_text_write_string(file, "		if(vector_"+safe_name+"[j].GetVar(varId_id) == NEWID){\n");
	file_text_write_string(file, "			found = true;\n")
	file_text_write_string(file, "			break;\n");
	file_text_write_string(file, "		}\n");
	file_text_write_string(file, "	}\n");
	file_text_write_string(file, "	if(!found)\n");
	file_text_write_string(file, "		"+safe_name+"_precreate(NEWX, NEWY, NEWXSCALE, NEWYSCALE, NEWID);\n");
	file_text_write_string(file, "	for(size_t j = 0; j < vector_" + safe_name + ".size(); j++){\n");
	file_text_write_string(file, "		self = &vector_" + safe_name + "[j];\n");
	file_text_write_string(file, "		" + safe_name + "_step();\n");
	file_text_write_string(file, "		" + safe_name + "_draw();\n\n");
	file_text_write_string(file, "	}\n");

	file_text_write_string(file, "}\n\n");
	file_text_close(file);

	
	var variable_handler = file_text_open_append(destination + "source/variable_handler.h");
	
	//define variables
	//file_text_write_string(variable_handler, "struct " + safe_name + "_variableholder {\n");
	
	scr_write_variables_custom(variable_handler);
	
	/*file_text_write_string(variable_handler, "};\n");
	file_text_write_string(variable_handler, "extern std::vector<" + safe_name + "_variableholder> vector_" + safe_name + ";\n");*/
	file_text_close(variable_handler);
}