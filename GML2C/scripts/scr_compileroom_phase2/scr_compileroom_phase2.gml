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
				datastruct + ".scaleX, " + datastruct + ".scaleY" + ");\n" //xscale yscale
				
				//get the functions for actual real
				if (!variable_struct_exists(unique_objects, instances[j].object)){
					unique_objects[$ instances[j].object] = true;
					got_funcs += "extern void " + instances[j].object + "_runevents(float, float, float, float);\n";
					got_funcs += "extern void " + instances[j].object + "_reset_frame();\n";
					object_scripts = "\t" + instances[j].object + "_reset_frame();\n" + object_scripts;
				}
			}
		}
	}
	
	var asset_layer = safe_name + "_asset_" + string(asset_layer_index);

	//write to the end of the room c file
	var roomcfile = file_text_open_append(destination + "source\\rooms\\" + safe_name + ".c");
	file_text_write_string(roomcfile,
		//got the funcs
		"\n" + got_funcs + "\n" +
		
		//the actual func!
		"void scr_runroom_" + yyfile.name +"(){\n" +
		"	printf(\"RUNNING ROOM " + yyfile.name + "\\n\");\n\n" +
		
		//draw assets
		"	for (int i = 0; i < " + asset_layer + ".assetCount; i++)\n" +
		"		draw_sprite_ext(" + asset_layer  + "_data" + "[i].sprite, 0, " + asset_layer  + "_data" + "[i].x, " + asset_layer  + "_data" + "[i].y, " + asset_layer  + "_data" + "[i].scaleX, " + asset_layer  + "_data" + "[i].scaleY, " + asset_layer  + "_data" + "[i].rotation, 0, 1);\n\n" +
		
		//room sizes
		"	if (" + yyfile.name + "_views[0].visible == 1){\n" +
		"		view0_camWidth = " + yyfile.name + "_views[0].camWidth;\n" +
		"		view0_camHeight = " + yyfile.name + "_views[0].camHeight;\n" +
		"	}\n" +
		"	else{\n" +
		"		view0_camWidth = " + yyfile.name + ".width;\n" +
		"		view0_camHeight = " + yyfile.name + ".height;\n" +
		"	}\n\n" +
		
		
		//objects
		object_scripts +
		"}\n"
	);
	file_text_close(roomcfile)
	
}