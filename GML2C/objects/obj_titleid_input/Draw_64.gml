if (!active)
	image_blend = c_gray;
else
	image_blend = c_white;

draw_sprite_ext(sprite_index, image_index, x, y, image_xscale, image_yscale, image_angle, image_blend, image_alpha);

draw_set_halign(fa_left);
draw_set_valign(fa_top);
draw_set_color(c_white);

draw_text(x+5, y-20, "Title ID");

if (current_time % 1000 < 500 && active)
    draw_text(x+5, y+5, global.title_id + "|");
else
    draw_text(x+5, y+5, global.title_id);

if (string_length(global.title_id) == 0 || string_length(global.title_id) == 1 && global.title_id == "|")
	draw_text_colour(x+12,y+6, "0000000000000000", c_gray, c_gray, c_gray, c_gray, 1);
