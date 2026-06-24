global.copysprite = true;
if(os_type == os_windows)
{
    global.OutputDrive = "C:\\";
}
else if(os_type == os_linux || os_type == os_macosx)
{
    global.OutputDrive = "/tmp/";
}
else
{
    show_message("OS is not supported! Issue in scr_compile.gml set output drive section");
    exit;
}

runnerfolder = working_directory + "runner/";
destination = global.OutputDrive + "GM_Anywhere/Runner/";

show_debug_message("runnerfolder = " + runnerfolder);
show_debug_message("destination = " + destination);

//had to ban these because they are built in C funcs lol (probably change this to something better...)
//just add them in the array and the compiler with throw a warning if its compiling something mentioning it
banned_strings = ["clock"]

global.SpriteWidths = [];
global.SpriteHeights = [];
global.SpriteOriginX = [];
global.SpriteOriginY = [];

global.SpriteBoxTOP = [];
global.SpriteBoxBOTTOM = [];
global.SpriteBoxLEFT = [];
global.SpriteBoxRIGHT = [];

global.ObjectIdOBJECT = [];
currentobject_count = -1;
object_count = 0;
general_varnames = [];
general_vardefaults = [];
varname_written = [];
vardefault_written = [];
var_names = [];
var_defaults = [];
globalvar_names = [];
all_resources = [];
all_rooms = [];
all_sprites = [];
all_objects = [];
yyfile = noone;
_id = noone;
currentsprite_count = 0;
t3s_file = noone;
textures_dolfile = noone;
roomid_count = 0;

logging = false;
delete_powershell_window = -40;
copy_powershell_window = -40;
compile_powershell_window = -40;
global.exporting = false;

//debug
global.copysprite = true;
global.copycode = true;
global.skip_installchecks = false;

keyboard_string = "";

global.export_mode = "3DSX";