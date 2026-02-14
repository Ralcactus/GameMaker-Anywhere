draw_set_color(c_yellow);
draw_text(10,30, "WIP! This only compiles the data.win\n" + 
	"THIS DOES NOT COMPILE TO THE PLATFORMS CURRENTLY!\n" +
	"you must place the data.win made by this into\nthe runner manually!\n" +
	/*"(the export mode does nothing btw)"*/"")

draw_set_halign(fa_center);
draw_set_valign(fa_top);
draw_sprite(spr_logo, 0, room_width/2, 20);
draw_set_color(c_white);

#region draw buttons
var btn_sel_hover = point_in_rectangle(mouse_x, mouse_y, 
    global.btn_select_x, global.btn_select_y, 
    global.btn_select_x + global.btn_select_w, 
    global.btn_select_y + global.btn_select_h);



draw_set_color(btn_sel_hover ? c_white : c_black);
draw_roundrect(global.btn_select_x, global.btn_select_y,
               global.btn_select_x + global.btn_select_w,
               global.btn_select_y + global.btn_select_h, false);
			   
draw_set_color(btn_sel_hover ? c_black : c_white);
draw_roundrect(global.btn_select_x, global.btn_select_y, 
               global.btn_select_x + global.btn_select_w, 
               global.btn_select_y + global.btn_select_h, true);
			   

draw_set_color(btn_sel_hover ? c_black : c_white);
draw_set_halign(fa_center);
draw_set_valign(fa_middle);
draw_text(global.btn_select_x + global.btn_select_w/2, 
          global.btn_select_y + global.btn_select_h/2, 
          "Select Project File");

#endregion


if (btn_sel_hover && mouse_check_button_pressed(mb_left)) {
    var yyp = get_open_filename("GameMaker Project|*.yyp", "");
    
    if (yyp != "") {
        global.selected_yyp = yyp;
        
        //check version
        var yypbuffer = buffer_load(yyp);
        var yypdata = buffer_read(yypbuffer, buffer_string);
        buffer_delete(yypbuffer);
        var yyp_json = json_parse(yypdata);
        
        if (yyp_json.MetaData.IDEVersion != "2024.14.2.213") {
            show_message("ERROR!\nProject must be a 2024.14.2.213 project!");
            global.selected_yyp = "";
        }
    }
}

draw_set_halign(fa_left);
draw_set_valign(fa_top);
draw_set_color(c_ltgray);

if (global.selected_yyp != "") {
    draw_text(610, 250, "selected: " + filename_name(global.selected_yyp));
} else {
    draw_text(610, 250, "No Project Selected");
}

draw_set_halign(fa_left);
draw_set_valign(fa_top);
var fields = [
    {label: "Game Name:", value: global.game_name, field_id: 0},
    {label: "Title ID:", value: global.title_id, field_id: 1, hint: "(16 hex)"},
    {label: "Publisher:", value: global.publisher, field_id: 2}
];

for (var i = 0; i < array_length(fields); i++) {
    var field = fields[i];
    var field_y = global.field_start_y + (i * global.field_spacing);
    var field_hover = point_in_rectangle(mouse_x, 
	    mouse_y, global.field_x, field_y - 5,
        global.field_x + global.field_w, 
        field_y + global.field_height - 5);
    
    draw_set_color(c_white);
    draw_text(100+350, field_y, field.label);
    
    if (variable_struct_exists(field, "hint")) {
        draw_set_color(c_gray);
        draw_text(100+350, field_y + 20, field.hint);
    }
    
    var is_active = (global.current_field == field.field_id);
    draw_set_color(is_active ? c_yellow : (field_hover ? c_ltgray : c_white));
    draw_roundrect(global.field_x, field_y - 5, 
                   global.field_x + global.field_w, 
                   field_y + global.field_height - 5, true);
    
    draw_set_color(c_white);
    var display_value = field.value;
    if (is_active && current_time % 1000 < 500) {
        display_value += "|";
    }
    draw_text(global.field_x + 8, field_y, display_value);
    if (field_hover && mouse_check_button_pressed(mb_left)) {
        global.current_field = field.field_id;
        keyboard_string = field.value;
    }
}




#region export modes (temporally disabled!!!)
/*
draw_set_halign(fa_left);
draw_set_valign(fa_top);
draw_set_color(c_white);
draw_text(20, 320, "Export Mode:");

for (var i = 0; i < array_length(modes); i++)
{
    var _y = global.field_start_y + (array_length(fields) * global.field_spacing) + 30 + i * 26 - 300;

    //click hitbox
    var hover = point_in_rectangle(mouse_x, mouse_y, 20, _y, 40, _y + 18);
    var selected = global.export_mode == modes[i].number_ver;

    //outline
    draw_set_color(c_white);
	if (hover)
		 draw_set_color(c_dkgray);
		
    draw_roundrect(20, _y, 20 + 18, _y + 18, false);

    //turn green when selected
    if (selected) {
		draw_set_color(c_green);
        draw_roundrect(20, _y, 38, _y + 18, false);
    }

    //os text
    draw_set_color(c_white);
    draw_text(48, _y - 1, modes[i].label);

    //change the mode variable
    if (hover && mouse_check_button_pressed(mb_left)) {
        global.export_mode = modes[i].number_ver;
    }
}
*/
#endregion



if (mouse_check_button_pressed(mb_left)) {
    var clicked_field = false;
    for (var i = 0; i < array_length(fields); i++) {
        var field_y = global.field_start_y + (i * global.field_spacing);
        if (point_in_rectangle(mouse_x, 
		    mouse_y, global.field_x, field_y - 5,
            global.field_x + global.field_w, 
            field_y + global.field_height - 5)) {
            clicked_field = true;
            break;
        }
    }
    if (!clicked_field && !btn_sel_hover) {
        global.current_field = -1;
    }
}


var btn_exp_hover = point_in_rectangle(mouse_x, 
    mouse_y, global.btn_export_x, global.btn_export_y,
    global.btn_export_x + global.btn_export_w,
    global.btn_export_y + global.btn_export_h);


var can_export = (global.selected_yyp != "" && 
    global.game_name != "" && 
    global.title_id != "" && 
    string_length(global.title_id) == 16 &&
    global.publisher != "");

if (can_export) {
    draw_set_color(btn_exp_hover ? c_lime : c_green);
} else {
    draw_set_color(c_dkgray);
}
draw_roundrect(global.btn_export_x, global.btn_export_y,
    global.btn_export_x + global.btn_export_w,
    global.btn_export_y + global.btn_export_h, false);

draw_set_color(c_white);
draw_roundrect(global.btn_export_x, global.btn_export_y,
    global.btn_export_x + global.btn_export_w,
    global.btn_export_y + global.btn_export_h, true);

draw_set_halign(fa_center);
draw_set_valign(fa_middle);
draw_text(global.btn_export_x + global.btn_export_w/2,
    global.btn_export_y + global.btn_export_h/2,
    "Export");

//pressed the compile button and all the fields are filled out
if (can_export && btn_exp_hover && mouse_check_button_pressed(mb_left)) {
	scr_compile();
}

draw_set_halign(fa_center);
draw_set_valign(fa_top);
draw_set_color(c_red);

if (global.selected_yyp == "") {
    draw_text(room_width/2, 700, "Please select a project file");
} else if (global.game_name == "") {
    draw_text(room_width/2, 700, "Please enter a game name");
} else if (global.title_id == "") {
    draw_text(room_width/2, 700, "Please enter a Title ID");
} else if (string_length(global.title_id) != 16) {
    draw_text(room_width/2, 700, "ID must be 16 hex digits (current: " + string(string_length(global.title_id)) + ")");
} else if (global.publisher == "") {
    draw_text(room_width/2, 700, "Please enter a publisher name");
} else {
    draw_set_color(c_lime);
    draw_text(room_width/2, 700, "Export Ready");
}
draw_set_halign(fa_left);
draw_set_valign(fa_top);
draw_set_color(c_white);
