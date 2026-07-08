function scr_compilesounds(){
	var safe_name = sanitize_filename(yyfile.name);
	
    show_debug_message("Sound: " + yyfile.name);
	
	var soundpath = filename_dir(filename_dir(global.selected_yyp) + "/" + _id.path) + "/" + yyfile.soundFile;
	show_debug_message(soundpath);
	file_copy(soundpath, destination+"/romfs/audio/TheSound.ogg")
	
	if (!file_exists(destination+"/romfs/TheSound.ogg")){
		sleep(10);
	}

	run_commandpowershell(destination+"/romfs/audio/", working_directory + "other/ffmpeg.exe -y -i TheSound.ogg -c:a libopus -b:a 128k TheSound.opus", false);
	file_delete(destination+"/romfs/audio/TheSound.ogg");
}
