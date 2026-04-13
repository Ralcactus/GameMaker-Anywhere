function scr_compilesprites(){
	var safe_name = sanitize_filename(yyfile.name);
	var file = file_text_open_write(destination + "source\\sprites\\" + safe_name + ".c");
    show_debug_message("Sprite: " + yyfile.name);
    var spriteoutput;
			
	if (global.export_mode == Export.CIA || global.export_mode == Export._3DSX)
	    spriteoutput = destination + "\\gfx\\";
				
	if (global.export_mode == Export.EXE)
		spriteoutput = destination + "\\output\\sprites\\";
				
	if (global.export_mode == Export.DC)
		spriteoutput = destination + "\\romdisk\\sprites\\";
				
    directory_create(spriteoutput + yyfile.name + "\\");
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
                
        file_copy(filename_dir(global.selected_yyp) + "\\sprites\\" + yyfile.name + "\\" + frame_name + ".png", 
                    spriteoutput + yyfile.name + "\\" + frame_name + ".png");
						 
        file_text_write_string(t3s_file, yyfile.name + "/" + frame_name + ".png\n");
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
            
    currentsprite_count++;
}