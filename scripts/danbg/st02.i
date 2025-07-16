import "bg";

void PlaceSection(float y) {
	dvm("st2BgWater", 0, y + 16, 0);
	dvm("st2BgLeaves", 0, y + 16, 0);

	dvm("st2BgCloud1", 2, y + 10, 4);
	dvm("st2BgCloud1", 4, y + 12, 2);
	dvm("st2BgCloud1", 0, y + 18, 2);
	dvm("st2BgCloud1", -4, y + 24, 4);

	dvm("st2BgCloud2", 0, y + 8, 2);
	dvm("st2BgCloud2", -4, y + 8, 4);
	dvm("st2BgCloud2", 2, y + 22, 4);
	dvm("st2BgCloud2", -2, y + 28, 2);
}

void main() {
	clearColorHex(0x000C30);
	fogColorHex(0x000C30);
	fogDist(2, 24);
	dvmFile("st02");
	modelFile("mesh/st2.mes");
	
	PlaceSection(0);
	PlaceSection(32);
	PlaceSection(64);
	PlaceSection(96);

	fov(rad(60));
	pos(0, 0, 16);
	dir(0, 0.7, -1);
	posTime(500, LINEAR, 0, 16, 8);
	dirTime(500, EASEOUT1, 0, 0.5, -0.5);
	fovTime(500, EASEOUT1, rad(45));
	wait(500);
	posTime(1000, LINEAR, 0, 32, 8);
	wait(1000);

	loop{
		pos(0, 32, 8);
		posTime(4000, LINEAR, 0, 96, 8);
		wait(4000);
	}

	loop wait(1000);
}