function scr_compileobject_phase2(spr_name, create_code, step_code){
	var safe_name = sanitize_filename(yyfile.name);
	var file = file_text_open_write(destination + "source\\objects\\" + safe_name + ".c");
	
	file_text_write_string(file, "// Object: " + yyfile.name + "\n\n");
	file_text_write_string(file, "#include <stdbool.h>\n");
	file_text_write_string(file, "#include <stdio.h>\n");
	file_text_write_string(file, "#include <string.h>\n");
	file_text_write_string(file, "#include \"../sprite_toid.h\"\n");
	file_text_write_string(file, "#include \"../cross_platform/drawing.h\"\n\n");
	file_text_write_string(file, "#include \"../cross_platform/input.h\"\n\n");
	file_text_write_string(file, "#include \"../room_handler.h\"\n\n");
	file_text_write_string(file, "#include \"../room_tostring.h\"\n\n");
	
	
	//define variables
	file_text_write_string(file, "static bool initialized[8] = {false};\n");
	file_text_write_string(file, "static float saved_x[8] = {0};\n");
	file_text_write_string(file, "static float saved_y[8] = {0};\n");
	file_text_write_string(file, "static float saved_xscale[8] = {1};\n");
	file_text_write_string(file, "static float saved_yscale[8] = {1};\n");
	file_text_write_string(file, "static int " + safe_name + "_call_index = 0;\n");
	file_text_write_string(file, "static float x = 0;\n");
	file_text_write_string(file, "static float y = 0;\n");
	file_text_write_string(file, "static float image_xscale = 1;\n");
	file_text_write_string(file, "static float image_yscale = 1;\n");
	file_text_write_string(file, "static float sprite_index = " + spr_name + ";\n");
	
	#region EVENTS
	//create
	file_text_write_string(file, "void " + safe_name + "_create() {\n");
	file_text_write_string(file, create_code + "\n");
	file_text_write_string(file, "}\n\n");
	//step
	file_text_write_string(file, "void " + safe_name + "_step() {\n");
	file_text_write_string(file, step_code + "\n");
	file_text_write_string(file, "}\n");
	//draw
	file_text_write_string(file, "void " + safe_name + "_draw() {\n");
	file_text_write_string(file, "draw_sprite_ext(sprite_index, 0, x, y, image_xscale, image_yscale, 0, 0, 1);\n");
	file_text_write_string(file, "}\n\n");
	#endregion
	
	//pre-create 
	file_text_write_string(file, "void " + safe_name + "_precreate(float NEWX, float NEWY, float NEWXSCALE, float NEWYSCALE) {\n");
	file_text_write_string(file, "x = NEWX;\n");
	file_text_write_string(file, "y = NEWY;\n");
	file_text_write_string(file, "image_xscale = NEWXSCALE;\n");
	file_text_write_string(file, "image_yscale = NEWYSCALE;\n");
	file_text_write_string(file, "}\n\n");
	
	//reset frame
	file_text_write_string(file, "void " + safe_name + "_reset_frame() {\n");
	file_text_write_string(file, "	" + safe_name + "_call_index = 0;\n");
	file_text_write_string(file, "}\n\n");
	
	//the event runner
	file_text_write_string(file, "void " + safe_name + "_runevents(float NEWX, float NEWY, float NEWXSCALE, float NEWYSCALE) {\n");
	
	//RUN THE EVENTS
	file_text_write_string(file, "	int i = " + safe_name + "_call_index++;\n\n");
	file_text_write_string(file, "	if (!initialized[i]){\n");
	file_text_write_string(file, "		saved_x[i] = NEWX;\n");
	file_text_write_string(file, "		saved_y[i] = NEWY;\n");
	file_text_write_string(file, "		saved_xscale[i] = NEWXSCALE;\n");
	file_text_write_string(file, "		saved_yscale[i] = NEWYSCALE;\n");
	file_text_write_string(file, "		initialized[i] = true;\n");
	file_text_write_string(file, "		" + safe_name + "_precreate(NEWX, NEWY, NEWXSCALE, NEWYSCALE);\n");
	file_text_write_string(file, "		" + safe_name + "_create();\n");
	file_text_write_string(file, "	}\n\n");
	file_text_write_string(file, "	x = saved_x[i];\n");
	file_text_write_string(file, "	y = saved_y[i];\n\n");
	file_text_write_string(file, "	image_xscale = saved_xscale[i];\n");
	file_text_write_string(file, "	image_yscale = saved_yscale[i];\n\n");
	file_text_write_string(file, "	" + safe_name + "_step();\n");
	file_text_write_string(file, "	" + safe_name + "_draw();\n\n");
	file_text_write_string(file, "	saved_x[i] = x;\n");
	file_text_write_string(file, "	saved_y[i] = y;\n");
	file_text_write_string(file, "	saved_xscale[i] = image_xscale;\n");
	file_text_write_string(file, "	saved_yscale[i] = image_yscale;\n");
	
	file_text_write_string(file, "}\n\n");
	file_text_close(file);
}