function scr_compilesounds(){
	var safe_name = sanitize_filename(yyfile.name);
    show_debug_message("Sound: " + yyfile.name);
	
	var spriteidh = file_text_open_append(destination + "source/helpers/asset_toid.h");
	file_text_write_string(spriteidh, "#define " + yyfile.name + " " + string(currentsound_count) + " //Sound\n");
	file_text_close(spriteidh);
	
	var soundpath = filename_dir(filename_dir(global.selected_yyp) + "/" + _id.path) + "/" + yyfile.soundFile;
	show_debug_message(soundpath);
	
	//File is ogg
	if (filename_ext(soundpath) == ".ogg"){
		show_debug_message("File format is ogg");
		
		file_copy(soundpath, destination+"/romfs/audio/TEMPOGG.ogg")
	
		if (!file_exists(destination+"/romfs/audio/TEMPOGG.ogg")){
			sleep(10);
		}

		var pid = run_commandpowershell(destination+"/romfs/audio/", working_directory + "other/ffmpeg.exe -y -i TEMPOGG.ogg -c:a libopus -b:a 128k " + string(currentsound_count) + ".opus", false);
		FreeExecutedProcessStandardOutput(pid);
		FreeExecutedProcessStandardInput(pid);
		pid = 0;
  
		file_delete(destination+"/romfs/audio/TEMPOGG.ogg");
	}
	else{
		show_debug_message("UNKNOWN SOUND FILE FORMAT :(")	
	}
	
	currentsound_count++;
}
