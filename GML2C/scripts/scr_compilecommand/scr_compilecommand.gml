//runs the make command
function scr_compilecommand(){
	//3ds cia
	if (global.export_mode == Export.CIA)
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make cia");
			
	//3ds 3dsx
	if (global.export_mode == Export._3DSX)
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make 3dsx");
					
	//windows exe
	if (global.export_mode == Export.EXE)
		compile_powershell_window = run_commandpowershell("C:\\GM_Anywhere\\Runner", "make win");
}