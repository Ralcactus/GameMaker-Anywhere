if (global.DisableMenu)
	exit;

if (mouse_wheel_up())
	texty+=50;	
	
if (mouse_wheel_down())
	texty-=50;	

if (obj_compiler.compile_powershell_window != -4)
	log = ExecutedProcessReadFromStandardOutput(obj_compiler.compile_powershell_window);