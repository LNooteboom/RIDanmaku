import "danmaku";

void MBoss() {
	dvmSet(0, "bhaBoss");
	dvmSet(1, "effHex");
	hurtbox(40, 40);
	hitbox(16, 16);
	setBoss(0, "Miyoi Okunoda", "");
	lifeSet(7800);
	setInterrupt(0, 1400, 25 * 60, "MBossCard1");
	setTimeout(0, "MBossEscape");
	lifeMarker(0, 1400, 0xFFFFA080);

	enmKillAll();
	bmClear(640, 0);
	CAPTURE = 1;
	distort(160, 0xFF80FF);

	movePosTime(60, EASEOUT1, 0, 128);
	wait(60);
	moveLimit(0, 64, 128, 160);

	loop {
		MBoss_at(1);
		MBoss_at2(-1);
		wait(180);
		moveRand(60, LINEAR, 1);
		wait(60);

		MBoss_at(-1);
		MBoss_at2(1);
		wait(240);
	}

	loop wait(1000);
}


async MBoss_at(float dir) {
	bmSet(0, AIM_ST_R, B16B, RED, 2, 1, 0, 0, 3, 1);
	bmDist(0, 32);
	bmCloud(0, CLOUD_LONG);
	bmStep(0, TRANS_WAIT, 60, 1, STEP_ANGREL, 0, 0);
	bmAccel(0, TRANS_WAIT, 60, 2.0 / 60, NEGF);

	float r2 = rad(-90);
	loop (24) {
		bmAngle(0, r2, 0);

		const int n = 5;
		float r = 0;
		loop(n) {
			bmStepSet(0, 1, TRANS_WAIT, 60, 1, STEP_ANGREL, r, 0);
			bmShoot(0);
			r += TAU / n;
		}
		wait(3);
		r2 += TAU / 24 * dir;
	}
}

async MBoss_at2(float dir) {
	bmSet(1, AIM_ST_R, B16B, ORANGE, 2, 1, 0, 0, 3, 1);
	bmDist(1, 96);
	bmCloud(1, CLOUD_LONG);
	bmStep(1, TRANS_WAIT, 60, 1, STEP_ANGREL, 0, 0);
	bmAccel(1, TRANS_WAIT, 120, 2.5 / 120, NEGF);

	float r2 = rad(-90);
	loop(24) {
		bmAngle(1, r2, 0);

		const int n = 5;
		float r = 0;
		loop(n) {
			bmStepSet(1, 1, TRANS_WAIT, 60, 1, STEP_ANGPL, r, 0);
			bmShoot(1);
			r += TAU / n;
		}
		wait(3);
		r2 += TAU / 24 * dir;
	}
}

void MBossCard1() {
	dvmEvent(0, 9);
	setInterrupt(0, 0, 30 * 60, "MBossEscape");
	spellEx(SPELL_BHA_1E, 30 * 60, 500000, "st2CardA");
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
	movePosTime(60, SMOOTHSTEP, 0, 160);
	wait(60);

	effChargeRed(0, 0);
	sfx(SE_CHARGE);
	wait(60);
	MBossCard1_at();

	loop wait(1000);
}

void MBossCard1_at() {
	bmSet(0, AIM_ST_R, B64A, RED64, 2, 1, rad(-20), 0, 3, 1);
	bmSound(0, SE_SHOT2, -1);
	bmCloud(0, CLOUD_MEDIUM);
	bmInvuln(0, TRANS_ASYNC, 999999);
	bmMark(0, TRANS_WAIT, 1);
	//bmAccel(0, TRANS_WAIT, 60, -1.0 / 60, NEGF);
	bmBounce(0, TRANS_ASYNC, 1000, BOUNCE_UDLR, NEGF);

	bmSet(1, AIM_ST_R, B16G, PINK, 3, 2, 0, 0, 1, 0);
	bmCloud(1, CLOUD_LONG);
	bmAccel(1, TRANS_WAIT, 180, 1.0 / 180, NEGF);

	bmShoot(0);
	wait(10);
	F0 = 0;
	loop (200) {
		funcCall("MBossCard1_func", 1, FUNC_COND_ALL);
		wait(12);
		F0 += rad(10);
	}
}

void MBossCard1_func(entity en, float x, float y) {
	bmAngle(1, F0, 0);
	bmOffsetAbs(1, x, y);
	bmShoot(1);
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
		dropMain(IT_LIFE);
		dropExtra(IT_STAR, 4);
	}
	dropExtra(IT_POWER_BIG, 2);
	dropExtra(IT_POWER, 5);
	dropExtra(IT_POINT, 10);
	dropArea(96, 96);
	dropItems();

	dvmEvent(0, 9);
	lifeSet(100000);
	setBoss(-1, "", "");
	wait(90);

	movePosTime(60, EASEOUT1, -32, -32);
	wait(60);
	die();
}