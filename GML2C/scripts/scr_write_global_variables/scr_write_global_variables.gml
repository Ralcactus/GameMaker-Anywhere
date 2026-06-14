function scr_write_global_variables(){
	var variable_handler = file_text_open_append(destination + "source/variable_handler.h");
	file_text_write_string(variable_handler, "struct global_bleh {\n");
	
	scr_write_globalvariable(variable_handler);
	
	file_text_write_string(variable_handler, "};\n");
	file_text_write_string(variable_handler, "inline global_bleh global;\n");
	file_text_close(variable_handler);
}
