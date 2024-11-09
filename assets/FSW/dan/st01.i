import "danmaku";

async MainSub00() {
	enmNewA("Spr00", 0, 128, 1000, 2);
}

void Spr00() {
	setSpir();

	loop wait(1000);
}

void main() {
	dvmFile("dan_enemy");
	dvmFile("st02");
	bgFile("st02");
	bgCall("main");
	wait(20);

	MainSub00();

	loop wait(1000);
}