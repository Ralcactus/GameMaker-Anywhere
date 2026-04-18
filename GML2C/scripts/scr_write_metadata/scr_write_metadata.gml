function scr_write_metadata(){
	//3DS
	if (global.export_mode == "3DSX" || global.export_mode == "CIA"){
		//app info
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
	}
}
