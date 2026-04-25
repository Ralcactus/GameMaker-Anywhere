//open to the app when compiling is done
if (CompletionStatusFromExecutedProcess(compile_powershell_window) && logging == true){
	run_commandpowershell(destination+"\\output\\", "explorer .")
	
	if (global.export_mode == "WII"){
		file_delete(destination + "\\output\\TheGame\\boot.elf\\")
	}
	
	logging = false;
}
