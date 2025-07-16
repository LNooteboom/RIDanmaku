import "danmaku";

async MainSub00() {
	I1 = 8;  enmNewA("Girl00", 32, -32, 1200, GIRL_R); wait(10);
	//I1 = 4;  enmNewA("Girl00", 64, -32, 40, GIRL_B); wait(10);
	//I1 = 8;  enmNewA("Girl00", 96, -32, 40, GIRL_R); wait(10);
	//I1 = 12; enmNewA("Girl00", 128, -32, 40, GIRL_B); wait(10);
}

void Girl00() {
	setSFGirl();
	moveVel(rad(90), 5);
	wait(I1);
	moveVelTime(60, EASEOUT1, rad(180), 0);
	wait(60);

	Girl00_at();

	loop wait(1000);
}

async Girl00_at() {
	bmSet(0, AIM_AT, B16D, BLUE, 3, 1, rad(30), 0, 5, 1);
	bmCloud(0, CLOUD_MEDIUM);
	bmOffscreen(0, 90);
	
	loop(4) {
		float s = 3.0;
		float acc = 1.0 / 60;
		float r = rad(120);
		float r2 = rad(20);
		loop(3) {
			bmSpeed(0, s, 1);
			bmAngle(0, rad(0) - r, r2);
			bmStepSet(0, 2, TRANS_WAIT, 30, 1, STEP_ANGREL, rad(90), 1.0);
			bmAccelAngvSet(0, 3, TRANS_WAIT, 60, acc, rad(0.8));
			bmShoot(0);

			bmAngle(0, rad(0) + r, r2);
			bmStepSet(0, 2, TRANS_WAIT, 30, 1, STEP_ANGREL, rad(-90), 1.0);
			bmAccelAngvSet(0, 3, TRANS_WAIT, 60, acc, rad(-0.8));
			bmShoot(0);

			acc += 0.5 / 60;
			s += 1.0;
			wait(3);
		}
		//wait(120);
	}
}

void main() {
	dvmFile("dan_enemy");
	dvmFile("st02");
	bgFile("st02");
	bgCall("main");
	wait(20);

	MainSub00();

	wait(300);
	end();

	loop wait(1000);
}