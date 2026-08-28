function scr_compilerooms(i, yyp_json){
	var safe_name = sanitize_filename(yyfile.name);
	var filepath = destination + "source/rooms/" + safe_name + ".cpp";
	var file = file_text_open_write(filepath);

	show_debug_message("Generating ROOM: " + yyfile.name);
	show_debug_message("Output file: " + filepath);

	var packed_layers = [];

	// Pack layers
	for (var L = 0; L < array_length(yyfile.layers); L++) {
		var _layer = yyfile.layers[L];

		var layer_out = {
		    type: _layer.resourceType,
		    depth: (variable_struct_exists(_layer, "depth") ? _layer.depth : 0)
		};

		// Instance layer
		if (_layer.resourceType == "GMRInstanceLayer") {
		    layer_out.instances = [];

		    if (variable_struct_exists(_layer, "instances") && is_array(_layer.instances)) {
		        for (var j = 0; j < array_length(_layer.instances); j++) {
		            var inst = _layer.instances[j];
					
					if (inst.objectId == undefined)
						continue;
					
		            array_push(layer_out.instances, {
		                object: inst.objectId.name,
		                x: inst.x,
		                y: inst.y,
		                scaleX: inst.scaleX,
		                scaleY: inst.scaleY,
		                rotation: inst.rotation
		            });
		        }
		    }
		}
		show_debug_message("Packed layer: " + _layer.resourceType);

		// Asset layer
		if (_layer.resourceType == "GMRAssetLayer") {
            layer_out.assets = [];
            if (variable_struct_exists(_layer, "assets") && is_array(_layer.assets)) {
                for (var j = 0; j < array_length(_layer.assets); j++) {
                    var assetlayer = _layer.assets[j];
                            
                    if (assetlayer.resourceType == "GMRSpriteGraphic") {
                        array_push(layer_out.assets, {
                            type: assetlayer.resourceType,
                            sprite: assetlayer.spriteId.name,
                            x: assetlayer.x, 
                            y: assetlayer.y,
                            scaleX: assetlayer.scaleX, scaleY: assetlayer.scaleY,
                            rotation: assetlayer.rotation
                        });
                    }
                }
            }
        }
		show_debug_message("asset layer: " + _layer.resourceType);
		// Background layer
		if (_layer.resourceType == "GMRBackgroundLayer") {
		    layer_out.background = {
		        colour: _layer.colour,
		        sprite: (_layer.spriteId != undefined  && _layer.spriteId != -4 ? _layer.spriteId.name : "")
		    };
		}

		array_push(packed_layers, layer_out);
	}
			
		array_push(all_rooms, {
        name: yyfile.name,
        roomwidth: yyfile.roomSettings.Width,
        roomheight: yyfile.roomSettings.Height,
		viewsenabled: yyfile.viewSettings.enableViews,
		viewport0_width: yyfile.views[0].wview,
		viewport0_height: yyfile.views[0].hview,
        layers: packed_layers

    });

	// Write file header
	file_text_write_string(file, "// Room: " + yyfile.name + "\n\n");
	file_text_write_string(file, "#include \"../gml/structs.h\"\n");
	file_text_write_string(file, "#include <stdbool.h>\n");
	file_text_write_string(file, "#include <stdio.h>\n");
	file_text_write_string(file, "#include <string.h>\n");
	file_text_write_string(file, "#include \"../helpers/asset_toid.h\"\n");
	file_text_write_string(file, "#include \"../gm_funcs/drawing.h\"\n");
	file_text_write_string(file, "#include \"../gm_funcs/misc.h\"\n");
	file_text_write_string(file, "#include \"../gm_funcs/audio.h\"\n");
	file_text_write_string(file, "#include \"../gm_funcs/filesystem.h\"\n");
	file_text_write_string(file, "#include \"../helpers/asset_toid.h\"\n\n");

	show_debug_message("write file header: " + yyfile.name);

	// Write bg data
	for (var k = 0; k < array_length(packed_layers); k++) {
		var _layer = packed_layers[k];

		if (_layer.type == "GMRBackgroundLayer" && variable_struct_exists(_layer, "background")) {
		    var cname = safe_name + "_bg_" + string(k);

		    file_text_write_string(file, "static GMLayerBackground " + cname + " = {\n");
			file_text_write_string(file, (_layer.background.sprite != "" ? "(GMSprite *)" + sanitize_filename(_layer.background.sprite) : "NULL") + ",\n");
			file_text_write_string(file, string(_layer.background.colour) + "\n");
			file_text_write_string(file, "};\n\n");
		}
	}
	show_debug_message("write bg data: " + yyfile.name);

	// Write instance data
	for (var k = 0; k < array_length(packed_layers); k++) {
		var _layer = packed_layers[k];

		if (_layer.type == "GMRInstanceLayer") {
		    var cname = safe_name + "_inst_" + string(k);

		    file_text_write_string(file, "static LayerInstances " + cname + "_data[] = {\n");

		    if (variable_struct_exists(_layer, "instances") && is_array(_layer.instances)) {
		        for (var j = 0; j < array_length(_layer.instances); j++) {
		            var inst = _layer.instances[j];
		            var objname = sanitize_filename(inst.object);

		            file_text_write_string(file, "    { ");
					file_text_write_string(file, string(inst.x) + ", ");
					file_text_write_string(file, string(inst.y) + ", ");
					file_text_write_string(file, string(inst.rotation) + ", ");
					file_text_write_string(file, string(inst.scaleX) + ", ");
					file_text_write_string(file, string(inst.scaleY) + ", ");
					file_text_write_string(file, objname + ", ");
					file_text_write_string(file, string(100000+object_count) + " },\n");
					
					object_count++;
					array_push(global.ObjectIdOBJECT, inst.object);
		        }
		    }

		    file_text_write_string(file, "};\n\n");

		    var count = (variable_struct_exists(_layer, "instances") ? array_length(_layer.instances) : 0);

		    file_text_write_string(file, "static GMLayerInstance " + cname + " = {\n");
			file_text_write_string(file, cname + "_data,\n");
			file_text_write_string(file, string(count) + "\n");
			file_text_write_string(file, "};\n\n");
		}
	}
	show_debug_message("write instance data: " + yyfile.name);
			
	// Write asset data
	for (var k = 0; k < array_length(packed_layers); k++) {
		var _layer = packed_layers[k];

		if (_layer.type == "GMRAssetLayer") {
		    var cname = safe_name + "_asset_" + string(k);

		    file_text_write_string(file, "static LayerAssets " + cname + "_data[] = {\n");

		    if (variable_struct_exists(_layer, "assets") && is_array(_layer.assets)) {
		        for (var j = 0; j < array_length(_layer.assets); j++) {
		            var asset = _layer.assets[j];

		            file_text_write_string(file, "    { ");
					file_text_write_string(file, string(asset.x) + ", ");
					file_text_write_string(file, string(asset.y) + ", ");
					file_text_write_string(file, string(asset.rotation) + ", ");
					file_text_write_string(file, string(asset.scaleX) + ", ");
					file_text_write_string(file, string(asset.scaleY) + ", ");
					file_text_write_string(file, sanitize_filename(asset.sprite) + " },\n");
		        }
		    }

		    file_text_write_string(file, "};\n\n");

		    var count = (variable_struct_exists(_layer, "assets") ? array_length(_layer.assets) : 0);

		    file_text_write_string(file, "static GMLayerAsset " + cname + " = {\n");
			file_text_write_string(file, cname + "_data,\n");
			file_text_write_string(file, string(count) + "\n");
			file_text_write_string(file, "};\n\n");
		}
	}
	show_debug_message("write asset data: " + yyfile.name);

	// Write layer array
	file_text_write_string(file, "static const GMLayer " + safe_name + "_layers[] = {\n");

	for (var k = 0; k < array_length(packed_layers); k++) {
		var _layer = packed_layers[k];

		var ctype = "0";
		var dataref = "NULL";

		if (_layer.type == "GMRBackgroundLayer") {
		    ctype = "LAYER_BACKGROUND";
		    dataref = "&" + safe_name + "_bg_" + string(k);
		}

		if (_layer.type == "GMRInstanceLayer") {
		    ctype = "LAYER_INSTANCE";
		    dataref = "&" + safe_name + "_inst_" + string(k);
		}
				
		if (_layer.type == "GMRAssetLayer") {
		    ctype = "LAYER_ASSET";
		    dataref = "&" + safe_name + "_asset_" + string(k);
		}

		file_text_write_string(file, "    { " + ctype + ", ");
		file_text_write_string(file, string(_layer.depth) + ", ");
		file_text_write_string(file, dataref + " },\n");
	}
	show_debug_message("write layer array: " + yyfile.name);

	file_text_write_string(file, "};\n\n");
			
	//Viewports
	var view_array_name = safe_name + "_views";

	//Views exist?
	var view_count = 0;
	if (variable_struct_exists(yyfile, "views") && is_array(yyfile.views)) {
		view_count = array_length(yyfile.views);
	}

	//Write array
	file_text_write_string(file, "static GMViewPorts " + view_array_name + "[] = {\n");
	var coma = ",";

	for (var k = 0; k < view_count; k++) {
		var v = yyfile.views[k];

		//Safe object reference
		var objref = "0";
		if (variable_struct_exists(v, "objectId") && is_struct(v.objectId) && variable_struct_exists(v.objectId, "name")) {
			objref = sanitize_filename(v.objectId.name);
		}
		
		if(k == view_count - 1) coma = "";

		file_text_write_string(file, "    {\n");
		file_text_write_string(file, string(v.xview) + ",\n");
		file_text_write_string(file, string(v.yview) + ",\n");
		file_text_write_string(file, string(v.wview) + ",\n");
		file_text_write_string(file, string(v.hview) + ",\n");
		file_text_write_string(file, string(v.xport) + ",\n");
		file_text_write_string(file, string(v.yport) + ",\n");
		file_text_write_string(file, string(v.wport) + ",\n");
		file_text_write_string(file, string(v.hport) + ",\n");
		file_text_write_string(file, string(v.hborder) + ",\n");
		file_text_write_string(file, string(v.vborder) + ",\n");
		file_text_write_string(file, string(v.hspeed) + ",\n");
		file_text_write_string(file, string(v.vspeed) + ",\n");
		file_text_write_string(file, string(v.inherit) + ",\n");
		file_text_write_string(file, string(v.visible) + ",\n");
		file_text_write_string(file, objref + "\n");
		file_text_write_string(file, "    }" + coma + "\n");
	}
	show_debug_message("write view array: " + yyfile.name);

	file_text_write_string(file, "};\n\n");
	var firstroom = false;
	if (yyp_json.RoomOrderNodes[0].roomId.name == safe_name){
		firstroom = true;
		
		//read main.cpp using buffer 
		var mainc_buf = buffer_load(destination + "source/main.cpp");
		var mainc_str = buffer_read(mainc_buf, buffer_string);
		buffer_delete(mainc_buf);
		
		mainc_str = string_replace_all(mainc_str, "room = -1", "room = " + safe_name);
		
		var mainc_write = file_text_open_write(destination + "source/main.cpp");
		file_text_write_string(mainc_write, mainc_str);
		file_text_close(mainc_write);
	}

	//WRITE ROOM STRUCT
	file_text_write_string(file, "GMRoom " + safe_name + "_INFO" + " = {\n");
	file_text_write_string(file, string(i) + ",\n");
	file_text_write_string(file, "\"" + yyfile.name + "\",\n");
	file_text_write_string(file, string(yyfile.roomSettings.Width) + ",\n");
	file_text_write_string(file, string(yyfile.roomSettings.Height) + ",\n");
	file_text_write_string(file, string(yyfile.roomSettings.persistent) + ",\n");
	file_text_write_string(file, string(yyfile.roomSettings.inheritRoomSettings) + ",\n");
	file_text_write_string(file, string(yyfile.viewSettings.enableViews) + ",\n");
	file_text_write_string(file, string(yyfile.viewSettings.clearDisplayBuffer) + ",\n");
	file_text_write_string(file, string(yyfile.viewSettings.clearViewBackground) + ",\n");
	file_text_write_string(file, string(yyfile.viewSettings.inheritViewSettings) + ",\n");
	file_text_write_string(file, view_array_name + ",\n");
	file_text_write_string(file, string(view_count) + ",\n");
	file_text_write_string(file, string(yyfile.physicsSettings.inheritPhysicsSettings) + ",\n");
	file_text_write_string(file, string(yyfile.physicsSettings.PhysicsWorld) + ",\n");
	file_text_write_string(file, string(yyfile.physicsSettings.PhysicsWorldGravityX) + ",\n");
	file_text_write_string(file, string(yyfile.physicsSettings.PhysicsWorldGravityY) + ",\n");
	file_text_write_string(file, string(yyfile.physicsSettings.PhysicsWorldPixToMetres) + ",\n");
	file_text_write_string(file, safe_name + "_layers,\n");
	file_text_write_string(file, "sizeof(" + safe_name + "_layers) / sizeof(GMLayer)\n");
	file_text_write_string(file, "};\n");
	file_text_close(file);
	show_debug_message("write room struct: " + yyfile.name);

	//the code was getting big im putting the "run room code" in here! -Ralcactus
	scr_compileroom_phase2(packed_layers);
	show_debug_message("run room phase2: " + yyfile.name);

	show_debug_message("CP: opening room_handler");
	var _rh_buf = buffer_load(destination + "source/room_handler.cpp");
	var _rh_str = buffer_read(_rh_buf, buffer_string);
	buffer_delete(_rh_buf);
	var lines = string_split(_rh_str, "\n");
	show_debug_message("CP: lines read=" + string(array_length(lines)));

	//find closing bracket
	var insert_at = -1;
	for (var j = array_length(lines) - 1; j >= 0; j--) {
	    if (string_trim(lines[j]) == "}") {
	        insert_at = j;
	        break;
	    }
	}

	//find last #include line
	var include_insert_at = -1;
	for (var j = 0; j < array_length(lines); j++) {
	    if (string_pos("#include", string_trim(lines[j])) == 1) {
	        include_insert_at = j;
	    }
	}

	//build new content
	var room_handlefile_write = file_text_open_write(destination + "source/room_handler.cpp");
	for (var j = 0; j < array_length(lines); j++) {
	    if (j == include_insert_at) {
	        file_text_write_string(room_handlefile_write, "#include \"../rooms/" + safe_name + ".h\"\n");
	    }
	    if (j == insert_at) {
	        file_text_write_string(room_handlefile_write, "    if (room == " + yyfile.name + "){\n");
	        file_text_write_string(room_handlefile_write, "        scr_runroom_" + yyfile.name + "();\n");
	        file_text_write_string(room_handlefile_write, "        return;\n");
	        file_text_write_string(room_handlefile_write, "    }\n");
	    }
	    file_text_write_string(room_handlefile_write, lines[j] + "\n");
	}
	file_text_close(room_handlefile_write);
	show_debug_message("write room handler: " + yyfile.name);
	
	var roomto_idh = file_text_open_append(destination + "source/helpers/asset_toid.h");
	file_text_write_string(roomto_idh, "#define " + yyfile.name + " " + string(roomid_count) + " //Room\n");
	roomid_count += 1;
	file_text_close(roomto_idh);
}