global.OutputDrive = "C:\\";

//folders location for exports
runnerfolder = working_directory + "Runner\\";
destination = global.OutputDrive + "GM_Anywhere\\Runner\\";

//had to ban these because they are built in C funcs lol (probably change this to something better...)
//just add them in the array and the compiler with throw a warning if its compiling something mentioning it
banned_strings = ["clock"]

global.SpriteWidths = [];
global.SpriteHeights = [];
global.SpriteOriginX = [];
global.SpriteOriginY = [];

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
