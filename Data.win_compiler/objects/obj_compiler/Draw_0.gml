draw_set_font(-1);

draw_set_halign(fa_center);
draw_set_valign(fa_top);
draw_set_color(c_yellow);
draw_text(room_width/2, 20, "GameMaker Anywhere");
draw_set_color(c_white);

var btn_sel_hover = point_in_rectangle(mouse_x, mouse_y, 
    global.btn_select_x, global.btn_select_y, 
    global.btn_select_x + global.btn_select_w, 
    global.btn_select_y + global.btn_select_h);



draw_set_color(btn_sel_hover ? c_white : c_black);
draw_roundrect(global.btn_select_x, global.btn_select_y, 
               global.btn_select_x + global.btn_select_w, 
               global.btn_select_y + global.btn_select_h, false);
			   
draw_set_color(btn_sel_hover ? c_black : c_white);
draw_roundrect(global.btn_select_x, global.btn_select_y, 
               global.btn_select_x + global.btn_select_w, 
               global.btn_select_y + global.btn_select_h, true);
			   
draw_set_color(btn_sel_hover ? c_black : c_white);
draw_set_halign(fa_center);
draw_set_valign(fa_middle);
draw_text(global.btn_select_x + global.btn_select_w/2, 
          global.btn_select_y + global.btn_select_h/2, 
          "Select Project File");



if (btn_sel_hover && mouse_check_button_pressed(mb_left)) {
    var yyp = get_open_filename("GameMaker Project|*.yyp", "");
    
    if (yyp != "") {
        global.selected_yyp = yyp;
        
        //check version
        var yypbuffer = buffer_load(yyp);
        var yypdata = buffer_read(yypbuffer, buffer_string);
        buffer_delete(yypbuffer);
        var yyp_json = json_parse(yypdata);
        
        if (yyp_json.MetaData.IDEVersion != "2024.14.2.213") {
            show_message("ERROR!\nProject must be a 2024.14.2.213 project!");
            global.selected_yyp = "";
        }
    }
}

draw_set_halign(fa_left);
draw_set_valign(fa_top);
draw_set_color(c_ltgray);
if (global.selected_yyp != "") {
    draw_text(100, 150, "selected: " + filename_name(global.selected_yyp));
} else {
    draw_text(100, 150, "No Project Selected");
}

draw_set_halign(fa_left);
draw_set_valign(fa_top);
var fields = [
    {label: "Game Name:", value: global.game_name, field_id: 0},
    {label: "Title ID:", value: global.title_id, field_id: 1, hint: "(16 hex)"},
    {label: "Publisher:", value: global.publisher, field_id: 2}
];

for (var i = 0; i < array_length(fields); i++) {
    var field = fields[i];
    var field_y = global.field_start_y + (i * global.field_spacing);
    var field_hover = point_in_rectangle(mouse_x, 
	    mouse_y, global.field_x, field_y - 5,
        global.field_x + global.field_w, 
        field_y + global.field_height - 5);
    
    draw_set_color(c_white);
    draw_text(100, field_y, field.label);
    
    if (variable_struct_exists(field, "hint")) {
        draw_set_color(c_gray);
        draw_text(100, field_y + 20, field.hint);
    }
    
    var is_active = (global.current_field == field.field_id);
    draw_set_color(is_active ? c_yellow : (field_hover ? c_ltgray : c_white));
    draw_roundrect(global.field_x, field_y - 5, 
                   global.field_x + global.field_w, 
                   field_y + global.field_height - 5, true);
    
    draw_set_color(c_white);
    var display_value = field.value;
    if (is_active && current_time % 1000 < 500) {
        display_value += "|";
    }
    draw_text(global.field_x + 8, field_y, display_value);
    if (field_hover && mouse_check_button_pressed(mb_left)) {
        global.current_field = field.field_id;
        keyboard_string = field.value;
    }
}

if (mouse_check_button_pressed(mb_left)) {
    var clicked_field = false;
    for (var i = 0; i < array_length(fields); i++) {
        var field_y = global.field_start_y + (i * global.field_spacing);
        if (point_in_rectangle(mouse_x, 
		    mouse_y, global.field_x, field_y - 5,
            global.field_x + global.field_w, 
            field_y + global.field_height - 5)) {
            clicked_field = true;
            break;
        }
    }
    if (!clicked_field && !btn_sel_hover) {
        global.current_field = -1;
    }
}


var btn_exp_hover = point_in_rectangle(mouse_x, 
    mouse_y, global.btn_export_x, global.btn_export_y,
    global.btn_export_x + global.btn_export_w,
    global.btn_export_y + global.btn_export_h);


var can_export = (global.selected_yyp != "" && 
    global.game_name != "" && 
    global.title_id != "" && 
    string_length(global.title_id) == 16 &&
    global.publisher != "");

if (can_export) {
    draw_set_color(btn_exp_hover ? c_lime : c_green);
} else {
    draw_set_color(c_dkgray);
}
draw_roundrect(global.btn_export_x, global.btn_export_y,
    global.btn_export_x + global.btn_export_w,
    global.btn_export_y + global.btn_export_h, false);

draw_set_color(c_white);
draw_roundrect(global.btn_export_x, global.btn_export_y,
    global.btn_export_x + global.btn_export_w,
    global.btn_export_y + global.btn_export_h, true);

draw_set_halign(fa_center);
draw_set_valign(fa_middle);
draw_text(global.btn_export_x + global.btn_export_w/2,
    global.btn_export_y + global.btn_export_h/2,
    "Export");

if (can_export && btn_exp_hover && mouse_check_button_pressed(mb_left)) {
    global.exporting = true;
    var yyp = global.selected_yyp;
    
    // create folders for exports
    var export_root  = filename_dir(yyp) + "\\3DS Data Exported\\";
    var sprites_root = export_root + "gfx\\";
    
    directory_create(export_root);
    directory_create(sprites_root);
    
    // create sprites.t3s
    var t3s_path = sprites_root + "sprites.t3s";
    var t3s_file = file_text_open_write(t3s_path);
    file_text_write_string(t3s_file, "--atlas\n");
    
    // json parse
    var yypbuffer = buffer_load(yyp);
    var yypdata = buffer_read(yypbuffer, buffer_string);
    buffer_delete(yypbuffer);
    var yyp_json = json_parse(yypdata);
    
    // get the first room
    var first_room_name = yyp_json.RoomOrderNodes[0].roomId.name;
    var first_room_path = yyp_json.RoomOrderNodes[0].roomId.path;
    
    // get all resources
    var all_resources = [];
    var all_rooms = [];
    var all_sprites = [];
    var all_objects = [];
    
    var currentsprite_count = 0;
    
    for (var i = 0; i < array_length(yyp_json.resources); i++) {
        var _id = yyp_json.resources[i].id;
        array_push(all_resources, {name: _id.name});
        
        var file_buffer = buffer_load(filename_dir(yyp) + "/" + _id.path);
        var yyfileFAKE = buffer_read(file_buffer, buffer_string);
        buffer_delete(file_buffer);
        yyfileFAKE = string_replace_all(yyfileFAKE, ",}", "}");
        yyfileFAKE = string_replace_all(yyfileFAKE, ",]", "]");
        var yyfile = json_parse(yyfileFAKE);
        
        var IsRoom = (yyfile.resourceType == "GMRoom");
        var IsSprite = (yyfile.resourceType == "GMSprite");
        var IsObject = (yyfile.resourceType == "GMObject");
        
        if (IsRoom) {
            show_debug_message("Room: " + yyfile.name);
            
            var packed_layers = [];
            
            for (var L = 0; L < array_length(yyfile.layers); L++) {
                var _layer = yyfile.layers[L];
                
                var layer_out = {
                    type: _layer.resourceType,
                    name: _layer.name,
                    depth: (variable_struct_exists(_layer, "depth") ? _layer.depth : 0),
                    visible: (variable_struct_exists(_layer, "visible") ? _layer.visible : true),
                };
                
                if (_layer.resourceType == "GMRInstanceLayer") {
                    layer_out.instances = [];
                    if (variable_struct_exists(_layer, "instances") && is_array(_layer.instances)) {
                        for (var j = 0; j < array_length(_layer.instances); j++) {
                            var instlayer = _layer.instances[j];
                            array_push(layer_out.instances, {
                                name: instlayer.name,
                                object: instlayer.objectId.name,
                                x: instlayer.x, y: instlayer.y,
                                scaleX: instlayer.scaleX, scaleY: instlayer.scaleY,
                                rotation: instlayer.rotation
                            });
                        }
                    }
                }
                
                if (_layer.resourceType == "GMRAssetLayer") {
                    layer_out.assets = [];
                    if (variable_struct_exists(_layer, "assets") && is_array(_layer.assets)) {
                        for (var j = 0; j < array_length(_layer.assets); j++) {
                            var assetlayer = _layer.assets[j];
                            
                            if (assetlayer.resourceType == "GMRSpriteGraphic") {
                                array_push(layer_out.assets, {
                                    type: assetlayer.resourceType,
                                    sprite: assetlayer.spriteId.name,
                                    x: assetlayer.x, 
                                    y: assetlayer.y,
                                    scaleX: assetlayer.scaleX, scaleY: assetlayer.scaleY,
                                    rotation: assetlayer.rotation
                                });
                            }
                        }
                    }
                }
                
                if (_layer.resourceType == "GMRBackgroundLayer") {
                    layer_out.background = {
                        colour: _layer.colour,
                        sprite: (_layer.spriteId != undefined && _layer.spriteId != null ? _layer.spriteId.name : "")
                    };
                }
                
                array_push(packed_layers, layer_out);
            }
            
            array_push(all_rooms, {
                name: yyfile.name,
                width: yyfile.roomSettings.Width,
                height: yyfile.roomSettings.Height,
                layers: packed_layers
            });
        }
        
        if (IsSprite) {
            show_debug_message("Sprite: " + yyfile.name);
            
            directory_create(sprites_root + yyfile.name + "\\");
            
            var sprite_rel_dir = "sprites/" + yyfile.name + "/";
            var sprite_frames = [];
            
            for (var f = 0; f < array_length(yyfile.frames); f++) {
                var frame_name = yyfile.frames[f].name;
                array_push(sprite_frames, frame_name);
                
                file_copy(filename_dir(yyp) + "\\sprites\\" + yyfile.name + "\\" + frame_name + ".png", 
                         sprites_root + yyfile.name + "\\" + frame_name + ".png");
                file_text_write_string(t3s_file, yyfile.name + "/" + frame_name + ".png\n");
            }
            
            array_push(all_sprites, {
                name: yyfile.name,
                dir: sprite_rel_dir,
                frames: sprite_frames,
                width: yyfile.width,
                height: yyfile.height,
                origin: yyfile.origin,
                xorigin: yyfile.sequence.xorigin,
                yorigin: yyfile.sequence.yorigin,
                SpriteNumber: currentsprite_count
            });
            
            currentsprite_count++;
        }
        
        if (IsObject) {
            show_debug_message("Object: " + yyfile.name);
            
            var spr_name = "";
            
            if (variable_struct_exists(yyfile, "spriteId")) {
                var sid = yyfile.spriteId;
                if (is_struct(sid) && variable_struct_exists(sid, "name"))
                    spr_name = sid.name;
            }
            
            var steppath = filename_dir(filename_dir(yyp) + "/" + _id.path) + "/Step_0.gml";
            var createpath = filename_dir(filename_dir(yyp) + "/" + _id.path) + "/Create_0.gml";
            
            var create_code = "";
            var step_code = "";
            
            if (file_exists(createpath)) {
                var createinside_buffer = buffer_load(createpath);
                create_code = buffer_read(createinside_buffer, buffer_string);
                buffer_delete(createinside_buffer);
                show_debug_message(create_code);
            }
            
            if (file_exists(steppath)) {
                var createinside_buffer = buffer_load(steppath);
                step_code = buffer_read(createinside_buffer, buffer_string);
                buffer_delete(createinside_buffer);
                show_debug_message(step_code);
            }
            
            array_push(all_objects, {
                name: yyfile.name,
                sprite: spr_name,
                CreateCode: create_code,
                StepCode: step_code
            });
        }
    }
    
    // export json
    var export_json = {
        GameMetadata: {
            name: global.game_name,
            titleId: global.title_id,
            publisher: global.publisher
        },
        
        FirstRoom: {
            name: first_room_name,
        },
        
        Resources: {
            all_resources
        },
        
        Rooms: {
            all_rooms
        },
        
        Sprites: {
            all_sprites
        },
        
        Objects: { 
            all_objects
        }
    };
    
    var file = file_text_open_write(export_root + "data.win");
    file_text_write_string(file, json_stringify(export_json, true));
    file_text_close(file);
    file_text_close(t3s_file);
    
    var bat = file_text_open_write(export_root + "build_sprites_t3x.bat");
    file_text_write_string(bat, "@echo off\n");
    file_text_write_string(bat, "cd /d \"%~dp0gfx\"\n");
    file_text_write_string(bat, "\"C:\\devkitPro\\tools\\bin\\tex3ds.exe\" -i sprites.t3s -o sprites.t3x\n");
    file_text_write_string(bat, "echo Done.\n");
    file_text_write_string(bat, "pause\n");
    file_text_close(bat);
    
    show_message("Export Successful!\n\n" +
                "Game: " + global.game_name + "\n" +
                "Title ID: " + global.title_id + "\n" +
                "Publisher: " + global.publisher + "\n\n" +
                "Files exported to:\n" + export_root);
    
    game_end();
}

draw_set_halign(fa_center);
draw_set_valign(fa_top);
draw_set_color(c_red);

if (global.selected_yyp == "") {
    draw_text(room_width/2, 510, "Please select a project file");
} else if (global.game_name == "") {
    draw_text(room_width/2, 510, "Please enter a game name");
} else if (global.title_id == "") {
    draw_text(room_width/2, 510, "Please enter a Title ID");
} else if (string_length(global.title_id) != 16) {
    draw_text(room_width/2, 510, "ID must be 16 hex digits (current: " + string(string_length(global.title_id)) + ")");
} else if (global.publisher == "") {
    draw_text(room_width/2, 510, "Please enter a publisher name");
} else {
    draw_set_color(c_lime);
    draw_text(room_width/2, 510, "Export Ready");
}
draw_set_halign(fa_left);
draw_set_valign(fa_top);
draw_set_color(c_white);
