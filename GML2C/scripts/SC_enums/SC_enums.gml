function SC_enums(code){
	var pos = string_pos("enum", code);
	
	while (pos != 0) {
		//rename the enum to struct
		code = string_delete(code, pos, 4);
		code = string_insert("struct", code, pos);

		//rename the struct
		var temp_pos = pos+7;
		var enumName = "";
		
		while (string_char_at(code, temp_pos) != " "){
			//temp_pos++; //move up to land on the name
			
			while (string_char_at(code, temp_pos) != " "){
				enumName += string_char_at(code, temp_pos);
				temp_pos++;
			}
		}
		
		var sillyinsert = "FAMILYUGYFUunnyMomentsMT" + string(irandom(100));
		code = string_insert(sillyinsert, code, temp_pos);
		temp_pos+=string_length(sillyinsert);
		
		//insert the first one
		while (string_char_at(code, temp_pos) != "\n"){
			temp_pos++;
		}
		temp_pos++; //actually go down to the next line
		code = string_insert("	GMvar", code, temp_pos);
	
		//insert the rest of the types
		while (string_char_at(code, temp_pos) != "}"){
			if (string_char_at(code, temp_pos) == ","){
				code = string_delete(code, temp_pos, 1);
				code = string_insert(";", code, temp_pos);
				
				while (string_char_at(code, temp_pos) != "\n")
					temp_pos++;
					
				temp_pos++; //actually go down to the next line
				if (string_char_at(code, temp_pos) != "}")
					code = string_insert("	GMvar", code, temp_pos);
					code = string_delete(code, temp_pos+6, 1)
					code = string_insert(" ", code, temp_pos+6)
			}
			
			if (string_char_at(code, temp_pos-1) != "}"){
				temp_pos++;	
			}
			else{
				temp_pos--;
			}
		}
		
		
		code = string_insert(";", code, temp_pos+1);
		
		var bleh = string_copy(code, pos, temp_pos-pos+2);
		code = string_delete(code, pos, temp_pos-pos+2);

		var miscH = file_text_open_append(destination + "source/variable_handler.h");
		file_text_write_string(miscH, bleh+"\n");
		file_text_write_string(miscH, "inline " + enumName + sillyinsert + " " + enumName + ";\n");
		file_text_close(miscH);

		pos = string_pos("enum", code);
	}
	
	return code;
}