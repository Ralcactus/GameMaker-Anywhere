function scr_compileSETUP(){
	
	if (global.skip_installchecks == false){
		// environment variable checks
		if (environment_get_variable("DEVKITPRO") == ""){
			show_message("DevKitPro is not installed!");
			exit;
		}
	
		var devkit_path = environment_get_variable("DEVKITPRO");
		var _3DSPortlibs = false;
		
		if (os_type == os_windows)
			_3DSPortlibs = directory_exists(global.OutputDrive + "devkitPro\\portlibs\\3ds\\");
		
		if (os_type == os_linux)
			_3DSPortlibs = directory_exists(devkit_path + "/portlibs/3ds/");
		
		if (!_3DSPortlibs && (global.export_mode == "CIA" || global.export_mode == "3DSX")){
			show_message("DevKitPro 3DS portlibs are not installed!");
			exit;
		}
	}

    global.exporting = true;

	// reset vars
    all_resources = [];
    all_rooms = [];
    all_sprites = [];
    all_objects = [];
	globalvar_names = [];
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
		// delete the old build
		if(os_type == os_windows)
		{
	    	delete_powershell_window = run_commandpowershell(global.OutputDrive, "Remove-Item -LiteralPath '" + global.OutputDrive + "GM_Anywhere' -Recurse -Force -ErrorAction SilentlyContinue");
		}
		else if(os_type == os_linux || os_type == os_macosx)
		{
			delete_powershell_window = run_commandpowershell(global.OutputDrive, "rm -rf '" + destination + "'");
		}
		else
		{
			show_message("OS is not supported! Issue in scr_compile.gml delete old build section");
			exit;
		}
		scr_compileMIDDLE1();
	}
	else{
		if(os_type == os_windows)
		{
			copy_powershell_window = run_commandpowershell(global.OutputDrive, "Copy-Item -Path \"" + runnerfolder + " -Destination \"" + destination + " -Recurse");
		}
		else if(os_type == os_linux || os_type == os_macosx)
		{
			copy_powershell_window = run_commandpowershell(
				global.OutputDrive,
				"mkdir -p '" + destination + "' && cp -r '" + runnerfolder + "/.' '" + destination + "'"
			);
		}
		else
		{
			show_message("OS is not supported! Issue in scr_compile.gml copy runner section");
			exit;
		}
		scr_compileMIDDLE2();
	}
}
//delete old builds
function scr_compileMIDDLE1(){
	if (CompletionStatusFromExecutedProcess(delete_powershell_window)){
		show_debug_message("Makefile exists: " + string(file_exists(destination + "/Makefile")));
		show_debug_message("Platform exists: " + string(directory_exists(destination + "/platform")));
		show_debug_message("Resources exists: " + string(directory_exists(destination + "/resources")));
		if (directory_exists(destination)){
			show_message("ERROR!\nthe previous build could not be deleted fully. Is the folder in use?")
			exit;
		}
		// copy the runner
		if(os_type == os_windows)
		{
	    	copy_powershell_window = run_commandpowershell(global.OutputDrive, "Copy-Item -Path \"" + runnerfolder + " -Destination \"" + destination + " -Recurse");
		}
		else if(os_type == os_linux || os_type == os_macosx)
		{
			copy_powershell_window = run_commandpowershell(
				global.OutputDrive,
				"mkdir -p '" + destination + "' && cp -r '" + runnerfolder + "/.' '" + destination + "'"
			);
		}
		else
		{
			show_message("OS is not supported! Issue in scr_compile.gml copy runner section");
			exit;
		}
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
		directory_create(destination + "/source/rooms/");
		directory_create(destination + "/source/objects/");
		directory_create(destination + "/source/sprites/");	
		directory_create(destination + "/output/");
		if (global.export_mode == "WII")
			directory_create(destination + "/output/TheGame/");
		show_debug_message("before compile");

		//write the builtin variables to the variable handler
		
		init_builtin_variables();
		scr_write_variables_builtin();
		
		scr_compile();
		show_debug_message("after compile");
	}
	else
		scr_compileMIDDLE2();
}

//This is the script that creates creates the code files and copys sprite data
function scr_compile()
{
	//create sprites.t3s
	
	directory_create(destination + "/gfx/");
	show_debug_message("create dir");

	if (global.export_mode == "3DSX" || global.export_mode == "CIA"){
		t3s_file = file_text_open_write(destination + "/gfx/" + "sprites.t3s");
		file_text_write_string(t3s_file, "--atlas\n");
		currentsprite_count+=1;
		show_debug_message("create t3s file");
	}

	if (global.export_mode == "GAMECUBE" || global.export_mode == "WII"){
		textures_dolfile = file_text_open_write(destination + "/gfx/" + "textures.scf");
		currentsprite_count+=1;
	}

    //json parse
    var yypbuffer = buffer_load(global.selected_yyp);
    var yypdata = buffer_read(yypbuffer, buffer_string);
    buffer_delete(yypbuffer);
    var yyp_json = json_parse(yypdata);

	show_debug_message("parse json");

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
		{
			show_debug_message("sprites!!!");
			scr_compilesprites();
		}
        if (yyfile.resourceType == "GMObject") //asset is a object!
			scr_compileobjects();
			
        if (yyfile.resourceType == "GMScript") //asset is a script!
			scr_compilescript();
    }
	show_debug_message("compiled resources");
	
	if (global.export_mode == "3DSX" || global.export_mode == "CIA")
	{
		file_text_close(t3s_file);
		show_debug_message("t3s file closed");
	}

	if (global.export_mode == "GAMECUBE" || global.export_mode == "WII")
		file_text_close(textures_dolfile);
	
	
	show_debug_message("CHECKPOINT 3.5");
	show_debug_message("CHECKPOINT 4");
	scr_write_metadata();
	show_debug_message("CHECKPOINT 5");
	scr_write_global_variables();
	show_debug_message("CHECKPOINT 6");
	

	//finsih!!!
	logging = true;
	global.exporting = false;

	if (!global.copysprite && (global.export_mode == "3DSX" || global.export_mode == "CIA"))
		file_delete(destination + "/gfx/" + "sprites.t3s");

	scr_compilecommand();
	show_debug_message("CHECKPOINT 7");
}