function scr_compilesprites(){
	var safe_name = sanitize_filename(yyfile.name);
	var sprwidth = 0;
	var sprheight = 0;
	
    show_debug_message("Sprite: " + yyfile.name);
    var spriteoutputDIR = destination + "/gfx/";

    var sprite_rel_dir = "sprites/" + yyfile.name + "/";
    var sprite_frames = [];

    for (var f = 0; f < array_length(yyfile.frames); f++) {
        var frame_name = yyfile.frames[f].name;
        array_push(sprite_frames, frame_name);

		show_debug_message("Copying sprite frame: " + frame_name);
		file_copy(filename_dir(global.selected_yyp) + "/sprites/" + yyfile.name + "/" + frame_name + ".png", spriteoutputDIR + frame_name + ".png");
		show_debug_message("Copied!");
		
		//T3S sprite list
		if (global.export_mode == "3DSX" || global.export_mode == "CIA")
			file_text_write_string(t3s_file, frame_name + ".png\n");

		//SCF sprite list
		if (global.export_mode == "GAMECUBE" || global.export_mode == "WII")
			file_text_write_string(textures_dolfile, "<filepath=" + frame_name + ".png" + " id=\"" + yyfile.name + "FSDSDFFDGIOJHDFIOHEFAMILYGUY2DDD" + "\" colfmt=6 />\n"); //for the sprite name, were just gonna use the custom defines so i just made it strange
    }

	var init_sprites = file_text_open_append(destination + "source/helpers/init_sprites.h");	
	file_text_write_string(init_sprites, "#define " + safe_name + " " + string(currentsprite_count) + "\n");	
	file_text_close(init_sprites);
	
    currentsprite_count++;
}