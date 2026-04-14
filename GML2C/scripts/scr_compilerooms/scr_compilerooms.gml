function scr_compilerooms(i){
	var safe_name = sanitize_filename(yyfile.name);
	var filepath = destination + "source\\rooms\\" + safe_name + ".c";
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

		// Background layer
		if (_layer.resourceType == "GMRBackgroundLayer") {
		    layer_out.background = {
		        colour: _layer.colour,
		        sprite: (_layer.spriteId != undefined && _layer.spriteId != -4 ? _layer.spriteId.name : "")
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
	file_text_write_string(file, "#include \"../sprite_toid.h\"\n");
	file_text_write_string(file, "#include \"../cross_platform/drawing.h\"\n\n");

	// Write bg data
	for (var k = 0; k < array_length(packed_layers); k++) {
		var _layer = packed_layers[k];

		if (_layer.type == "GMRBackgroundLayer" && variable_struct_exists(_layer, "background")) {
		    var cname = safe_name + "_bg_" + string(k);

		    file_text_write_string(file,
		    "static GMLayerBackground " + cname + " = {\n" +
		    "    .sprite = " + (_layer.background.sprite != "" ? "spr_" + sanitize_filename(_layer.background.sprite) : "NULL") + ",\n" +
		    "    .color = " + string(_layer.background.colour) + ",\n" +
		    "};\n\n");
		}
	}

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

		            file_text_write_string(file,
		            "    { .x=" + string(inst.x) +
		            ", .y=" + string(inst.y) +
		            ", .rotation=" + string(inst.rotation) +
		            ", .scaleX=" + string(inst.scaleX) +
		            ", .scaleY=" + string(inst.scaleY) +
		            ", .object=" + objname +
		            " },\n");
		        }
		    }

		    file_text_write_string(file, "};\n\n");

		    var count = (variable_struct_exists(_layer, "instances") ? array_length(_layer.instances) : 0);

		    file_text_write_string(file,
		    "static GMLayerInstance " + cname + " = {\n" +
		    "    .instances = " + cname + "_data,\n" +
		    "    .instanceCount = " + string(count) + "\n" +
		    "};\n\n");
		}
	}
			
	// Write asset data
	for (var k = 0; k < array_length(packed_layers); k++) {
		var _layer = packed_layers[k];

		if (_layer.type == "GMRAssetLayer") {
		    var cname = safe_name + "_asset_" + string(k);

		    file_text_write_string(file, "static LayerAssets " + cname + "_data[] = {\n");

		    if (variable_struct_exists(_layer, "assets") && is_array(_layer.assets)) {
		        for (var j = 0; j < array_length(_layer.assets); j++) {
		            var asset = _layer.assets[j];

		            file_text_write_string(file,
		            "    { .x=" + string(asset.x) +
		            ", .y=" + string(asset.y) +
		            ", .rotation=" + string(asset.rotation) +
		            ", .scaleX=" + string(asset.scaleX) +
		            ", .scaleY=" + string(asset.scaleY) +
		            ", .sprite=" + sanitize_filename(asset.sprite) +
		            " },\n");
		        }
		    }

		    file_text_write_string(file, "};\n\n");

		    var count = (variable_struct_exists(_layer, "assets") ? array_length(_layer.assets) : 0);

		    file_text_write_string(file,
		    "static GMLayerAsset " + cname + " = {\n" +
		    "    .assets = " + cname + "_data,\n" +
		    "    .assetCount = " + string(count) + "\n" +
		    "};\n\n");
		}
	}

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

		file_text_write_string(file,
		    "    { " + ctype + ", " + string(_layer.depth) + ", " + dataref + " },\n"
		);
	}

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

	for (var k = 0; k < view_count; k++) {
		var v = yyfile.views[k];

		//Safe object reference
		var objref = "NULL";
		if (variable_struct_exists(v, "objectId") && is_struct(v.objectId) && variable_struct_exists(v.objectId, "name")) {
			objref = "&obj_" + sanitize_filename(v.objectId.name);
		}

		file_text_write_string(file,
		"    {\n" +
		"        .camXPos=" + string(v.xview) + ",\n" +
		"        .camYPos=" + string(v.yview) + ",\n" +
		"        .camWidth=" + string(v.wview) + ",\n" +
		"        .camHeight=" + string(v.hview) + ",\n" +
		"        .viewXPos=" + string(v.xport) + ",\n" +
		"        .viewYPos=" + string(v.yport) + ",\n" +
		"        .viewWidth=" + string(v.wport) + ",\n" +
		"        .viewHeight=" + string(v.hport) + ",\n" +
		"        .objFHBorder=" + string(v.hborder) + ",\n" +
		"        .objFVBorder=" + string(v.vborder) + ",\n" +
		"        .hSpd=" + string(v.hspeed) + ",\n" +
		"        .vSpd=" + string(v.vspeed) + ",\n" +
		"        .inherit=" + string(v.inherit) + ",\n" +
		"        .visible=" + string(v.visible) + ",\n" +
		"        .object=" + objref + "\n" +
		"    },\n");
	}

	file_text_write_string(file, "};\n\n");

	var firstroom = false;
	if (!KnownFirstRoom){
		firstroom = true;
		KnownFirstRoom = true;
		
		//write the first room into main.c
		var mainc = file_text_open_read(destination + "source\\main.c");
		var lines = [];
		var j = 0;
		
		while (!file_text_eof(mainc))
		    lines[j++] = file_text_readln(mainc);

		lines = string_split(string_replace_all(string_join_ext("", lines), "CurrentRoom = \"NULL\"", "CurrentRoom = \"" + safe_name + "\""), "\n");
		file_text_close(mainc)

		var mainc_write = file_text_open_write(destination + "source\\main.c");
		for (var k = 0; k < array_length(lines); k++) {
		    file_text_write_string(mainc_write, lines[k]);
		    file_text_writeln(mainc_write);
		}
		
		file_text_close(mainc_write);
	}

	//WRITE ROOM STRUCT
	file_text_write_string(file,
	"GMRoom " + safe_name + " = {\n" +
	"    .id = " + string(i) + ",\n" +
	"    .name = \"" + yyfile.name + "\",\n" +
	"    .width = " + string(yyfile.roomSettings.Width) + ",\n" +
	"    .height = " + string(yyfile.roomSettings.Height) + ",\n" +
	"    .persistent = " + string(yyfile.roomSettings.persistent) + ",\n" +
	"    .inheritRmSettings = " + string(yyfile.roomSettings.inheritRoomSettings) + ",\n" +
	"    .enableViews = " + string(yyfile.viewSettings.enableViews) + ",\n" +
	"    .clearDisplayBuffer = " + string(yyfile.viewSettings.clearDisplayBuffer) + ",\n" +
	"    .clearViewBackground = " + string(yyfile.viewSettings.clearViewBackground) + ",\n" +
	"    .inheritViewSettings = " + string(yyfile.viewSettings.inheritViewSettings) + ",\n" +
	"    .views = " + view_array_name + ",\n" +
	"    .viewCount = " + string(view_count) + ",\n" +
	"    .inheritPhySettings = " + string(yyfile.physicsSettings.inheritPhysicsSettings) + ",\n" +
	"    .phyEnabled = " + string(yyfile.physicsSettings.PhysicsWorld) + ",\n" +
	"    .gravX = " + string(yyfile.physicsSettings.PhysicsWorldGravityX) + ",\n" +
	"    .gravY = " + string(yyfile.physicsSettings.PhysicsWorldGravityY) + ",\n" +
	"    .pix2met = " + string(yyfile.physicsSettings.PhysicsWorldPixToMetres) + ",\n" +
	"    .layers = " + safe_name + "_layers,\n" +
	"    .layerCount = sizeof(" + safe_name + "_layers) / sizeof(GMLayer)\n" +
	"};\n");
	file_text_close(file);


	//the code was getting big im putting the "run room code" in here! -Ralcactus
	scr_compileroom_phase2(packed_layers);


	//add entry to room handler and include it
	//read existing file
	var room_handlefile = file_text_open_read(destination + "source\\room_handler.c");
	var file_content = "";
	while (!file_text_eof(room_handlefile)) {
	    file_content += file_text_read_string(room_handlefile) + "\n";
	    file_text_readln(room_handlefile);
	}
	file_text_close(room_handlefile);

	var lines = string_split(file_content, "\n");

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
	var new_content = "";
	for (var j = 0; j < array_length(lines); j++) {
	    if (j == include_insert_at) {
	        new_content += "#include \"../rooms/" + safe_name + ".h\"\n";
	    }
	    if (j == insert_at) {
	        new_content += "    if (strcmp(CurrentRoom, \"" + yyfile.name + "\") == 0){\n";
	        new_content += "        scr_runroom_" + yyfile.name + "();\n";
	        new_content += "    }\n";
	    }
	    new_content += lines[j] + "\n";
	}

	//write back full file
	var room_handlefile_write = file_text_open_write(destination + "source\\room_handler.c");
	file_text_write_string(room_handlefile_write, new_content);
	file_text_close(room_handlefile_write);
}