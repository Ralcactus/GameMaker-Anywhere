width = 300;
height = 0;
depth = -999;
SettingsNames = [];
SettingsVariables = [];

ButtonSprites = [];
ButtonFunctions = [];
ButtonX = [];
ButtonY = [];

TurnThisSettingBoxThingGray = -4;
DestroySettings = false;

bgx = 0;
bgy = 0;

create_option("Interpolate colours between pixels", "SmoothPixels");
create_option("Use synchronization to avoid tearing (DOES NOTHING, STUBBED)", "Vsync");


//debug_option
create_option("Copy sprites to the project [[DEBUG]]", "copysprite");
create_option("Copy code onto scripts and objects [[DEBUG]]", "copycode");
create_option("Skip install checks [[DEBUG]]", "skip_installchecks");

//back button
create_button(100, 50, spr_back, 
function(){
	global.DisableMenu = false;
	DestroySettings = true;
});

//debug runner button
create_button(400, 50, spr_debug, 
function(){
	show_message("PICK A FOLDER CONTAINING WHERE A CUSTOM \"RUNNER\" IS");
	var folder = get_directory("");
	if (!file_exists(folder + "/makefile/")){
		var choice = show_question("This doesn't seem to be a correct runner folder, proceed anyways?");
		
		if (choice == false)
			exit;
	}
	
	obj_compiler.runnerfolder = folder;
	
	
});


function create_option(name, variable){
	array_push(SettingsNames, name);
	array_push(SettingsVariables, variable);
}

function create_button(_x, _y, sprite, Order){
	array_push(ButtonSprites, sprite);
	array_push(ButtonFunctions, Order);
	array_push(ButtonX, _x);
	array_push(ButtonY, _y);
}