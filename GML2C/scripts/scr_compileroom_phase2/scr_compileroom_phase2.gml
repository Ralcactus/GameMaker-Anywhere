function scr_compileroom_phase2(packed_layers){
	var safe_name = sanitize_filename(yyfile.name);
	var roomhfile = file_text_open_write(destination + "source/rooms/" + safe_name + ".h");
	file_text_write_string(roomhfile, 
		"void scr_runroom_" + yyfile.name +"();"
	)
	file_text_close(roomhfile)
	show_debug_message("close file");

	//i don't think this works with more then 1 asset layer!
	var asset_layer_index = [];
	for (var k = 0; k < array_length(packed_layers); k++){
	    if (packed_layers[k].type == "GMRAssetLayer"){
			array_push(asset_layer_index, k);
	    }
	}
	show_debug_message("asset layer");
	
	//i don't think this works with more then 1 bg layer!
	var bg_layer_index = -1;
	for (var k = 0; k < array_length(packed_layers); k++){
	    if (packed_layers[k].type == "GMRBackgroundLayer"){
	        bg_layer_index = k;
	        break;
	    }
	}
	show_debug_message("bg layer: " + string(bg_layer_index));

	var object_scripts = "";
	var unique_objects = {};
	var got_funcs = "";
	var object_layer_index;
		
	for (var k = 0; k < array_length(packed_layers); k++){
		if (packed_layers[k].type == "GMRInstanceLayer"){
			var instances = packed_layers[k].instances;
			object_layer_index = k;
			
			for (var j = 0; j < array_length(instances); j++){
				//add event scripts
				var datastruct = safe_name + "_inst_" + string(object_layer_index) + "_data[" + string(j) + "]";
				object_scripts += "	" + instances[j].object + "_runevents(" + 
				datastruct + ".x, " + datastruct + ".y, " + //x y
				datastruct + ".scaleX, " + datastruct + ".scaleY, " + //xscale yscale
				datastruct + ".id" + ");\n" //id
				
				//get the functions for actual real
				if (!variable_struct_exists(unique_objects, instances[j].object)){
					unique_objects[$ instances[j].object] = true;
					got_funcs += "extern void " + instances[j].object + "_runevents(float, float, float, float, float);\n";
					got_funcs += "extern void " + instances[j].object + "_reset_frame();\n";
					object_scripts = "\t" + instances[j].object + "_reset_frame();\n" + object_scripts;
				}
			}
		}
	}
	show_debug_message("instance layers: " + string(object_layer_index));

	var background_layer = safe_name + "_bg_" + string(bg_layer_index);
	
	var assetdraw_loop = [];
	var bg_color_applycode = "";
	
	for (var i = 0; i < array_length(asset_layer_index); ++i) {
		var asset_layer = safe_name + "_asset_" + string(i);
		array_push(assetdraw_loop,  "	for (int i = 0; i < " + asset_layer + ".assetCount; i++)\n" +
						"		draw_sprite_ext(" + asset_layer  + "_data" + "[i].sprite, 0, " + asset_layer  + "_data" + "[i].x, " + asset_layer  + "_data" + "[i].y, " + asset_layer  + "_data" + "[i].scaleX, " + asset_layer  + "_data" + "[i].scaleY, " + asset_layer  + "_data" + "[i].rotation, 0, 1);\n\n");
	}
	
	if (bg_layer_index != -1){
		bg_color_applycode = "	bgcolor = " + background_layer + ".color;\n";
	}
	
	show_debug_message("write to room c file: " + yyfile.name);

	//write to the end of the room c file
	var roomcfile = file_text_open_append(destination + "source/rooms/" + safe_name + ".cpp");
	//sorry for lack of comments in this chunk, but at least it doesnt crash now?
	file_text_write_string(roomcfile, "\n" + got_funcs + "\n");
	file_text_write_string(roomcfile, "void scr_runroom_" + yyfile.name + "(){\n");
	file_text_write_string(roomcfile, "   //printf(\"RUNNING ROOM " + yyfile.name + "\\n\");\n\n");
	
	for (var i = 0; i < array_length(assetdraw_loop); ++i) {
		file_text_write_string(roomcfile, assetdraw_loop[i]);
	}
	
	file_text_write_string(roomcfile, "   if (" + yyfile.name + "_views[0].visible == 1){\n");
	file_text_write_string(roomcfile, "       view0_camWidth = " + yyfile.name + "_views[0].camWidth;\n");
	file_text_write_string(roomcfile, "       view0_camHeight = " + yyfile.name + "_views[0].camHeight;\n");
	file_text_write_string(roomcfile, "   }\n   else{\n");
	file_text_write_string(roomcfile, "       view0_camWidth = " + yyfile.name+"_INFO" + ".width;\n");
	file_text_write_string(roomcfile, "       view0_camHeight = " + yyfile.name+"_INFO" + ".height;\n");
	file_text_write_string(roomcfile, "   }\n\n");
	file_text_write_string(roomcfile, "   room_width = " + yyfile.name+"_INFO" + ".width;\n");
	file_text_write_string(roomcfile, "   room_height = " + yyfile.name+"_INFO" + ".height;\n");
	file_text_write_string(roomcfile, bg_color_applycode);
	file_text_write_string(roomcfile, object_scripts);
	file_text_write_string(roomcfile, "}\n");
	show_debug_message("close file: " + yyfile.name);
	file_text_close(roomcfile)
		
}