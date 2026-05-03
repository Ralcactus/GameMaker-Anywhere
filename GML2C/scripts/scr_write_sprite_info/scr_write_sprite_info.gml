function scr_write_sprite_info(){
	var spriteinfoC = file_text_open_append(destination + "source\\get_spriteinfo.cpp");
	var realspritewidth_array = string_replace(string_replace(global.SpriteWidths, "[", "{"), "]", "}");
	var realspriteheight_array = string_replace(string_replace(global.SpriteHeights, "[", "{"), "]", "}");
	
	var realspriteoriginX_array = string_replace(string_replace(global.SpriteOriginX, "[", "{"), "]", "}");
	var realspriteoriginY_array = string_replace(string_replace(global.SpriteOriginY, "[", "{"), "]", "}");
	
	file_text_write_string(spriteinfoC, "int SpriteWidths[] = " + string(realspritewidth_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteHeights[] = " + string(realspriteheight_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteOriginX[] = " + string(realspriteoriginX_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteOriginY[] = " + string(realspriteoriginY_array) + ";\n");
	
	//sprite bounding box
	var real_spriteboundbox_TOP_array = string_replace(string_replace(global.SpriteBoxTOP, "[", "{"), "]", "}");
	var real_spriteboundbox_BOTTOM_array = string_replace(string_replace(global.SpriteBoxBOTTOM, "[", "{"), "]", "}");
	var real_spriteboundbox_LEFT_array = string_replace(string_replace(global.SpriteBoxLEFT, "[", "{"), "]", "}");
	var real_spriteboundbox_RIGHT_array = string_replace(string_replace(global.SpriteBoxRIGHT, "[", "{"), "]", "}");
	
	file_text_write_string(spriteinfoC, "int SpriteCollideTOP[] = " + string(real_spriteboundbox_TOP_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteCollideBOTTOM[] = " + string(real_spriteboundbox_BOTTOM_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteCollideLEFT[] = " + string(real_spriteboundbox_LEFT_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteCollideRIGHT[] = " + string(real_spriteboundbox_RIGHT_array) + ";\n");
	
	//other object x and y
	var object_x = "{"
	for (var i = 0; i < object_count; i += 1){
		if (i != object_count-1)
			object_x+= "0,"
		else
			object_x+= "0};"
		
	}
	var object_y = "{"
	for (var i = 0; i < object_count; i += 1){
		if (i != object_count-1)
			object_y+= "0,"
		else
			object_y+= "0};"
		
	}

	file_text_write_string(spriteinfoC, "int otherobject_x[] = " + object_x + "\n");
	file_text_write_string(spriteinfoC, "int otherobject_y[] = " + object_y + "\n");
	
	file_text_close(spriteinfoC);
}