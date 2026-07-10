//open to the app when compiling is done
if (CompletionStatusFromExecutedProcess(compile_powershell_window) && logging == true){
	var FileExtension = "";
	if (global.export_mode == "3DSX"){
		file_rename(destination+"/output/Runner.3dsx", destination+"/output/" + global.game_name+".3dsx");
		file_delete(destination+"/output/Runner.elf");
		file_delete(destination+"/output/Runner.smdh");
	}

	if (global.export_mode == "CIA"){
		file_rename(destination+"/output/Runner.cia", destination+"/output/" + global.game_name+".cia");
		file_delete(destination+"/output/Runner.elf");
		file_delete(destination+"/output/Runner.smdh");		
	}

	if (global.export_mode == "WII"){
		file_rename(destination+"/output/boot.dol", destination+"/output/" + global.game_name + "/" + "boot.dol");
		file_delete(destination + "/output/boot.elf/");
	}
	
	if (global.export_mode == "GAMECUBE"){
		file_rename(destination+"/output/Runner.dol", destination+"/output/" + global.game_name+".dol");
		file_delete(destination + "/output/Runner.elf/");
	}

	var bleh = run_commandpowershell(destination+"/output/", "explorer .")

	FreeExecutedProcessStandardOutput(bleh);
	FreeExecutedProcessStandardInput(bleh);
	FreeExecutedProcessStandardOutput(compile_powershell_window);
	FreeExecutedProcessStandardInput(compile_powershell_window);
	compile_powershell_window = -4;

	
	
	logging = false;
}

