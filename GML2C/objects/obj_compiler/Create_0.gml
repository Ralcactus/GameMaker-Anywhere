//folders location for exports
runnerfolder = working_directory + "Runner\\";
destination = "C:\\GM_Anywhere\\Runner\\";

var_names = [];
var_defaults = [];
KnownFirstRoom = false;
all_resources = [];
all_rooms = [];
all_sprites = [];
all_objects = [];
yyfile = noone;
_id = noone;
currentsprite_count = 0;
t3s_file = noone;

logging = false;
delete_powershell_window = -40;
copy_powershell_window = -40;
compile_powershell_window = -40;
global.selected_yyp = "";
global.game_name = "";
global.title_id = "";
global.publisher = "";
global.current_field = -1; // -1 = none, 0 = game name, 1 = title id, 2 = publisher
global.exporting = false;

var xadd = 700;
var yadd = 100;

global.btn_select_x = 300+xadd;
global.btn_select_y = 100+yadd;
global.btn_select_w = 200;
global.btn_select_h = 40;

global.btn_export_x = 300+xadd;
global.btn_export_y = 400+yadd;
global.btn_export_w = 200;
global.btn_export_h = 40;

global.field_start_y = 180+yadd;
global.field_height = 35;
global.field_spacing = 80;
global.field_x = 250+xadd;
global.field_w = 300;

global.shellid = noone;

keyboard_string = "";

enum Export{
	CIA = 0,
	_3DSX = 1,
	EXE = 2,
	DC = 3
}

global.export_mode = Export._3DSX;
modes = [
    {label:"3DS cia", number_ver: Export.CIA},
	{label:"3DS 3dsx", number_ver: Export._3DSX}
	//removed these for now
	//{label:"Windows exe (Raylib)", number_ver: Export.EXE}, 
	//{label:"Dreamcast (DreamSDK & Raylib)", number_ver: Export.DC}
];