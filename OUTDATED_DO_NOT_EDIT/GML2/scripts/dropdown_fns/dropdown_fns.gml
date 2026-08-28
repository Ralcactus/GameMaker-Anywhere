
/// @param x
/// @param y
/// @param width
/// @param height
/// @param array
/// @param script
/// @param args
/// @param initial_index
/// @param line_height
function dropdown_create(x, y, w, h, a, s, r, c, e) {

	var i = instance_create_depth(0, 0, depth - 1, dropdown),
		f = id;

	with (i) {
		var l = array_length(a);
		draw.dh = min(h, e * l);		// draw height
		draw.sx = x;					// start x
		draw.sy = y;					// start y
		draw.eh = e;					// line height
		draw.dw = w;					// draw width
		curt.pf = f;					// prev focus
		curt.sc = s;					// script
		curt.ar = r;					// args
		curt.cu = c;					// current
		curt.li = a;					// list
		curt.le = l;					// length
		dropdown_refresh();
	}
	
	return i;

}

/// @param instance
/// @param font
/// @param text_color
/// @param background_color
/// @param hover_color
/// @param padding
/// @param border_color
function dropdown_set_style(i, f, t, b, h, p = 10, r = -1) {

	with (i) {
		draw.ft = f;					// font
		draw.tc = t;					// text color
		draw.bc = b;					// background color
		draw.hc = h;					// hover color
		draw.pa = p;					// padding
		draw.bo = r;					// border color
		draw.re = true;
	}

}