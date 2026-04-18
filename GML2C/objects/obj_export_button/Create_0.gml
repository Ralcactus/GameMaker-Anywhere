draw = {
	sx : 480,					// start x
	sy : 240,					// start y
	ex : 800,					// end x
	ey : 278					// end y
}

curt = {
	cu : 0,						// current
	li : noone,					// list
	dd : false
}

function example_selected(i) {

	with (curt.pf) {
		curt.cu = i;
		curt.dd = false;
	}

}

curt.li = ["3DSX", "CIA"];
