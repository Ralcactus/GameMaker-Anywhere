//this needs to be cleaned up so bad, its so messy
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


	var object_xscale = "{"
	for (var i = 0; i < object_count; i += 1){
		if (i != object_count-1)
			object_xscale+= "1,"
		else
			object_xscale+= "1};"
		
	}
	var object_yscale = "{"
	for (var i = 0; i < object_count; i += 1){
		if (i != object_count-1)
			object_yscale+= "1,"
		else
			object_yscale+= "1};"
		
	}
	file_text_write_string(spriteinfoC, "int otherobject_xscale[] = " + object_xscale + "\n");
	file_text_write_string(spriteinfoC, "int otherobject_yscale[] = " + object_yscale + "\n");
	
	var object_sprite = "{"
	for (var i = 0; i < object_count; i += 1){
		if (i != object_count-1)
			object_sprite+= "-4,"
		else
			object_sprite+= "-4};"
		
	}
	file_text_write_string(spriteinfoC, "int otherobject_sprite[] = " + object_sprite + "\n");
	file_text_write_string(spriteinfoC, "int object_count = " + string(object_count) + ";\n");
	
	var ObjectIDS = [];
	for (var i = 0; i < object_count; i += 1)
		array_push(ObjectIDS, i);
	
	var real_ObjectIDS_array = string_replace(string_replace(string(ObjectIDS), "[", "{"), "]", "}");
	file_text_write_string(spriteinfoC, "int otherobject_id[] = " + string(real_ObjectIDS_array) + ";\n");
	
	var real_objectoid_array = string_replace(string_replace(global.ObjectIdOBJECT, "[", "{"), "]", "}");
	real_objectoid_array = string_replace_all(real_objectoid_array, "\"", "");
	file_text_write_string(spriteinfoC, "int IDtoObject[] = " + string(real_objectoid_array) + ";\n");
	
	file_text_close(spriteinfoC);
}