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
        
        if (IsRoom) {
            show_debug_message("Room: " + yyfile.name);
            
            var packed_layers = [];
            
            for (var L = 0; L < array_length(yyfile.layers); L++) {
                var _layer = yyfile.layers[L];
                
                var layer_out = {
                    type: _layer.resourceType,
                    name: _layer.name,
                    depth: (variable_struct_exists(_layer, "depth") ? _layer.depth : 0),
                    visible: (variable_struct_exists(_layer, "visible") ? _layer.visible : true),
                };
                
                if (_layer.resourceType == "GMRInstanceLayer") {
                    layer_out.instances = [];
                    if (variable_struct_exists(_layer, "instances") && is_array(_layer.instances)) {
                        for (var j = 0; j < array_length(_layer.instances); j++) {
                            var instlayer = _layer.instances[j];
                            array_push(layer_out.instances, {
                                name: instlayer.name,
                                object: instlayer.objectId.name,
                                x: instlayer.x, y: instlayer.y,
                                scaleX: instlayer.scaleX, scaleY: instlayer.scaleY,
                                rotation: instlayer.rotation
                            });
                        }
                    }
                }
                
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
        }
        
        if (IsSprite) {
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
            
            for (var f = 0; f < array_length(yyfile.frames); f++) {
                var frame_name = yyfile.frames[f].name;
                array_push(sprite_frames, frame_name);
                
                file_copy(filename_dir(global.selected_yyp) + "\\sprites\\" + yyfile.name + "\\" + frame_name + ".png", 
                         spriteoutput + yyfile.name + "\\" + frame_name + ".png");
						 
                file_text_write_string(t3s_file, yyfile.name + "/" + frame_name + ".png\n");
            }
            
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
            
            currentsprite_count++;
        }
        
        if (IsObject) {
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
        }
    }
    
    // export json
    var export_json = {
        GameMetadata: {
            name: global.game_name,
            titleId: global.title_id,
            publisher: global.publisher
        },
        
        FirstRoom: {
            name: first_room_name,
        },
        
        Resources: {
            all_resources
        },
        
        Rooms: {
            all_rooms
        },
        
        Sprites: {
            all_sprites
        },
        
        Objects: { 
            all_objects
        }
    };
    
	//create the data file
	//3DS
	if (global.export_mode == Export.CIA || global.export_mode == Export._3DSX){
		directory_create(destination + "\\romfs\\");
	    var file = file_text_open_write(destination + "\\romfs\\" + "data.gad");
	    file_text_write_string(file, json_stringify(export_json, true));
	    file_text_close(file);
	}    
	
	//windows
	if (global.export_mode == Export.EXE){
		directory_create(destination + "\\output\\");
	    var file = file_text_open_write(destination + "\\output\\" + "data.gad");
	    file_text_write_string(file, json_stringify(export_json, true));
	    file_text_close(file);
	}
	
	//Dreamcast
	if (global.export_mode == Export.DC){
		directory_create(destination + "\\romdisk\\");
	    var file = file_text_open_write(destination + "\\romdisk\\" + "data.gad");
	    file_text_write_string(file, json_stringify(export_json, true));
	    file_text_close(file);
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
	if (global.export_mode == 0)
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make cia");
			
	//3ds 3dsx
	if (global.export_mode == 1)
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make 3dsx");
					
	//windows exe
	if (global.export_mode == 2)
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make win");
	
	//finsih!!!
	logging = true;
	global.exporting = false;
	//show_message("Compiling now!\nCheck " + destination + "\\output\\ " + " For the rom!")
}