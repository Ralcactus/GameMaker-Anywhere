//folders location for exports
runnerfolder = working_directory + "Runner\\";
destination = "C:\\GM_Anywhere\\Runner\\";

var_names = [];
var_defaults = [];
all_resources = [];
all_rooms = [];
all_sprites = [];
all_objects = [];
yyfile = noone;
_id = noone;
currentsprite_count = 0;
t3s_file = noone;
roomid_count = 0;

logging = false;
delete_powershell_window = -40;
copy_powershell_window = -40;
compile_powershell_window = -40;
global.exporting = false;

//debug
global.copysprite = true;
global.copycode = true;

keyboard_string = "";

enum Export{
	CIA = 0,
	_3DSX = 1,
	EXE = 2,
	DC = 3
}

global.export_mode = "3DSX";
