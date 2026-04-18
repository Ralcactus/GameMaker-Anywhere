if (active){
	if (keyboard_string != "")
		global.selected_yyp = keyboard_string;
}
else{
	global.selected_yyp = string_replace(global.selected_yyp, "|", "");
}

if (point_in_rectangle(mouse_x, mouse_y, x+sprite_width+10, y, x +sprite_width+10 + sprite_get_width(spr_select_file), y + sprite_get_height(spr_select_file))){
	file_selecthover = true
	if (mouse_check_button_pressed(mb_left)){
	    var yyp = get_open_filename("GameMaker Project|*.yyp", "");
    
	    if (yyp != "") {
	        global.selected_yyp = yyp;
			if (active)
				keyboard_string = yyp;
		
	        //check version
	        var yypbuffer = buffer_load(yyp);
	        var yypdata = buffer_read(yypbuffer, buffer_string);
	        buffer_delete(yypbuffer);
	        var yyp_json = json_parse(yypdata);
        
	        //if (yyp_json.MetaData.IDEVersion != "2024.14.2.213")
	            //show_message("WARNING!\nThis project isn't 2024.14.2.213, it may not work!");
	    }
	}
}
else
	file_selecthover = false;

if (mouse_check_button_pressed(mb_left)){	
	if (instance_position(mouse_x, mouse_y, id)){
		active = true;
		obj_gamename_input.active = false;
		obj_titleid_input.active = false;
		keyboard_string = global.selected_yyp;
	}
	else{
		active = false;
	}
}	
