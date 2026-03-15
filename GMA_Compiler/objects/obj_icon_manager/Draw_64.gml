var btn_sel_hover = point_in_rectangle(mouse_x, mouse_y, 
    x, y+ 10, 
    x + global.btn_select_w, 
	y+ 10 + global.btn_select_h);


draw_set_color(btn_sel_hover ? c_white : c_black);
draw_roundrect(x, y+10,
               x + global.btn_select_w,
               y+10 + global.btn_select_h, false);
			   
draw_set_color(btn_sel_hover ? c_black : c_white);
draw_roundrect(x, y+10,
               x + global.btn_select_w,
               y+10 + global.btn_select_h, true);
			   

draw_set_color(btn_sel_hover ? c_black : c_white);
draw_set_halign(fa_center);
draw_set_valign(fa_middle);
draw_text(x + global.btn_select_w/2, y+10 + global.btn_select_h/2, "Add Icon");
		  

if (btn_sel_hover && mouse_check_button_pressed(mb_left)) {
	var savedicon_path = global.iconpath;
	var savedicon_sprite = global.iconspr;
	
	global.iconpath = get_open_filename("3ds Icon|*.png", "");
	global.iconspr = sprite_add(global.iconpath, 0, false, false, -8, 48);
	
	if (sprite_get_width(global.iconspr) != 48 || sprite_get_height(global.iconspr) != 48){
		show_message("ERROR!\nThe icon must be 48x48!");
		global.iconspr = savedicon_sprite;
		global.iconpath = savedicon_path;
		exit;
	}
	
	sprite_index = global.iconspr;
}
