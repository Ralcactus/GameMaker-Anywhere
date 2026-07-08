function scr_compilesprites(){
	var safe_name = sanitize_filename(yyfile.name);
	
    show_debug_message("Sprite: " + yyfile.name);
    var spriteoutputDIR = destination + "/gfx/";

    var sprite_rel_dir = "sprites/" + yyfile.name + "/";
    var sprite_frames = [];

    for (var f = 0; f < array_length(yyfile.frames); f++) {
        var frame_name = yyfile.frames[f].name;
        array_push(sprite_frames, frame_name);
		currentsprite_count++;

		show_debug_message("Copying sprite frame: " + frame_name);
		file_copy(filename_dir(global.selected_yyp) + "/sprites/" + yyfile.name + "/" + frame_name + ".png", spriteoutputDIR + frame_name + ".png");
		show_debug_message("Copied!");
		
		show_debug_message("Saving sprite info...");
		scr_savesprite_info(frame_name);
		show_debug_message("Saved!");
		
		//T3S sprite list
		if (global.export_mode == "3DSX" || global.export_mode == "CIA")
			file_text_write_string(t3s_file, frame_name + ".png\n");

		//SCF sprite list
		if (global.export_mode == "GAMECUBE" || global.export_mode == "WII")
			file_text_write_string(textures_dolfile, "<filepath=" + frame_name + ".png" + " id=\"" + yyfile.name + "FSDSDFFDGIOJHDFIOHEFAMILYGUY2DDD"+string(irandom(100))+string(f) + "\" colfmt=6 />\n"); //for the sprite name, were just gonna use the custom defines so i just made it strange
    }
	currentsprite_count--;
	
	var init_sprites = file_text_open_append(destination + "source/helpers/init_sprites.h");	
	file_text_write_string(init_sprites, "#define " + safe_name + " " + string(currentsprite_count) + "\n");	
	file_text_close(init_sprites);
	
    currentsprite_count++;
}

function scr_savesprite_info(frame_name){
	var sprite = sprite_add(filename_dir(global.selected_yyp) + "/sprites/" + yyfile.name + "/" + frame_name + ".png", 0, false, false, 0, 0);
	var sprwidth = sprite_get_width(sprite);
	var sprheight = sprite_get_height(sprite);
	sprite_delete(sprite);	
	
	array_push(global.SpriteWidths, sprwidth);
	array_push(global.SpriteHeights, sprheight);
	array_push(global.SpriteOriginX, yyfile.sequence.xorigin);
	array_push(global.SpriteOriginY, yyfile.sequence.yorigin);
	array_push(global.SpriteFrameCount, array_length(yyfile.frames)-1);
	array_push(global.SpriteAnimTimer, yyfile.sequence.playbackSpeed);
	array_push(global.SpriteAnimSpeedType, yyfile.sequence.playbackSpeedType);
	
	//collide box
	array_push(global.SpriteBoxTOP, yyfile.bbox_top);
	array_push(global.SpriteBoxBOTTOM, yyfile.bbox_bottom);
	array_push(global.SpriteBoxLEFT, yyfile.bbox_left);
	array_push(global.SpriteBoxRIGHT, yyfile.bbox_right);
}