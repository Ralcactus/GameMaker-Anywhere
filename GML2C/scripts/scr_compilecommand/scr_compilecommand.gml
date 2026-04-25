//runs the make command
function scr_compilecommand(){
	//3ds cia
	if (global.export_mode == "CIA")
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make cia");
			
	//3ds 3dsx
	if (global.export_mode == "3DSX")
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make 3dsx");
			

	//Gamecube dol
	if (global.export_mode == "GAMECUBE")
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make gamecube");			


	//Wii dol
	if (global.export_mode == "WII")
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make wii");
}	