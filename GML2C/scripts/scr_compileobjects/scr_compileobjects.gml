function scr_compileobjects(){
	var safe_name = sanitize_filename(yyfile.name);
	var file = file_text_open_write(destination + "source\\objects\\" + safe_name + ".c");
    show_debug_message("Object: " + yyfile.name);
            
    var spr_name = "";
            
    if (variable_struct_exists(yyfile, "spriteId")) {
        var sid = yyfile.spriteId;
        if (is_struct(sid) && variable_struct_exists(sid, "name"))
            spr_name = sid.name;
    }
            
    var steppath = filename_dir(filename_dir(global.selected_yyp) + "/" + _id.path) + "/Step_0.gml";
    var createpath = filename_dir(filename_dir(global.selected_yyp) + "/" + _id.path) + "/Create_0.gml";
            
    var create_code = "";
    var step_code = "";
            
    if (file_exists(createpath)) {
        var createinside_buffer = buffer_load(createpath);
		if (buffer_get_size(createinside_buffer) > 0)
			create_code = buffer_read(createinside_buffer, buffer_string);
        buffer_delete(createinside_buffer);
        show_debug_message(create_code);
    }
            
    if (file_exists(steppath)) {
        var createinside_buffer = buffer_load(steppath);
		if (buffer_get_size(createinside_buffer) > 0)
			step_code = buffer_read(createinside_buffer, buffer_string);
        buffer_delete(createinside_buffer);
        show_debug_message(step_code);
    }
            
    array_push(all_objects, {
        name: yyfile.name,
        sprite: spr_name,
        CreateCode: create_code,
        StepCode: step_code
    });
			
	//basic c structure
	file_text_write_string(file, "// Object: " + yyfile.name + "\n\n");

	file_text_write_string(file, "void " + safe_name + "_create() {\n");
	file_text_write_string(file, create_code + "\n");
	file_text_write_string(file, "}\n\n");

	file_text_write_string(file, "void " + safe_name + "_step() {\n");
	file_text_write_string(file, step_code + "\n");
	file_text_write_string(file, "}\n");

	file_text_close(file);
}