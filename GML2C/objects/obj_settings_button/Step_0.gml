if (global.DisableMenu){
	image_blend = c_white
	exit;	
}
	
if (instance_position(mouse_x, mouse_y, id)){
	image_blend = c_gray;

	if (mouse_check_button_pressed(mb_left)){
		global.DisableMenu = true;
		instance_create_depth(x,y,depth,obj_settings);
	}
}
else
	image_blend = c_white;