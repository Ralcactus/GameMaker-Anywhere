function scr_compileobjects(){
	var safe_name = sanitize_filename(yyfile.name);
	var file = file_text_open_write(destination + "source/objects/" + safe_name + ".cpp");
    show_debug_message("Object: " + yyfile.name);
    var spr_name = "";
            
    if (variable_struct_exists(yyfile, "spriteId")){
        var sid = yyfile.spriteId;
        if (is_struct(sid) && variable_struct_exists(sid, "name"))
            spr_name = sid.name;
    }
	
	//object doesn't have a sprite
	if (spr_name == "")
		spr_name = "-1";
		
	if (array_contains(banned_strings, safe_name)){
		var answer = false;
		show_debug_message("WARNING\nBanned string \"" + safe_name + "\" continue compiling this object? (will cause issues!)")
		
		if (answer = false)
			exit;
	}
	
    var steppath = filename_dir(filename_dir(global.selected_yyp) + "/" + _id.path) + "/Step_0.gml";
    var createpath = filename_dir(filename_dir(global.selected_yyp) + "/" + _id.path) + "/Create_0.gml";
    var drawpath = filename_dir(filename_dir(global.selected_yyp) + "/" + _id.path) + "/Draw_0.gml";
            
    var create_code = "";
    var step_code = "";
	var draw_code = "";

	if (global.copycode == true){
	    if (file_exists(createpath)) {
			create_code = syntax_convert(create_code, createpath);
	        show_debug_message(create_code);

	    }
            
	    if (file_exists(steppath)) {
			step_code = syntax_convert(step_code, steppath);
	        show_debug_message(step_code);
	    }
                  
	    if (file_exists(drawpath)) {
			draw_code = syntax_convert(draw_code, drawpath);
	        show_debug_message(draw_code);
	    }
	}
	
	scr_write_customvariables(create_code);
	scr_write_customvariables(step_code);
	scr_write_customvariables(draw_code);
	
    array_push(all_objects, {
        name: yyfile.name,
        sprite: spr_name,
        CreateCode: create_code,
        StepCode: step_code,
		DrawCode: draw_code
    });
		
	file_text_close(file);
	
	scr_compileobject_phase2(spr_name, create_code, step_code, draw_code);
	currentobject_count--;
	
	var spriteidh = file_text_open_append(destination + "source/helpers/asset_toid.h");
	file_text_write_string(spriteidh, "#define " + yyfile.name + " " + string(currentobject_count) + " //Object\n");
	file_text_close(spriteidh);
	
    currentobject_count++;
}