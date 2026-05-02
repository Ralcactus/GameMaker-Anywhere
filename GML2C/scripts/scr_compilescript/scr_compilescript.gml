function scr_compilescript(){
	show_debug_message("Script: " + yyfile.name);
	show_debug_message(filename_dir(global.selected_yyp) + "\\scripts\\" + yyfile.name + "\\" + yyfile.name + ".gml");
	
	var customfuncsCPP = file_text_open_append(destination + "source\\custom_funcs\\customfuncs.cpp");
	var customfuncsH = file_text_open_append(destination + "source\\custom_funcs\\customfuncs.h");
	var gmlfilebuffer = buffer_load(filename_dir(global.selected_yyp) + "\\scripts\\" + yyfile.name + "\\" + yyfile.name + ".gml");
    var gml_code = buffer_read(gmlfilebuffer, buffer_string);
    buffer_delete(gmlfilebuffer);
	
	file_text_write_string(customfuncsCPP, gml_code + "\n");
	
	//h file
	var pos = string_pos("function", gml_code);
	while (pos > 0){
		var bracket_open  = string_pos_ext("(", gml_code, pos);
		var bracket_close = string_pos_ext(")", gml_code, bracket_open);
        
		if (bracket_open > 0 && bracket_close > 0) {
			var sig = string_copy(gml_code, pos, (bracket_close - pos)+1);
			file_text_write_string(customfuncsH, sig + ";\n");
		}

		pos = string_pos_ext("function", gml_code, pos + 1);
	}
	
	file_text_close(customfuncsCPP);
	file_text_close(customfuncsH);
}