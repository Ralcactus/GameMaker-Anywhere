if (!active)
	image_blend = c_gray;
else
	image_blend = c_white;

draw_sprite_ext(sprite_index, image_index, x, y, image_xscale, image_yscale, image_angle, image_blend, image_alpha);

draw_set_halign(fa_left);
draw_set_valign(fa_top);
draw_set_color(c_white);

draw_text(x+5, y-20, "Game Name");
if (current_time % 1000 < 500 && active)
    draw_text(x+5, y+5, global.game_name + "|");
else
    draw_text(x+5, y+5, global.game_name);
