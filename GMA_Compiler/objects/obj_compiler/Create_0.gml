global.compile_waiting_delete = false;
global.compile_delete_started = false;
global.selected_yyp = "";
global.game_name = "";
global.title_id = "";
global.publisher = "";
global.current_field = -1; // -1 = none, 0 = game name, 1 = title id, 2 = publisher
global.exporting = false;

global.btn_select_x = 300+300;
global.btn_select_y = 100+200;
global.btn_select_w = 200;
global.btn_select_h = 40;

global.btn_export_x = 300+300;
global.btn_export_y = 450+200;
global.btn_export_w = 200;
global.btn_export_h = 40;

global.field_start_y = 180+200;
global.field_height = 35;
global.field_spacing = 80;
global.field_x = 250+300;
global.field_w = 300;

keyboard_string = "";

global.export_mode = 0; //1 = 3ds, 2 = wii u, 3 = nds
modes = [
    {label:"3DS cia", number_ver:0},
	{label:"3DS 3dsx", number_ver:1}
];