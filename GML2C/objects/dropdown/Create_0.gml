draw = {
	su : noone,				// surface
	re : false,				// refresh surface
	sx : 64,				// start x
	sy : 64,				// start y
	eh : 38,				// line height
	dw : 160,				// draw width
	dh : 152,				// draw height
	oy : 0,					// offset y
	ft : noone,				// font
	tc : $000000,			// text color
	bc : $ffffff,			// background color
	hc : $dddddd,			// hover color
	pa : 10,				// padding
	bo : -1,				// border color
	my : 0					// mouse y
}

curt = {
	fc : true,				// focus
	pf : noone,				// prev focus
	sc : noone,				// script
	ar : [],				// args
	cu : 0,					// current
	li : [],				// list
	le : 0,					// length
	br : 0
}

#region functions

	dropdown_refresh = function() {

		var eh = draw.eh,
			dh = draw.dh - eh,
			oy = draw.oy,
			ch = curt.cu * eh,
			oh = ch - oy;

		if (oh < 0) oy = max(0, ch);
		else if (oh > dh) oy = ch - dh;

		draw.oy = oy;
		draw.re = true;

	}

	dropdown_cinput = function(c) {
	
		var le = curt.le - 1,
			vi = curt.cu + c;
	
		if (vi > le) vi = 0;
		else if (vi < 0) vi = le;
		curt.cu = vi;

		dropdown_refresh();

	}

#endregion