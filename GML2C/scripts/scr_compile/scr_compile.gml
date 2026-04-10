function sanitize_filename(name) {
    var invalid = ["/", "\\", ":", "*", "?", "\"", "<", ">", "|"];
    
    for (var i = 0; i < array_length(invalid); i++) {
        name = string_replace_all(name, invalid[i], "_");
    }
    
    return name;
}

function scr_compileSETUP(){
	//enviorment variable checks
	if (environment_get_variable("DEVKITPRO") == ""){
		show_message("DevKitPro is not installed!");
		exit;
	}
	
	if (!directory_exists("C:\\devkitPro\\portlibs\\3ds\\") && (global.export_mode == Export.CIA || global.export_mode == Export._3DSX)){
		show_message("DevKitPro 3DS portlibs are not installed!");
		exit;
	}

    global.exporting = true;
	
	if (directory_exists(destination)){
		//delete the old build
	    delete_powershell_window = run_commandpowershell("C:\\", "Remove-Item -LiteralPath 'C:\\GM_Anywhere' -Recurse -Force -ErrorAction SilentlyContinue");
		scr_compileMIDDLE1();
	}
	else{
		//copy the runner to the compile folder on the C drive
		copy_powershell_window = run_commandpowershell("C:\\", "Copy-Item -Path \"" + runnerfolder + " -Destination \"" + destination + " -Recurse");
		scr_compileMIDDLE2();
	}
}

//delete old builds
function scr_compileMIDDLE1(){
	if (CompletionStatusFromExecutedProcess(delete_powershell_window)){
		//copy the runner to the compile folder on the C drive
		copy_powershell_window = run_commandpowershell("C:\\", "Copy-Item -Path \"" + runnerfolder + " -Destination \"" + destination + " -Recurse");
		scr_compileMIDDLE2();
		exit;
	}
	else
		scr_compileMIDDLE1();
}

//copy runner
function scr_compileMIDDLE2(){
	if (CompletionStatusFromExecutedProcess(copy_powershell_window))
		scr_compile();
	else
		scr_compileMIDDLE2();
}

//This is the script that creates the data.win, copys sprites, and makes the t3s for compiling
function scr_compile()
{
	// create sprites.t3s
	directory_create(destination + "\\gfx\\");
	var t3s_path = destination + "\\gfx\\" + "sprites.t3s";
	var t3s_file = file_text_open_write(t3s_path);
	file_text_write_string(t3s_file, "--atlas\n");

    // json parse
    var yypbuffer = buffer_load(global.selected_yyp);
    var yypdata = buffer_read(yypbuffer, buffer_string);
    buffer_delete(yypbuffer);
    var yyp_json = json_parse(yypdata);
    
    // get the first room
    var first_room_name = yyp_json.RoomOrderNodes[0].roomId.name;
    var first_room_path = yyp_json.RoomOrderNodes[0].roomId.path;
    
    // get all resources
    var all_resources = [];
    var all_rooms = [];
    var all_sprites = [];
    var all_objects = [];
    
    var currentsprite_count = 0;
    
    for (var i = 0; i < array_length(yyp_json.resources); i++) {
        var _id = yyp_json.resources[i].id;
        array_push(all_resources, {name: _id.name});
        
        var file_buffer = buffer_load(filename_dir(global.selected_yyp) + "/" + _id.path);
        var yyfileFAKE = buffer_read(file_buffer, buffer_string);
        buffer_delete(file_buffer);
        yyfileFAKE = string_replace_all(yyfileFAKE, ",}", "}");
        yyfileFAKE = string_replace_all(yyfileFAKE, ",]", "]");
        var yyfile = json_parse(yyfileFAKE);
        
        var IsRoom = (yyfile.resourceType == "GMRoom");
        var IsSprite = (yyfile.resourceType == "GMSprite");
        var IsObject = (yyfile.resourceType == "GMObject");
        
		directory_create(destination + "\\rooms\\");
		if (IsRoom) {

		    var safe_name = sanitize_filename(yyfile.name);
		    var filepath = destination + "\\rooms\\" + safe_name + ".c";
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
		    file_text_write_string(file, "#include \"../gml/structs.h\"\n\n");
		    file_text_write_string(file, "// Room: " + yyfile.name + "\n\n");

		    // Write bg data
		    for (var k = 0; k < array_length(packed_layers); k++) {
		        var _layer = packed_layers[k];

		        if (_layer.type == "GMRBackgroundLayer" && variable_struct_exists(_layer, "background")) {
		            var cname = safe_name + "_bg_" + string(k);

		            file_text_write_string(file,
		            "static GMLayerBackground " + cname + " = {\n" +
		            "    .sprite = " + (_layer.background.sprite != "" ? "&spr_" + sanitize_filename(_layer.background.sprite) : "NULL") + ",\n" +
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
		                    ", .object=&obj_" + objname +
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
		                    ", .sprite=&spr_" + sanitize_filename(asset.sprite) +
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
		}
        
		directory_create(destination + "\\sprites\\");
        if (IsSprite) {
			var safe_name = sanitize_filename(yyfile.name);
			var file = file_text_open_write(destination + "\\sprites\\" + safe_name + ".c");
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
        
		directory_create(destination + "\\objects\\");
        if (IsObject) {
			var safe_name = sanitize_filename(yyfile.name);
			var file = file_text_open_write(destination + "\\objects\\" + safe_name + ".c");
            show_debug_message("Object: " + yyfile.name);
            
            var spr_name = "";
            
            if (variable_struct_exists(yyfile, "spriteId")) {
                var sid = yyfile.spriteId;
                if (is_struct(sid) && variable_struct_exists(sid, "name"))
                    spr_name = sid.name;
            }
            
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
			
			//basic c structure
			file_text_write_string(file, "// Object: " + yyfile.name + "\n\n");

			file_text_write_string(file, "void " + safe_name + "_create() {\n");
			file_text_write_string(file, create_code + "\n");
			file_text_write_string(file, "}\n\n");

			file_text_write_string(file, "void " + safe_name + "_step() {\n");
			file_text_write_string(file, step_code + "\n");
			file_text_write_string(file, "}\n");

			file_text_close(file);
			
        }
    }
	
    file_text_close(t3s_file);
	
	//create the 3ds app info
	var appinfo = file_text_open_write(destination + "\\resources\\AppInfo");
	file_text_write_string(appinfo, "APP_TITLE = " + global.game_name + "\n"
	+ "APP_DESCRIPTION = " + "Created with GameMaker Anywhere!\n"
	+ "APP_AUTHOR = " + global.publisher + "\n"
	+ "APP_PRODUCT_CODE = " + "Your-Code\n"
	+ "APP_UNIQUE_ID = " +"0x" + global.title_id + "\n"
	+ "APP_VERSION_MAJOR = " + "1\n"
	+ "APP_VERSION_MINOR = " + "0\n"
	+ "APP_VERSION_MICRO = " + "0\n");
	file_text_close(appinfo);
	
	//copy icon
	if (global.iconpath != ""){
		file_delete(destination + "\\resources\\icon.png")
        file_copy(global.iconpath, destination + "\\resources\\icon.png");
	}
	
	//3ds cia
	/*if (global.export_mode == 0)
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make cia");
			
	//3ds 3dsx
	if (global.export_mode == 1)
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make 3dsx");
					
	//windows exe
	if (global.export_mode == 2)
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make win");
	*/
	//finsih!!!
	logging = true;
	global.exporting = false;
	//show_message("Compiling now!\nCheck " + destination + "\\output\\ " + " For the rom!")
}