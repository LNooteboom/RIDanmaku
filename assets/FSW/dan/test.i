import "danmaku";

void Enm1() {
	setGirl();

	bmSet(0, AIM_ST_R, B16A, BLUE, 10, 1, 0, 0, 3, 1);
	loop{
		bmShoot(0);
		wait(30);
	}

	loop wait(1000);
}

void main() {
	dvmFile("dan_enemy");
	dvmFile("st02");
	bgFile("st02");
	bgCall("main");
	wait(20);

	enmNewA("Enm1", 0, 128, 100, GIRL_R);
	
	loop wait(1000);
}