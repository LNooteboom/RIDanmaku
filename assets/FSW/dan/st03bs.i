import "danmaku";

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
		int cnt = 8;
		float rd = 2 * PI / cnt;
		F0 = AIM;
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

	bmSet(0, AIM_ST_R, B32B, GREEN32, 7, 1, 0, 0, 0, 0);
	bmDist(0, 20);
	bmCloud(0, CLOUD_MEDIUM);
	bmWait(0, 120);
	bmDelete(0, 1);

	bmSet(1, AIM_ST_R, B32A, GREEN32, 1, 1, 0, 0, 0, 0);
	bmCloud(1, CLOUD_MEDIUM);
	bmWait(1, 120);
	bmAccel(1, TRANS_WAIT, 60, 1.5 / 60, NEGF);

	loop(20) {
		float r = RRAD, d = 32 * RF;
		bmOffsetRad(0, r, d);
		bmAngle(0, RRAD, 0);
		bmShoot(0);

		bmOffsetRad(1, r, d);
		bmAngle(1, RRAD, 0);
		bmShoot(1);

		wait(6);
	}
}