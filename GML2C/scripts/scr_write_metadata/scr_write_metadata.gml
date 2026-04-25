function scr_write_metadata(){
	//3DS
	if (global.export_mode == "3DSX" || global.export_mode == "CIA"){
		//app info
		var appinfo = file_text_open_write(destination + "\\resources\\AppInfo");
		file_text_write_string(appinfo, 
		"APP_TITLE = " + global.game_name + "\n"
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
	
	//WII
	if (global.export_mode == "WII"){
		//app info
		var metadataxml = file_text_open_write(destination + "\\output\\TheGame\\meta.xml");
		file_text_write_string(metadataxml, 
		"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
		+ "<app version=\"1\">\n"
		+ "	<name>" + global.game_name + "</name>\n"
		+ "	<coder>" + global.publisher + "</coder>\n"
		+ "	<release_date>00000000</release_date>\n"
		+ "	<short_description>" + "Play GameMaker Games ANYWHERE!" + "</short_description>\n"
		+ "	<long_description>" + "Play GameMaker Games ANYWHERE!" + "</long_description>\n"
		+ "</app>\n");
		file_text_close(metadataxml);
		
		file_copy(working_directory + "Other\\wii_icon.png", destination + "output\\TheGame\\icon.png")
	}
	


 
  


}
