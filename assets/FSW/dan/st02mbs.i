import "danmaku";

void MBoss() {
	dvmSetMove(0, "st2Boss");
	dvmSet(1, "effHex");
	hurtbox(40, 40);
	hitbox(16, 16);
	setBoss(0, "Daiyousei", "");
	lifeSet(7600);
	setInterrupt(0, 1200, 1800, "MBossCard1");
	setTimeout(0, "MBossEscape");
	lifeMarker(0, 1200, 0xFFFFA080);

	enmKillAll();
	bmClear(640, 0);
	CAPTURE = 1;
	distort(160, 0xFF80FF);

	movePosTime(60, EASEOUT1, 0, 128);
	wait(60);
	moveLimit(0, 128, 256, 64);

	loop{
		dvmEvent(0, 6);
		wait(12);
		MBoss1_at();
		dvmEvent(0, 7);
		wait(60);

		dvmEvent(0, 8);
		wait(30);
		movePos(0 + RF2 * 128, 128, 0);
		dvmEvent(0, 9);
		wait(30);

		MBoss1_at2();
		wait(5);
	}

	loop wait(1000);
}

void MBoss1_at() {
	bmSet(0, AIM_ST_R, B16D, GREEN, 1, diffI(4, 6, 8, 12), 0, 0, diffF(3, 4.5, 6.5, 8), diffF(2, 2, 3, 3));
	bmCloud(0, CLOUD_MEDIUM);

	float a = rad(-90);
	int cnt = diffI(12, 16, 20, 26);
	float ad = 2 * PI / cnt;
	loop(cnt) {
		bmAngle(0, a, 0);
		bmShoot(0);
		a += ad;
		wait(1);
	}
	a += ad * 0.5f;
	bmSprite(0, B16D, DARKGREEN);
	loop(cnt) {
		bmAngle(0, a, 0);
		bmShoot(0);
		a -= ad;
		wait(1);
	}

	wait(30);
	a = rad(-90);
	bmSprite(0, B16D, RED);
	loop(cnt) {
		bmAngle(0, a, 0);
		bmShoot(0);
		a -= ad;
		wait(1);
	}
	a += ad * 0.5f;
	bmSprite(0, B16D, DARKRED);
	loop(cnt) {
		bmAngle(0, a, 0);
		bmShoot(0);
		a += ad;
		wait(1);
	}
}

void MBoss1_at2() {
	bmSet(1, AIM_ST, B16E, BLUE, 1, 1, AIM + rad(60), rad(10), 5, 1);
	bmCloud(1, CLOUD_MEDIUM);
	bmStep(1, TRANS_WAIT, 60, 1, STEP_ANGREL, 0, 0);
	bmStep(1, TRANS_WAIT, 60, 1, STEP_ANGPL, rad(0), 3);

	int cnt = 1;
	float spd = 5.5;
	loop(diffI(4, 6, 7, 10)) {
		bmCount(1, cnt, 1);
		bmSpeed(1, spd, 1);
		bmStepSet(1, 2, TRANS_WAIT, 30 + cnt * 3, 1, STEP_ANGPL, 0, 3);
		bmShoot(1);
		cnt++;
		spd -= 0.5;
	}

	bmAngle(1, AIM - rad(60), rad(10));
	cnt = 1;
	spd = 5;
	loop(diffI(4, 6, 7, 10)) {
		bmCount(1, cnt, 1);
		bmSpeed(1, spd, 1);
		bmStepSet(1, 2, TRANS_WAIT, 30 + cnt * 3, 1, STEP_ANGPL, 0, 3);
		bmShoot(1);
		cnt++;
		spd -= 0.5;
	}
}

void MBossCard1() {
	dvmEvent(0, 9);
	setInterrupt(0, 0, 30 * 60, "MBossEscape");
	spellE(SPELL_ST2_1E, 30 * 60, 500000, "st2CardA");
	flagSet(FLAG_RESIST);
	if (CAPTURE) {
		dropMain(IT_LIFEPIECE);
		dropItems();
	}
	CAPTURE = 1;
	killAll();
	breakWave();
	invuln(60);
	moveLimitReset();
	wait(30);

	dvmEvent(0, 8);
	wait(30);
	movePos(-64, 128, 0);
	dvmEvent(0, 9);
	wait(30);
	MBossCard1_at();
	dvmEvent(0, 6);
	sfx(SE_CHARGE);
	effChargeGreen(0, 0);
	wait(120);
	enmNew("MBossCard1At", 0, 0, 1000, 0);
	dvmEvent(0, 7);
	wait(400);

	dvmEvent(0, 8);
	wait(30);
	movePos(64, 128, 0);
	dvmEvent(0, 9);
	wait(30);
	MBossCard1_at();
	dvmEvent(0, 6);
	sfx(SE_CHARGE);
	effChargeGreen(0, 0);
	wait(120);
	enmNew("MBossCard1At", 0, 0, 1000, 0);
	dvmEvent(0, 7);
	wait(400);

	dvmEvent(0, 8);
	wait(30);
	movePos(0, 128, 0);
	dvmEvent(0, 9);
	wait(30);
	MBossCard1_at();
	dvmEvent(0, 6);
	sfx(SE_CHARGE);
	effChargeGreen(0, 0);
	wait(120);
	enmNew("MBossCard1At", 0, 0, 1000, 0);
	dvmEvent(0, 7);
	wait(400);

	loop wait(1000);
}

async MBossCard1_at() {
	bmSet(0, AIM_RAND, B8A, BLUE, 1, 1, 0, rad(180), 0, 0);
	bmProtect(0, 48);
	bmCloud(0, CLOUD_MEDIUM);
	bmWait(0, 120);
	bmAccel(0, TRANS_WAIT, 120, 1.0 / 120, NEGF);
	bmWait(0, 300);
	bmDelete(0, 1);

	int w = 120;
	float y = 8;
	float dst = diffF(64, 52, 48, 40);
	loop(14) {
		float x = -192;
		bmWaitSet(0, 1, w);
		loop(10) {
			bmOffsetAbs(0, x + RF2 * 16, y + RF2 * 16);
			bmShoot(0);
			x += dst;
		}
		w -= 3;
		y += dst;
		wait(3);
	}
}

void MBossCard1At() {
	flagSet(FLAG_NO_HIT | FLAG_NO_HURT);
	float xs = 0;
	float ys = 0;

	MBossCard1At_at();
	float w = diffF(120, 90, 70, 60);
	loop(diffI(3, 4, 5, 6)) {
		float xs2 = cos(AIM) * 256;
		float ys2 = sin(AIM) * 256;
		moveBezier(w, xs, ys, PLAYER_X, PLAYER_Y, xs2, ys2);
		xs = xs2;
		ys = ys2;
		wait(w);
	}
	die();
}

async MBossCard1At_at() {
	bmSet(0, AIM_ST_R, B16C, GREEN, 5, 1, 0, 0, 0, 0);
	bmDist(0, 8);
	bmCloud(0, CLOUD_LONG);
	bmWait(0, diffI(40, 60, 90, 120));
	bmDelete(0, 1);

	loop(120) {
		bmAngle(0, RRAD, 0);
		bmOffsetRad(0, RRAD, RF * 32);
		bmShoot(0);
		wait(3);
	}
}

void MBossEscape() {
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
	dropItems();

	dvmEvent(0, 9);
	lifeSet(100000);
	setBoss(-1, "", "");
	wait(90);

	movePosTime(60, EASEOUT1, -32, -32);
	wait(60);
	die();
}