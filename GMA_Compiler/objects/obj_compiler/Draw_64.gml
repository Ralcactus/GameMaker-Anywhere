draw_set_halign(fa_center);
draw_set_valign(fa_top);
draw_set_color(c_white);

//PROJECT FILE BUTTON
var btn_sel_hover_prjfile = point_in_rectangle(mouse_x, mouse_y, global.btn_select_x, global.btn_select_y, global.btn_select_x + global.btn_select_w, global.btn_select_y + global.btn_select_h);

draw_set_color(btn_sel_hover_prjfile ? c_white : c_black);
draw_roundrect(global.btn_select_x, global.btn_select_y, global.btn_select_x + global.btn_select_w, global.btn_select_y + global.btn_select_h, false);

draw_set_color(btn_sel_hover_prjfile ? c_black : c_white);
draw_roundrect(global.btn_select_x, global.btn_select_y,  global.btn_select_x + global.btn_select_w, global.btn_select_y + global.btn_select_h, true);
			   
draw_set_color(btn_sel_hover_prjfile ? c_black : c_white);
draw_set_halign(fa_center);
draw_set_valign(fa_middle);
draw_text(global.btn_select_x + global.btn_select_w/2, global.btn_select_y + global.btn_select_h/2, "Select Project File");



if (btn_sel_hover_prjfile && mouse_check_button_pressed(mb_left)) {
    var yyp = get_open_filename("GameMaker Project|*.yyp", "");
    
    if (yyp != "") {
        global.selected_yyp = yyp;
        
        //check version
        var yypbuffer = buffer_load(yyp);
        var yypdata = buffer_read(yypbuffer, buffer_string);
        buffer_delete(yypbuffer);
        var yyp_json = json_parse(yypdata);
        
        if (yyp_json.MetaData.IDEVersion != "2024.14.2.213")
            show_message("WARNING!\nThis project isn't 2024.14.2.213, it may not work!");
    }
}

draw_set_halign(fa_left);
draw_set_valign(fa_top);
draw_set_color(c_ltgray);

draw_set_halign(fa_center);
if (global.selected_yyp != "")
    draw_text(1100, 170, "selected: " + filename_name(global.selected_yyp));
else
    draw_text(1100, 170, "No Project Selected!");

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
    var field_hover = point_in_rectangle(mouse_x, mouse_y, global.field_x, field_y - 5, global.field_x + global.field_w, field_y + global.field_height - 5);
    
    draw_set_color(c_white);
    draw_text(850, field_y, field.label);
    
    if (variable_struct_exists(field, "hint")){
        draw_set_color(c_gray);
        draw_text(850, field_y + 20, field.hint);
    }
    
    var is_active = (global.current_field == field.field_id);
    draw_set_color(is_active ? c_yellow : (field_hover ? c_ltgray : c_white));
    draw_roundrect(global.field_x, field_y - 5, global.field_x + global.field_w, field_y + global.field_height - 5, true);
    
    draw_set_color(c_white);
    var display_value = field.value;
    if (is_active && current_time % 1000 < 500) 
        display_value += "|";
		
    draw_text(global.field_x + 8, field_y, display_value);
    if (field_hover && mouse_check_button_pressed(mb_left)){
        global.current_field = field.field_id;
        keyboard_string = field.value;
    }
}




if (mouse_check_button_pressed(mb_left)){
    var clicked_field = false;
    for (var i = 0; i < array_length(fields); i++) {
        var field_y = global.field_start_y + (i * global.field_spacing);
        if (point_in_rectangle(mouse_x, mouse_y, global.field_x, field_y - 5, global.field_x + global.field_w, field_y + global.field_height - 5)) {
            clicked_field = true;
            break;
        }
    }
	
    if (!clicked_field && !btn_sel_hover_prjfile)
        global.current_field = -1;
}


//EXPORT BUTTON
//hovering on export button?
var btn_exp_hover = point_in_rectangle(mouse_x, mouse_y, global.btn_export_x, global.btn_export_y,global.btn_export_x + global.btn_export_w, global.btn_export_y + global.btn_export_h);

//export button inside
draw_set_color(btn_exp_hover ? c_lime : c_green);
draw_roundrect(global.btn_export_x, global.btn_export_y, global.btn_export_x + global.btn_export_w, global.btn_export_y + global.btn_export_h, false);

//export button outline
draw_set_color(c_white);
draw_roundrect(global.btn_export_x, global.btn_export_y, global.btn_export_x + global.btn_export_w, global.btn_export_y + global.btn_export_h, true);

draw_set_halign(fa_center);
draw_set_valign(fa_middle);
draw_text(global.btn_export_x + global.btn_export_w/2, global.btn_export_y + global.btn_export_h/2, "Export");

//Pressed the export button
if (btn_exp_hover && mouse_check_button_pressed(mb_left)){
	if (global.selected_yyp == "")
	    show_message("Please select a project file!");
	else if (global.game_name == "")
	    show_message("Please enter a game name!");
	else if (global.title_id == "")
	    show_message("Please enter a Title ID!");
	else if (string_length(global.title_id) != 16)
	    show_message("ID must be 16 hex digits (current: " + string(string_length(global.title_id)) + ")");
	else if (global.publisher == "")
	    show_message("Please enter a publisher name!");
	else
		scr_compile();
}

#region export modes
draw_set_halign(fa_left);
draw_set_valign(fa_top);
draw_set_color(c_white);

for (var i = 0; i < array_length(modes); i++)
{
    var _y = global.field_start_y + (array_length(fields) * global.field_spacing) + 90 + i * 26 - 300;
	var _x = 400;
	draw_text(_x, 270, "Export Mode:");

    //click hitbox
    var hover = point_in_rectangle(mouse_x, mouse_y, _x, _y, _x+20, _y + 18);
    var selected = global.export_mode == modes[i].number_ver;

    //outline
    draw_set_color(c_white);
	if (hover)
		 draw_set_color(c_dkgray);
		
    draw_roundrect(_x, _y, _x + 18, _y + 18, false);

    //turn green when selected
    if (selected){
		draw_set_color(c_green);
        draw_roundrect(_x, _y, _x + 18, _y + 18, false);
    }

    //export text
    draw_set_color(c_white);
    draw_text(_x+28, _y - 1, modes[i].label);

    //change the mode variable
    if (hover && mouse_check_button_pressed(mb_left))
        global.export_mode = modes[i].number_ver;
}
#endregion


