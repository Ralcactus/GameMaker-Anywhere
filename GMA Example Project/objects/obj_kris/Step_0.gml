if (gamepad_button_check(4, gp_padl)) //left
	moveH = -1;
else if (gamepad_button_check(4, gp_padr)) //right
	moveH = 1;
else
	moveH = 0;
	
if (gamepad_button_check(4, gp_padu)) //up
	moveV = -1;
else if gamepad_button_check(4, gp_padd) //down (notice how no brackets around the if? gobo syntax conversion :drool:)
	moveV = 1;
else
	moveV = 0;

if (moveH != 0){
	sprite_index = spr_kris_side;
	image_xscale = moveH;	
}

if (moveV == 1){
	sprite_index = spr_kris_down;
	image_xscale = 1;	
}

if (moveV == -1){
	sprite_index = spr_kris_up;
	image_xscale = 1;	
}

if (moveV == 0 && moveH == 0)
	image_index = 0;

hsp = moveH*movespeed;
vsp = moveV*movespeed;

x+=hsp;
y+=vsp;

camx = x-camera_get_view_width(view_camera[0])/2;
camy = y-camera_get_view_height(view_camera[0])/2;

camx = clamp(camx, 0, room_width-camera_get_view_width(view_camera[0]));
camy = clamp(camy, 0, room_height-camera_get_view_height(view_camera[0]));

camera_set_view_pos(view_camera[0], camx, camy);

if (gamepad_button_check_pressed(4, gp_start))
	room_goto(Global_test);
