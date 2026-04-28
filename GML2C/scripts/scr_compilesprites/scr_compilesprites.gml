function scr_compilesprites(){
	var safe_name = sanitize_filename(yyfile.name);
	var sprwidth = 0;
	var sprheight = 0;
	
	if (array_contains(banned_strings, safe_name)){
		var answer = show_question("WARNING\nBanned string \"" + safe_name + "\" continue compiling this sprite? (will cause issues!)")
		
		if (answer = false){
			global.exporting = false;
			return true; //cancel compile
		}
	}
	
	var file = file_text_open_write(destination + "source\\sprites\\" + safe_name + ".cpp");
    show_debug_message("Sprite: " + yyfile.name);
    var spriteoutput;
			
	if (global.export_mode == "GAMECUBE" || global.export_mode == "WII" || global.export_mode == "CIA" || global.export_mode == "3DSX")
		spriteoutput = destination + "\\gfx\\";

    var sprite_rel_dir = "sprites/" + yyfile.name + "/";
    var sprite_frames = [];
			
	// Write file header
	file_text_write_string(file, "#include \"../gml/structs.h\"\n\n");
	file_text_write_string(file, "// Sprite: " + yyfile.name + "\n\n");
			
	//Frames
	var frame_array_name = "spr_" + safe_name + "_frames";

	//Write array
	file_text_write_string(file, "static Frames " + frame_array_name + "[] = {\n");
            
    for (var f = 0; f < array_length(yyfile.frames); f++) {
        var frame_name = yyfile.frames[f].name;
        array_push(sprite_frames, frame_name);
				
		file_text_write_string(file,
		"    { \"" + frame_name + "\" },\n");
         
		if (global.copysprite){
			var sprite = sprite_add(filename_dir(global.selected_yyp) + "\\sprites\\" + yyfile.name + "\\" + frame_name + ".png", 0, false, false, 0, 0);
			sprwidth = sprite_get_width(sprite);
			sprheight = sprite_get_height(sprite);
			array_push(global.SpriteWidths, sprwidth);
			array_push(global.SpriteHeights, sprheight);
			array_push(global.SpriteOriginX, yyfile.sequence.xorigin);
			array_push(global.SpriteOriginY, yyfile.sequence.yorigin);
			sprite_delete(sprite);
			
			 
			file_copy(filename_dir(global.selected_yyp) + "\\sprites\\" + yyfile.name + "\\" + frame_name + ".png", spriteoutput + frame_name + ".png");
		}
		
		if (global.export_mode == "3DSX" || global.export_mode == "CIA")
			file_text_write_string(t3s_file, frame_name + ".png\n");

		if (global.export_mode == "GAMECUBE" || global.export_mode == "WII")
			file_text_write_string(textures_dolfile, "<filepath=" + frame_name + ".png" + " id=\"" + yyfile.name + "FSDSDFFDGIOJHDFIOHEFAMILYGUY2DDD" + "\" colfmt=6 />\n"); //for the sprite name, were just gonna use the custom defines so i just made it strange
    }
	
	file_text_write_string(file, "};\n\n");

    array_push(all_sprites, {
        name: yyfile.name,
        dir: sprite_rel_dir,
        frames: sprite_frames,
        width: yyfile.width,
        height: yyfile.height,
        origin: yyfile.origin,
        xorigin: yyfile.sequence.xorigin,
        yorigin: yyfile.sequence.yorigin,
        SpriteNumber: currentsprite_count
    });

	var bboxt = "0";
	var colType = "0";

	if (yyfile.bboxMode == 0) bboxt = "AUTOMATIC";
	else if (yyfile.bboxMode == 1) bboxt = "FULL_IMAGE";
	else bboxt = "MANUAL";
			
	if (yyfile.collisionKind == 0) colType = "PRECISE";
	if (yyfile.collisionKind == 1) colType = "RECTANGLE";

	//Sprite thingie
	file_text_write_string(file,
	"GMSprite spr_" + safe_name + " = {\n" +
	"    .location = \"sprites/" + yyfile.name + "/\",\n" +
	"    .width = " + string(yyfile.width) + ",\n" +
	"    .height = " + string(yyfile.height) + ",\n" +
	"    .originX = " + string(yyfile.sequence.xorigin) + ",\n" +
	"    .originY = " + string(yyfile.sequence.yorigin) + ",\n" +
	"    .fps = " + string(yyfile.sequence.playbackSpeed) + ",\n" +
	"    .frameSpd = " + string(yyfile.sequence.playbackSpeedType) + ",\n" +
	"    .maskLeft = " + string(yyfile.bbox_left) + ",\n" +
	"    .maskTop = " + string(yyfile.bbox_top) + ",\n" +
	"    .maskRight = " + string(yyfile.bbox_right) + ",\n" +
	"    .maskBottom = " + string(yyfile.bbox_bottom) + ",\n" +
	"    .type = " + colType + ",\n" +
	"    .bboxType = " + bboxt + ",\n" +
	"    .frames = " + frame_array_name + ",\n" +
	"};\n");
			
	file_text_close(file);
            
	var spriteidh = file_text_open_append(destination + "source\\sprite_toid.h");
	file_text_write_string(spriteidh, "#define " + yyfile.name + " " + string(currentsprite_count) + "\n");
	file_text_close(spriteidh);
	
    currentsprite_count++;
}