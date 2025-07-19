import "danmaku";

void Boss() {
	setSFGirl();
	dvmSet(1, "effHex");
	setBoss(0, "Test", "");
	invuln(60);
	distort(160, 0xFF80FF);

	lifeSet(8600);
	lifeMarker(0, 1400, 0xFFFFA080);
	setInterrupt(0, 1400, 25 * 60, "BossCard1");
	stars(5);

	movePosTime(60, EASEOUT1, 0, 128);
	wait(60);

	Boss1();

	loop wait(1000);
}

void Boss1() {
	CAPTURE = 1;
	moveLimit(0, 128, 256, 64);

	loop {
		Boss1Shoot(1);
		Boss1ShootB(1);
		wait(60);
		Boss1Shoot(-1);
		Boss1ShootB(-1);
		wait(60);
		moveRand(30, EASEOUT1, 5);
		wait(30);
		Boss1Shoot2();
	}
}

async Boss1Shoot(float dir) {
	bmSet(0, AIM_ST, B16A, RED, 3, 1, 0, rad(20), 1, 1);
	bmDist(0, 32);
	bmCloud(0, CLOUD_MEDIUM);
	
	int n = 20;
	float rd = rad(10) * dir;
	float r = AIM - rd * ((n - 1) / 2.0);
	float s = 1;
	loop (n) {
		bmAngle(0, r, rad(30) + rad(5) * RF);
		bmAccelSet(0, 1, TRANS_ASYNC, 40, s / 40, NEGF);
		bmShoot(0);
		wait(1);

		r += rd;
		s += 0.2;
	}
}
async Boss1ShootB(float dir) {
	bmSet(1, AIM_RAND, B32A, RED32, 1, 2, 0, rad(40), 1, 0);
	bmDist(1, 32);
	bmCloud(1, CLOUD_MEDIUM);

	wait(5);

	int n = 19;
	float rd = rad(10) * dir;
	float r = AIM - rd * ((n - 1) / 2.0);
	float s = 0.5;
	loop(n) {
		bmAngle(1, r, rad(5));
		bmAccelSet(1, 1, TRANS_ASYNC, 40, s / 40, NEGF);
		bmShoot(1);
		wait(1);

		r += rd;
		s += 0.2;
	}
}
void Boss1Shoot2() {
	bmSet(0, AIM_ST, B16D, DARKRED, 1, 1, AIM, rad(20), 2, 1);
	bmDist(0, 48);
	bmCloud(0, CLOUD_MEDIUM);
	bmWait(0, 30);
	bmAccel(0, TRANS_WAIT, 180, -1.0 / 180, NEGF);
	int c = 1;
	loop (10) {
		bmCount(0, c, 1);
		loop(3) {
			bmShoot(0);
			wait(3);
		}

		c += 1;
	}
}


void main() {
	dvmFile("dan_enemy");
	dvmFile("st02");
	bgFile("st02");
	bgCall("main");
	wait(20);

	enmNewA("Boss", -128, -32, 1000, 0);

	//wait(300);
	//end();

	loop wait(1000);
}