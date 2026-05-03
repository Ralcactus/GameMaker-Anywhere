function scr_compileSETUP(){
	
	if (global.skip_installchecks == false){
		//enviorment variable checks
		if (environment_get_variable("DEVKITPRO") == ""){
			show_message("DevKitPro is not installed!");
			exit;
		}
	
		if (!directory_exists(global.OutputDrive + "devkitPro\\portlibs\\3ds\\") && (global.export_mode == "CIA" || global.export_mode == "3DSX")){
			show_message("DevKitPro 3DS portlibs are not installed!");
			exit;
		}
	}

    global.exporting = true;

	//reset vars
    all_resources = [];
    all_rooms = [];
    all_sprites = [];
    all_objects = [];
	yyfile = noone;
	_id = noone;
	currentsprite_count = -1;
	t3s_file = noone;
	roomid_count = 0;
	object_count = 0;
	currentobject_count = 0;
	
	global.SpriteWidths = [];
	global.SpriteHeights = [];
	global.SpriteOriginX = [];
	global.SpriteOriginY = [];

	global.SpriteBoxTOP = [];
	global.SpriteBoxBOTTOM = [];
	global.SpriteBoxLEFT = [];
	global.SpriteBoxRIGHT = [];
	global.ObjectIdOBJECT = [];

	if (directory_exists(destination)){
		//delete the old build
	    delete_powershell_window = run_commandpowershell(global.OutputDrive, "Remove-Item -LiteralPath '" + global.OutputDrive + "GM_Anywhere' -Recurse -Force -ErrorAction SilentlyContinue");
		scr_compileMIDDLE1();
	}
	else{
		//copy the runner to the compile folder on the C drive
		copy_powershell_window = run_commandpowershell(global.OutputDrive, "Copy-Item -Path \"" + runnerfolder + " -Destination \"" + destination + " -Recurse");
		scr_compileMIDDLE2();
	}
}

//delete old builds
function scr_compileMIDDLE1(){
	if (CompletionStatusFromExecutedProcess(delete_powershell_window)){
		//copy the runner to the compile folder on the C drive
		if (directory_exists(destination)){
			show_message("ERROR!\nthe previous build could not be deleted fully. Is the folder in use?")
			exit;
		}
		
		copy_powershell_window = run_commandpowershell(global.OutputDrive, "Copy-Item -Path \"" + runnerfolder + " -Destination \"" + destination + " -Recurse");
		scr_compileMIDDLE2();
		exit;
	}
	else
		scr_compileMIDDLE1();
}

//copy runner
function scr_compileMIDDLE2(){
	if (CompletionStatusFromExecutedProcess(copy_powershell_window)){

		if (!directory_exists(destination)){
			show_message("ERROR!\nthe cpp project could not be copied! Is the folder in use?")
			exit;
		}		
		
		//make dirs
		directory_create(destination + "\\source\\rooms\\");
		directory_create(destination + "\\source\\objects\\");
		directory_create(destination + "\\source\\sprites\\");	
		directory_create(destination + "\\output\\");
		if (global.export_mode == "WII")
			directory_create(destination + "\\output\\TheGame\\");
			
		scr_compile();
	}
	else
		scr_compileMIDDLE2();
}

//This is the script that creates creates the code files and copys sprite data
function scr_compile()
{
	//create sprites.t3s
	directory_create(destination + "\\gfx\\");
	
	if (global.export_mode == "3DSX" || global.export_mode == "CIA"){
		t3s_file = file_text_open_write(destination + "\\gfx\\" + "sprites.t3s");
		file_text_write_string(t3s_file, "--atlas\n");
		currentsprite_count+=1;
	}
	
	if (global.export_mode == "GAMECUBE" || global.export_mode == "WII"){
		textures_dolfile = file_text_open_write(destination + "\\gfx\\" + "textures.scf");
		currentsprite_count+=1;
	}

    //json parse
    var yypbuffer = buffer_load(global.selected_yyp);
    var yypdata = buffer_read(yypbuffer, buffer_string);
    buffer_delete(yypbuffer);
    var yyp_json = json_parse(yypdata);


	//the actual compile
    for (var i = 0; i < array_length(yyp_json.resources); i++) {
        _id = yyp_json.resources[i].id;
        array_push(all_resources, {name: _id.name});
        
        var file_buffer = buffer_load(filename_dir(global.selected_yyp) + "/" + _id.path);
        var yyfileFAKE = buffer_read(file_buffer, buffer_string);
        buffer_delete(file_buffer);
        yyfileFAKE = string_replace_all(yyfileFAKE, ",}", "}");
        yyfileFAKE = string_replace_all(yyfileFAKE, ",]", "]");
        yyfile = json_parse(yyfileFAKE);
        
		
		if (yyfile.resourceType == "GMRoom") //asset is a room!
			scr_compilerooms(i, yyp_json);
		
        if (yyfile.resourceType == "GMSprite") //asset is a sprite!
			scr_compilesprites();

        if (yyfile.resourceType == "GMObject") //asset is a object!
			scr_compileobjects();
			
        if (yyfile.resourceType == "GMScript") //asset is a script!
			scr_compilescript();
    }
	
	
	if (global.export_mode == "3DSX" || global.export_mode == "CIA")
		file_text_close(t3s_file);
	
	if (global.export_mode == "GAMECUBE" || global.export_mode == "WII")
		file_text_close(textures_dolfile);
		
	
	scr_write_sprite_info();
	scr_write_metadata();

	//finsih!!!
	logging = true;
	global.exporting = false;

	if (!global.copysprite && (global.export_mode == "3DSX" || global.export_mode == "CIA"))
		file_delete(destination + "\\gfx\\" + "sprites.t3s");

	scr_compilecommand();
}