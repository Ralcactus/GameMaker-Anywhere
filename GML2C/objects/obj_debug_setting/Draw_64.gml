var btn_sel_hover = point_in_rectangle(mouse_x, mouse_y, 
    x, y+ 10, 
    x + btn_select_w, 
	y+ 10 + btn_select_h);


draw_set_color(btn_sel_hover ? c_white : c_black);
draw_roundrect(x, y+10,
               x + btn_select_w,
               y+10 + btn_select_h, false);
			   
draw_set_color(btn_sel_hover ? c_black : c_white);
draw_roundrect(x, y+10,
               x + btn_select_w,
               y+10 + btn_select_h, true);
			   

draw_set_color(btn_sel_hover ? c_black : c_white);
draw_set_halign(fa_center);
draw_set_valign(fa_middle);
draw_text(x + btn_select_w/2, y+10 + btn_select_h/2, "Debug Settings");
		  

if (btn_sel_hover && mouse_check_button_pressed(mb_left)) {
	global.copysprite = show_question("Copy sprites?");
	global.copycode = show_question("Copy code?");
	global.skip_installchecks = show_question("Skip install Checks?");
}

draw_set_colour(c_white);
draw_text(100,100, "copy sprites " + string(global.copysprite));
draw_text(100,120, "copy code " + string(global.copycode));
draw_text(100,140, "skip install checks " + string(global.skip_installchecks));
