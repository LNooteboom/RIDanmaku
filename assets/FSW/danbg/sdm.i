import "bg";

async testBgRock() {
	while (1) {
		float t = 300;
		upTime(t, SMOOTHSTEP, -0.1, 0, 1);
		wait(t);
		upTime(t, SMOOTHSTEP, 0.1, 0, 1);
		wait(t);
	}
}

void sdm1() {
	const int color = 0x800000;
	clearColorHex(color);
	fogColorHex(color);
	fogDist(30, 60);

	modelFile("mesh/SDM1.mes");
	dvmFile("danbg_sdm");
	dvm("bgSDM1Hall", 0, 0, 0);
	dvm("bgSDM1Hall", 0, 36, 0);
	dvm("bgSDM1Hall", 0, 36*2, 0);

	testBgRock();
	dir(0, 1.3, -0.3);
	fov(rad(70));
	loop {
		pos(-2, 0, 3);
		posTime(400, LINEAR, -2, 36, 3);
		wait(400);
	}
}