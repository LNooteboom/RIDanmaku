import "bg";

void PlaceSection(float y) {
	dvm("bhaBgRoad", 0, y, 0);
	dvm("bhaBgHouse1", 7, y - 11, 0);
	dvm("bhaBgHouse2", 8, y - 3, 0);
	dvm("bhaBgAlley", 7, y + 1, 0);
	dvm("bhaBgHouse3", 8, y + 6, 0);
	dvm("bhaBgHouse4", 8, y + 13, 0);

	dvm("bhaBgHouse1", -7, y + 11, 0);
	dvmRotate(DVM_COUNT - 1, 0, 0, rad(180));
	dvm("bhaBgHouse2", -8, y + 3, 0);
	dvmRotate(DVM_COUNT - 1, 0, 0, rad(180));
	dvm("bhaBgAlley", -7, y - 1, 0);
	dvmRotate(DVM_COUNT - 1, 0, 0, rad(180));
	dvm("bhaBgHouse3", -8, y - 6, 0);
	dvmRotate(DVM_COUNT - 1, 0, 0, rad(180));
	dvm("bhaBgHouse4", -8, y - 13, 0);
	dvmRotate(DVM_COUNT - 1, 0, 0, rad(180));
}

void main() {
	clearColorHex(0);
	fogColorHex(0);
	fogDist(2, 48);
	dvmFile("bha");
	modelFile("mesh/bha.mes");

	dvm("bhaBgImg", 0, 0, 0);
	PlaceSection(0);
	PlaceSection(32);
	PlaceSection(64);

	up(0, 0, 1);
	fov(rad(40));
	pos(-2, -16, 48);
	dir(0, 0.6, -1);

	int rck = rockingA(240, 0, rad(5));

	posTime(400, LINEAR, 0, 16, 28);
	wait(400);

	pos(0, -16, 28);
	posTime(500, LINEAR, 0, 16, 8);
	wait(120);
	dirTime(380, SMOOTHSTEP, 0, 1, -0.65);
	fovTime(380, SMOOTHSTEP, rad(70));
	wait(380);
	//dirTime(120, SMOOTHSTEP, 0, 1, 0.1);

	int partIdx = DVM_COUNT;
	dvm("bhaBgParticles", 0, 0, 0);

	loop (9) {
		pos(0, -16, 8);
		posTime(600, LINEAR, 0, 16, 8);
		wait(600);
	}

	dvmDelete(partIdx);
	kill(rck);
	upTime(60, SMOOTHSTEP, 0, 0, 1);

	pos(0, -16, 8);
	posTime(600, EASEOUT1, 0, 16, 12);
	dirTime(500, SMOOTHSTEP, 0, 1, 0.1);
	fovTime(500, SMOOTHSTEP, rad(60));

	loop wait(1000);
}

void boss() {
	killAll();
	pos(0, 16, 12);
	dir(0, 1, 0.1);
	up(0, 0, 1);
	fov(rad(60));

	posTime(300, EASEIN1, 0, -16, 8);
	dirTime(300, SMOOTHSTEP, 0, 1, -0.1);
	wait(300);
	loop {
		pos(0, 16, 8);
		posTime(120, LINEAR, 0, -16, 8);
		wait(120);
	}
}