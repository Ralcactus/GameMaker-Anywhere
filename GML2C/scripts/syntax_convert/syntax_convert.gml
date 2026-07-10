function syntax_convert(gmlfile_path){
	var newcode = "";
	
	//run through gobo (cleans code up by adding ";" "if (bleh == true)" etc)
	newcode = gobo_cleaner(gmlfile_path);

	//here will be for c++ speciifc stuff like decimals with "0.2f"

	return newcode; 
}