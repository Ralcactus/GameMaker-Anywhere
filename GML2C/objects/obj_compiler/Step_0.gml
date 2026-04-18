//open to the app when compiling is done
if (CompletionStatusFromExecutedProcess(compile_powershell_window) && logging == true){
	run_commandpowershell(destination+"\\output\\", "explorer .")
	logging = false;
}
