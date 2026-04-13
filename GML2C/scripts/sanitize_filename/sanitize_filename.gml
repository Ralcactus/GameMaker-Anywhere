function sanitize_filename(name) {
    var invalid = ["/", "\\", ":", "*", "?", "\"", "<", ">", "|"];
    
    for (var i = 0; i < array_length(invalid); i++) {
        name = string_replace_all(name, invalid[i], "_");
    }
    
    return name;
}
