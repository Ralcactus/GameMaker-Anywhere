function scr_compileobjects(){
	var safe_name = sanitize_filename(yyfile.name);
	var file = file_text_open_write(destination + "source\\objects\\" + safe_name + ".c");
    show_debug_message("Object: " + yyfile.name);
            
    var spr_name = "";
            
    if (variable_struct_exists(yyfile, "spriteId")){
        var sid = yyfile.spriteId;
        if (is_struct(sid) && variable_struct_exists(sid, "name"))
            spr_name = sid.name;
    }

	//sprite isn't set (see the 2nd comment in sprite_toid to see why its so long)
	if (spr_name == "")
		spr_name = "JUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHIJUOEDESBGFIUFDSVIUFSDVBSAOPDSFIGHSDUIOGFSDIUFDSGFDSIUPFDSIFSDOPFSDAFSDASPFADOPSJSDJOIHFSDISFDOHI";
          
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
		
	file_text_close(file);
	
	scr_compileobject_phase2(spr_name, create_code, step_code);
	
	var spriteidh = file_text_open_append(destination + "source\\sprite_toid.h");
	file_text_write_string(spriteidh, "#define " + yyfile.name + " " + string(0) + "\n");
	file_text_close(spriteidh);
}