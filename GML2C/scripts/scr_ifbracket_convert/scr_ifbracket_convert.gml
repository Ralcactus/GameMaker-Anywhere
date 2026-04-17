function scr_ifbracket_convert(i, char_code, originalcode){
	if (char_code == "i"){																	//space										//tab
		if (string_char_at(originalcode, i+1) == "f" && (string_char_at(originalcode, i+2) == " " || string_char_at(originalcode, i+2) == "	")){
			//show_debug_message("found an if statment!!");
		
			var code_cursor = i + 2;
		
			//starting bracket
			while (code_cursor <= string_length(originalcode)){
				var cursorchar = string_char_at(originalcode, code_cursor);
				if (cursorchar != " " && cursorchar != "	")
					break;
						
				code_cursor++;
			}
				
			//already a (
			if (string_char_at(originalcode, code_cursor) == "(")
				return originalcode;
					
			originalcode = string_insert("(", originalcode, code_cursor);
			code_cursor++;
				
			//end bracket (this is gonna suck...)
			var condition_end = -1;
			var last_nonempty_char = "";

			for (; code_cursor <= string_length(originalcode); code_cursor++) {
				var current_char = string_char_at(originalcode, code_cursor);

				if (current_char == "\n" && last_nonempty_char != "&" && last_nonempty_char != "|" && last_nonempty_char != "+" && last_nonempty_char != "-" && last_nonempty_char != "*" && last_nonempty_char != "/" && last_nonempty_char != "," && last_nonempty_char != "\\"){
					var peek = code_cursor + 1;
					
					for (; code_cursor <= string_length(originalcode); peek++){
						if (string_char_at(originalcode, peek) != " " && string_char_at(originalcode, peek) != "\t" && string_char_at(originalcode, peek) != "\n")
							break;
					}

					if (string_char_at(originalcode, peek) != "&" && string_char_at(originalcode, peek) != "|" && string_char_at(originalcode, peek) != "+" && string_char_at(originalcode, peek) != "-" && string_char_at(originalcode, peek) != "*" && string_char_at(originalcode, peek) != "/"){
						condition_end = code_cursor;
						break;
					}
				}

				if (current_char != " " && current_char != "\n")
					last_nonempty_char = current_char;

			}

			originalcode = string_insert(")", originalcode, condition_end-2);
			i = condition_end;
		}
	}
	
	return originalcode;
}