function gobo_cleaner(gmlfile_path){
	var goboWin = run_commandpowershell("C:/", working_directory+"Other/gobo.exe --write-stdout --skip-write '" + gmlfile_path + "'", false); //run gobo
	var output = ExecutedProcessReadFromStandardOutput(goboWin); //read the output
	var startingpoint = string_length("Formatting " + filename_name(gmlfile_path))+4; //find the starting formatting start text and +4 to skip the "l..."
	var fullstring_pt1 = string_copy(output, startingpoint, string_length(output) - startingpoint + 1); //copy the output besides the formating start text

	FreeExecutedProcessStandardOutput(goboWin);
	FreeExecutedProcessStandardInput(goboWin);	

	//delete the timer text
	var lastD = string_last_pos("D", fullstring_pt1); //find the D in "Done in 13 ms"
	var fullstring_pt2 = string_delete(fullstring_pt1, lastD, string_length(fullstring_pt1) - lastD + 1); //Delete everything in front

	return fullstring_pt2;
} 