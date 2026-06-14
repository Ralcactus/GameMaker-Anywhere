//runs the make command
function scr_compilecommand(){
	//3ds cia
	if (global.export_mode == "CIA")
		compile_powershell_window = run_commandpowershell(global.OutputDrive + "GM_Anywhere/Runner", "make cia");
			
	//3ds 3dsx
	if (global.export_mode == "3DSX")
		compile_powershell_window = run_commandpowershell(global.OutputDrive + "GM_Anywhere/Runner", "make 3dsx");
			

	//Gamecube dol
	if (global.export_mode == "GAMECUBE")
		compile_powershell_window = run_commandpowershell(global.OutputDrive + "GM_Anywhere/Runner", "make gamecube");			


	//Wii dol
	if (global.export_mode == "WII")
		compile_powershell_window = run_commandpowershell(global.OutputDrive + "GM_Anywhere/Runner", "make wii");
}	

function scr_copy_output_to_home(){
    if (CompletionStatusFromExecutedProcess(compile_powershell_window)){
        var home_dir = "";
        var output_folder = destination + "/output/";
        var copy_dest = "";
		if(os_type == os_linux || os_type == os_macosx)
        {
            home_dir = environment_get_variable("HOME");
			copy_dest = home_dir + "/GM_Anywhere/";
			run_commandpowershell(".", "cp -r '" + output_folder + "' '" + copy_dest + "'");
			show_message("Compilation complete! Output copied to:\n" + copy_dest);
        }
		else if(os_type == os_windows)
		{
			show_message("Compilation complete! Output located at:\n" + output_folder);
		}
        
    }
}