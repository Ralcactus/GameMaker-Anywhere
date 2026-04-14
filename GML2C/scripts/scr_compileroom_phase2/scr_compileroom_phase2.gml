function scr_compileroom_phase2(packed_layers){
	var safe_name = sanitize_filename(yyfile.name);
	var roomhfile = file_text_open_write(destination + "source\\rooms\\" + safe_name + ".h");
	file_text_write_string(roomhfile, 
		"void scr_runroom_" + yyfile.name +"();"
	)
	file_text_close(roomhfile)
	

	//i don't think this works with more then 1 asset layer!
	var asset_layer_index;
	for (var k = 0; k < array_length(packed_layers); k++){
	    if (packed_layers[k].type == "GMRAssetLayer"){
	        asset_layer_index = k;
	        break;
	    }
	}	

	var bleh = safe_name + "_asset_" + string(asset_layer_index);
	var bleh2 = bleh  + "_data";
	
	//write to the end of the room c file
	var roomcfile = file_text_open_append(destination + "source\\rooms\\" + safe_name + ".c");
	file_text_write_string(roomcfile,
		"void scr_runroom_" + yyfile.name +"(){\n" +
		"	printf(\"RUNNING ROOM " + yyfile.name + "\\n\");\n" +
		"	for (int i = 0; i < " + bleh + ".assetCount; i++)\n" +
		"		draw_sprite_ext(" + bleh2 + "[i].sprite, 0, " + bleh2 + "[i].x, " + bleh2 + "[i].y, " + bleh2 + "[i].scaleX, " + bleh2 + "[i].scaleY, " + bleh2 + "[i].rotation, 0, 1);\n" +
		"	if (" + yyfile.name + "_views[0].visible == 1){\n" +
		"		view0_camWidth = " + yyfile.name + "_views[0].camWidth;\n" +
		"		view0_camHeight = " + yyfile.name + "_views[0].camHeight;\n" +
		"	}\n" +
		"	else{\n" +
		"		view0_camWidth = " + yyfile.name + ".width;\n" +
		"		view0_camHeight = " + yyfile.name + ".height;\n" +
		"	}\n" +
		"}\n"
	);
	file_text_close(roomcfile)
	
}