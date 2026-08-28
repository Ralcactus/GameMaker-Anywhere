//Wow this does NOTHING!
//Well it does add it as an asset but doesn't copy or do anything to the font
function scr_compilefonts(){
	var assetidh = file_text_open_append(destination + "source/helpers/asset_toid.h");
	file_text_write_string(assetidh, "#define " + yyfile.name + " " + string(currentfont_count) + " //Font\n");
	file_text_close(assetidh);
	
	
	currentfont_count++;
}