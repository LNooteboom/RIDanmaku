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

	Boss1Card();

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
	bmDist(0, 16);
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




void Boss1Card() {
	enmKillAll();
	killAll();
	invuln(60);
	movePosTime(60, EASEOUT1, 0, 128);
	//moveLimit(0, 128, 256, 96);
	moveLimitReset();
	breakWave();
	if (CAPTURE) {
		dropMain(IT_LIFEPIECE);
		dropItems();
	}
	CAPTURE = 1;
	wait(90);

	bossCard1Shoot();
	wait(60);

	loop{
		//movePosTime(60, EASEOUT1, PLAYER_X, PLAYER_Y);
		wait(60);
	}

	loop wait(1000);
}

void buAddSpeed(entity en, float x, float y, float fric) {
	float xs = 0, ys = 0, spd = 0;
	bulletGetAngSpd(en, xs, ys, spd);
	xs = xs * spd + x;
	ys = ys * spd + y;
	if (abs(xs) + abs(ys) > 0.0001) {
		spd = sqrt(xs * xs + ys * ys);
		xs /= spd;
		ys /= spd;
		if (spd > fric)
			bulletSetAngSpd(en, xs, ys, spd - fric);
		else
			bulletSetAngSpd(en, xs, ys, 0);
	}
	else {
		bulletSetAngSpd(en, 1, 0, 0);
	}
}

inline void spring(ref float forceX, ref float forceY, float dx, float dy, float idleLen) {
	float springK = 1; // Force per pixel extended
	float dst = sqrt(dx * dx + dy * dy);
	if (dst < idleLen) {
		forceX = 0;
		forceY = 0;
	} else {
		float coeff = (dst - idleLen) * springK / dst;
		forceX = dx * coeff;
		forceY = dy * coeff;
	}

}

async bossCard1Shoot() {
	bmSet(0, AIM_ST, B16B, RED, 1, 1, rad(90), 0, 0, 0);
	bmCloud(0, CLOUD_MEDIUM);
	bmOffscreen(0, 30000);
	bmInvuln(0, TRANS_ASYNC, 300);

	int chainLen = 6;

	float aim = AIM + rad(180);
	bmOffsetRad(0, aim, 16);
	entity chain = [bmShootSingle(0)];
	for (int i = 1; i < chainLen; i++) {
		bmOffsetRad(0, aim, 16 + i * 16);
		chain[i] = bmShootSingle(0);
		wait(3);
	}
	bmSprite(0, B64A, RED64);
	bmOffsetRad(0, aim, 32 + chainLen * 16);
	bmAccel(0, TRANS_WAIT, 10, 10.0 / 10, RRAD);
	chain[chainLen] = bmShootSingle(0);

	// Force = mass * (pixels/frame^2) -> accel = Force / mass
	float massBall = 30;
	float springIdleLen1 = 16, springIdleLen2 = 32;
	float c1spd = 1;
	loop{
		float rr, ri, s;
		bulletGetAngSpd(chain[0], rr, ri, s);
		if (ri < 0 && c1spd >= 0) {
			//bulletSetAngSpd(chain[0], 0, 1, 10);
		}
		c1spd = ri;

		float leftX = POS_X, leftY = POS_Y;
		for (int i = 0; i < chainLen + 1; i++) {
			float curX = 0, curY = 0;
			bulletGetPos(chain[i], curX, curY);

			float forceX, forceY;
			float fric = 0.1;
			if (i == chainLen) {
				spring(forceX, forceY, curX - leftX, curY - leftY, springIdleLen2);
				buAddSpeed(chain[i], forceX / -massBall, forceY / -massBall, 0);
			} else {
				spring(forceX, forceY, curX - leftX, curY - leftY, springIdleLen1);
				buAddSpeed(chain[i], -forceX, -forceY, fric);
			}
			if (i) {
				/* Apply equal and opposite force */
				buAddSpeed(chain[i - 1], forceX, forceY, 0);
			}

			leftX = curX;
			leftY = curY;
		}

		wait(1);
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