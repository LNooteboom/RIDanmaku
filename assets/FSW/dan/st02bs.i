import "danmaku";

void Boss() {
	dvmSetMove(0, "st2Boss");
	dvmSet(1, "effHex");
	hurtbox(40, 40);
	hitbox(16, 16);
	setBoss(0, "Daiyousei", "");
	invuln(60);
	distort(160, 0xFF80FF);

	lifeSet(8000);
	movePosTime(60, EASEOUT1, 0, 128);
	wait(60);
	dialogWait();
	
	Boss1();
	//BossCard3();

	die();
}

// NON 1

void Boss1() {
	stars(2);
	lifeMarker(0, 1500, 0xFFFFA080);
	setInterrupt(0, 1500, 35 * 60, "BossCard1");
	moveLimit(0, 128, 256, 32);
	F1 = 1;
	loop{

		int n = diffI(4, 4, 5, 6);
		float rd = (2 * PI) / n;
		F0 = AIM + rd / 2;
		loop (n) {
			enmNew("Boss1At", 0, 0, 10000, GIRL_G);
			F0 += rd;
		}
		F1 = -F1;

		wait(120);
		moveRand(60, EASEOUT1, 1);
		wait(60);
	}
}

void Boss1At() {
	setGirl();
	invuln(60);
	lifeSet(10000);

	moveVel(F0, 0);
	moveVelTime(180, EASEOUT1, F0 - rad(180) * F1, 5);

	wait(10);

	Boss1At_at();

	loop wait(1000);
}

void Boss1At_at() {
	bmSet(0, AIM_ST_R, B16C, GREEN, 2, 1, F0, 0, 0, 0);
	bmDist(0, 16);
	bmCloud(0, CLOUD_MEDIUM);
	bmWait(0, 60);
	bmAccel(0, TRANS_WAIT, 60, 1.5 / 60, NEGF);

	int i = 0;
	loop (diffI(12, 20, 20, 20)) {
		bmSprite(0, B16C, i % 2 ? GREEN : BLUE);
		bmAngle(0, F0, 0);
		bmShoot(0);
		F0 += F1 * rad(15);
		i++;
		diffWait(10, 6, 6, 6);
	}
}

// CARD 1

void BossCard1() {
	setInterrupt(0, 0, 35 * 60, "Boss2");
	spellE(SPELL_ST2_1E, 35 * 60, 500000, "st2CardA");
	enmKillAll();
	killAll();
	invuln(60);
	movePosTime(60, EASEOUT1, 0, 128);
	moveLimit(0, 128, 256, 192);
	breakWave();
	if (CAPTURE) {
		dropMain(IT_LIFEPIECE);
		dropItems();
	}
	CAPTURE = 1;

	loop {
		float y = 16;
		F0 = 0;
		loop (7) {
			enmNewA("BossCard1At", -192, y + RF2 * 32, 10000, 0);
			y += 64;
		}
		F0 = rad(90);
		y = -128;
		loop(6) {
			enmNewA("BossCard1At", y + RF2 * 32, 0, 10000, 0);
			y += 64;
		}
		wait(120);
		BossCard1_at();
		wait(400);

		y = 16;
		F0 = 0;
		loop(7) {
			enmNewA("BossCard1At", -192, y + RF2 * 32, 10000, 0);
			y += 64;
		}
		F0 = rad(90);
		y = -128;
		loop(6) {
			enmNewA("BossCard1At", y + RF2 * 32, 0, 10000, 0);
			y += 64;
		}
		wait(120);
		BossCard1_at2();
		wait(400);
	}

	loop wait(1000);
}

async BossCard1_at() {
	bmSet(0, AIM_AT_R, B16B, GREEN, 5, 10, 0, PI / 64, 3, 2);
	bmCloud(0, CLOUD_MEDIUM);
	bmFast(0, TRANS_WAIT);

	loop(4) {
		bmAngle(0, 0, PI / 32);
		bmShoot(0);
		wait(20);

		bmAngle(0, 0, PI / -32);
		bmShoot(0);
		wait(20);
	}
}

async BossCard1_at2() {
	bmSet(0, AIM_AT_PYR, B16C, CYAN, 12, 5, 0, PI / 64, 2.5, 1.5);
	bmCloud(0, CLOUD_MEDIUM);
	bmFast(0, TRANS_WAIT);

	loop(8) {
		bmShoot(0);
		wait(20);
	}
}

void BossCard1At() {
	dvmSet(0, "enmMagic");
	flagSet(FLAG_NO_HIT | FLAG_NO_HURT);

	bmSet(0, AIM_ST, LINF, BLUE, 1, 1, F0, 0, 0, 0);
	bmLaserInfShoot(0, 0, 512, 16, 120, 10, 240, 10);
	sfx(SE_LASER1);
	wait(400);
	die();
}

// NON 2

void Boss2() {
	timerReset();
	bmClear(640, !TIMEOUT);
	spellEnd();
	lifeSet(8000);
	moveLimitReset();
	enmKillAll();
	killAll();

	stars(1);
	lifeMarker(0, 1500, 0xFFFFA080);
	setInterrupt(0, 1500, 35 * 60, "BossCard2");
	moveLimit(0, 128, 256, 32);
	if (CAPTURE) {
		dropMain(IT_LIFEPIECE);
		dropItems();
	}
	CAPTURE = 1;
	invuln(60);
	movePosTime(60, EASEOUT1, 0, 128);
	wait(90);

	F1 = 1;
	loop{

		int n = diffI(4, 4, 5, 6);
		float rd = (2 * PI) / n;
		F0 = AIM + rd / 2;
		loop(n) {
			enmNew("Boss2At", 0, 0, 10000, GIRL_Y);
			F0 += rd;
		}
		F1 = -F1;

		wait(120);
		moveRand(60, EASEOUT1, 1);
		wait(60);
	}
}

void Boss2At() {
	setGirl();
	invuln(60);
	lifeSet(10000);

	moveVel(F0, 0);
	moveVelTime(180, EASEOUT1, F0 - rad(180) * F1, 5);

	wait(10);

	Boss2At_at();

	loop wait(1000);
}

void Boss2At_at() {
	bmSet(0, AIM_ST_R, B16C, GREEN, 3, 1, F0, 0, 0, 0);
	bmDist(0, 16);
	bmCloud(0, CLOUD_MEDIUM);
	bmWait(0, 60);
	bmAccel(0, TRANS_WAIT, 60, 1.5 / 60, NEGF);

	int i = 0;
	int[] colors = [GREEN, LIME, YELLOW];
	loop(diffI(12, 20, 20, 20)) {
		bmSprite(0, B16C, colors[i % 3]);
		bmAngle(0, F0, 0);
		bmShoot(0);
		F0 += F1 * rad(3);
		i++;
		diffWait(10, 6, 6, 6);
	}
}

// SPELL 2

void BossCard2() {
	setInterrupt(0, 0, 35 * 60, "BossCard3");
	spellE(SPELL_ST2_1E, 35 * 60, 500000, "st2CardA");
	enmKillAll();
	killAll();
	invuln(60);
	movePosTime(60, EASEOUT1, 0, 128);
	moveLimit(0, 128, 256, 192);
	breakWave();
	if (CAPTURE) {
		dropMain(IT_LIFEPIECE);
		dropItems();
	}
	CAPTURE = 1;
	wait(60);

	loop{
		int cnt = diffI(4, 5, 6, 6);
		float rd = 2 * PI / cnt;
		F0 = AIM + rd / 2;
		loop(cnt) {
			enmNew("BossCard2At", 0, 0, 10000, 0);
			F0 += rd;
		}
		wait(300);
	}

	loop wait(1000);
}

void BossCard2At() {
	flagSet(FLAG_NO_HIT | FLAG_NO_HURT);
	moveVel(F0, 3);

	BossCard2At_at();

	loop wait(1000);
}

async BossCard2At_at() {
	wait(10);

	bmSet(0, AIM_ST_R, B16C, CYAN, 3, 1, 0, 0, 0, 0);
	bmDist(0, 12);
	bmCloud(0, CLOUD_MEDIUM);
	bmWait(0, 120);
	bmAccel(0, TRANS_WAIT, 60, 1.0 / 60, NEGF);

	bmSet(1, AIM_ST_R, B16A, BLUE, 1, 1, 0, 0, 0, 0);
	bmCloud(1, CLOUD_MEDIUM);
	bmWait(1, 120);
	bmAccel(1, TRANS_WAIT, 60, 1.5 / 60, NEGF);

	loop (20) {
		float r = RRAD, d = 32 * RF;
		bmOffsetRad(0, r, d);
		bmAngle(0, RRAD, 0);
		bmShoot(0);

		bmOffsetRad(1, r, d);
		bmAngle(1, RRAD, 0);
		bmShoot(1);

		//wait(6);
		diffWait(10, 8, 8, 6);
	}
}

void BossCard3() {
	timerReset();
	bmClear(640, !TIMEOUT);
	spellEnd();
	moveLimit(0, 128, 128, 64);
	enmKillAll();
	killAll();
	stars(0);
	lifeSet(1200);
	lifeMarker(0, 1200, 0xFFFFA080);
	if (CAPTURE) {
		dropMain(IT_LIFEPIECE);
		dropItems();
	}
	CAPTURE = 1;

	setInterrupt(0, 0, 35 * 60, "BossDead");
	spellE(SPELL_ST2_1E, 35 * 60, 500000, "st2CardA");

	invuln(100);
	movePosTime(60, EASEOUT1, 0, 128);
	wait(90);

	loop {
		BossCard3_at();
		wait(120);
		BossCard3_at2();
		wait(120);

		moveRand(60, LINEAR, 2);
		wait(60);

		loop(3) {
			enmNew("BossCard3At", 0, 0, 400, GIRL_B);
		}
	}

	loop wait(1000);
}

async BossCard3_at() {
	bmSet(0, AIM_ST_R, B16I, BLUE, 8, 1, 0, 0, 1.5, 1);
	bmSound(0, SE_SHOT1, -1);
	bmCloud(0, CLOUD_MEDIUM);
	bmAccelAngv(0, TRANS_WAIT, 20, 1.5 / 20, rad(90) / 20);
	bmAccelAngv(0, TRANS_WAIT, 120, 0, PI / 64);
	bmChangeSprite(0, B16C, BLUE);
	bmPlaySound(0, TRANS_WAIT, SE_KIRA1);
	bmAccelAngv(0, TRANS_WAIT, 60, -1.5 / 60, PI / -128);

	bmSet(1, AIM_ST_R, B16I, GREEN, 8, 1, 0, 0, 1.5, 1);
	bmSound(1, SE_SHOT1, -1);
	bmCloud(1, CLOUD_MEDIUM);
	bmAccelAngv(1, TRANS_WAIT, 20, 1.5 / 20, rad(-90) / 20);
	bmAccelAngv(1, TRANS_WAIT, 120, 0, PI / -64);
	bmChangeSprite(1, B16C, GREEN);
	bmPlaySound(1, TRANS_WAIT, SE_KIRA1);
	bmAccelAngv(1, TRANS_WAIT, 60, -1.5 / 60, PI / 128);

	loop(12) {
		bmAngle(0, RRAD, 0);
		bmShoot(0);

		bmAngle(1, RRAD, 0);
		bmShoot(1);
		wait(3);
	}
}

async BossCard3_at2() {
	bmSet(2, AIM_ST_R, B16E, BLUE, 7, 1, 0, 0, 3.0, 1);
	bmCloud(2, CLOUD_MEDIUM);
	bmStep(2, TRANS_WAIT, 60, 1, STEP_ANGREL, rad(-20), 3.0);

	float r2 = RRAD;
	float r = r2;
	loop(16) {
		bmAngle(2, r, 0);
		bmShoot(2);
		//bmAngle(2, -r, 0);
		//bmShoot(2);
		r += PI / 128;
		wait(2);
	}

	bmStepSet(2, 1, TRANS_WAIT, 60, 1, STEP_ANGREL, rad(-20), 3.0);
	r = r2;
	loop(16) {
		bmAngle(2, r, 0);
		bmShoot(2);
		r -= PI / 128;
		wait(2);
	}
}

void BossCard3At() {
	setGirl();
	moveVel(RRAD, 3);
	moveVelTime(60, LINEAR, NEGF, 0);
	wait(60);

	BossCard3At_at();

	wait(120);
	moveVel(rad(-90), 0);
	moveVelTime(60, LINEAR, NEGF, 3);

	loop wait(1000);
}

async BossCard3At_at() {
	bmSet(0, AIM_AT, B64A, BLUE64, 3, 1, 0, rad(30), 3, 1);
	bmCloud(0, CLOUD_MEDIUM);
	
	loop(5) {
		bmShoot(0);
		wait(20);
	}
}

void BossDead() {
	clearInterrupt(0);
	spellEnd();
	moveLimitReset();
	distort(0, 0xFF80FF);
	enmKillAll();
	if (TIMEOUT) {
		breakWaveNi();
	} else {
		breakWave();
		dropClear();
		if (CAPTURE)
			dropMain(IT_LIFEPIECE);
		dropExtra(IT_STAR, 4);
	}
	dropExtra(IT_POWER, 10);
	dropExtra(IT_POINT, 10);
	dropArea(64, 64);
	//dropItems();
	setBoss(-1, "", "");

	dvmPlay("effExplodeInvert", 0, 0, 0);
	wait(90);
	die();
}