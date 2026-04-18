if (curt.dd) return;
var create_dd = function() {
	var cu = curt.cu,
		cx = x,
		cy = y+sprite_height;

	curt.dd = dropdown_create(cx, cy, 160, 160, curt.li, example_selected, [cu], cu, 38);
	dropdown_set_style(curt.dd, -4, $ffffff, $333333, $444444, 10, $aaaaaa);
}


if (mouse_check_button_pressed(mb_left)) {
	var ex = draw.ex;
	if (point_in_rectangle(mouse_x, mouse_y, x, y, x+sprite_width, y+sprite_height))
		create_dd();
}

global.export_mode = curt.li[curt.cu];
