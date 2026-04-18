if (instance_position(mouse_x, mouse_y, id) && mouse_check_button_pressed(mb_left)){
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
	else{
		with (obj_compiler)
			scr_compileSETUP();	
	}
}