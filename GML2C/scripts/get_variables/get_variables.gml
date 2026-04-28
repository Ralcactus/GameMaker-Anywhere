function get_variables(caught, code){
	var write = "";
	
	for (var i = 1; i < string_length(code); i += 1){
		var cursor = string_char_at(code, i);
		
		//found =, check its not a ==
		if (cursor == "=" && is_seto(code, i)){
			show_debug_message("found a set thing!");
			var highlight = 0;
			
			//go back until no longer on the =
			var cursor2 = string_char_at(code, i-1);
			while (cursor2 == "="){
				cursor2 = string_char_at(code, i-highlight)
				highlight++;	
			}
			
			//find the first letter of the variable
			highlight = i;
			while (!valid_variablename(cursor2)){
				highlight--;
				cursor2 = string_char_at(code, highlight)
			}
			
			
			
			//get the full variable name
			//get to the back
			while (highlight > 1 && valid_variablename(string_char_at(code, highlight - 1))){
				highlight--;
			}

			//go forward until finding the full name
			var variablename = "";
			for (var j = highlight; j < i; j++){
			    if (!valid_variablename(string_char_at(code, j)))
					break;
					
				variablename += string_char_at(code, j);
			}
			
			if (array_contains(caught, variablename))
				continue;


			write += "std::variant<const char*, float, int> " + variablename + ";\n";
			array_push(caught, variablename);
			//show_message(variablename)
		}
	}
	
	return write;
}

function valid_variablename(letter){
	return (isAlpha(letter) || isDigit(letter) || letter == "_")
}

function is_seto(code, i){
	return (string_char_at(code, i+1) != "=" && string_char_at(code, i-1) != "=" && string_char_at(code, i+1) != ">" &&
	string_char_at(code, i+1) != "<" && string_char_at(code, i-1) != "+" && string_char_at(code, i-1) != "-")
}
