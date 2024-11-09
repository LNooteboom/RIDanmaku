import "danmaku";

// SUB 00

async MainSub00() {
	enmNewA("Girl00", 160 + RF2 * 32, -32, 100, GIRL_R);
	wait(20);
	enmNewA("Girl00", -160 + RF2 * 32, -32, 100, GIRL_R);
	wait(20);
	enmNewA("Girl00", 128 + RF2 * 32, -32, 100, GIRL_B);
	wait(20);
	enmNewA("Girl00", -128 + RF2 * 32, -32, 100, GIRL_B);
	wait(20);

	enmNewA("Girl00", 160 + RF2 * 32, -32, 100, GIRL_R);
	wait(20);
	enmNewA("Girl00", -160 + RF2 * 32, -32, 100, GIRL_R);
	wait(20);
	enmNewA("Girl00", 96 + RF2 * 32, -32, 100, GIRL_B);
	wait(20);
	enmNewA("Girl00", -96 + RF2 * 32, -32, 100, GIRL_B);
	wait(20);

	enmNewA("Girl00", 128 + RF2 * 32, -32, 100, GIRL_R);
	wait(20);
	enmNewA("Girl00", -128 + RF2 * 32, -32, 100, GIRL_R);
	wait(20);
}

void Girl00() {
	setGirl();
	float x, y;
	circlePos(x, y, RRAD, RF * 32);
	x += PLAYER_X;
	y += PLAYER_Y - 128;
	if (y < 128)
		y = 128;
	moveBezier(90, 0, 512, x, y, 0, 0);
	wait(90);
	Girl00_at();
	wait(90);
	moveVel(AIM, 0);
	moveVelTime(60, LINEAR, NEGF, 4);

	wait(6000);
	die();
}
async Girl00_at() {
	bmSet(0, AIM_AT_R, B16C, BLUE, diffI(1, 1, 6, 12), 1, 0, rad(2), diffF(1.5, 2.5, 3.5, 4.5), 1);
	bmCloud(0, CLOUD_MEDIUM);
	bmFast(0, TRANS_WAIT);
	bmShoot(0);
	loop(diffI(1, 10, 10, 20)) {
		wait(1);
		bmShoot(0);
	}
}

// SUB 01

async MainSub01() {
	wait(260);
	enmNewA("Girl02", -224, 192, 100, GIRL_R);
	wait(20);
	enmNewA("Girl02", -240, 192, 100, GIRL_R);
	wait(20);
	enmNewA("Girl02", -256, 192, 100, GIRL_R);
	wait(20);
	enmNewA("Girl02", -272, 192, 100, GIRL_R);
	enmNewAM("Girl01", 224, 192, 100, GIRL_B);
	wait(20);
	enmNewAM("Girl01", 240, 192, 100, GIRL_B);
	wait(20);
	enmNewAM("Girl01", 256, 192, 100, GIRL_B);
	wait(20);
	enmNewAM("Girl01", 272, 192, 100, GIRL_B);

	enmNewA("Girl01", -224, 192, 100, GIRL_R);
	wait(20);
	enmNewA("Girl01", -240, 192, 100, GIRL_R);
	wait(20);
	enmNewA("Girl01", -256, 192, 100, GIRL_R);
	wait(20);
	enmNewA("Girl01", -272, 192, 100, GIRL_R);
	enmNewAM("Girl02", 224, 192, 100, GIRL_B);
	wait(20);
	enmNewAM("Girl02", 240, 192, 100, GIRL_B);
	wait(20);
	enmNewAM("Girl02", 256, 192, 100, GIRL_B);
	wait(20);
	enmNewAM("Girl02", 272, 192, 100, GIRL_B);

	enmNewA("Girl01", -224, 192, 100, GIRL_R);
	wait(20);
	enmNewA("Girl02", -240, 192, 100, GIRL_R);
	wait(20);
	enmNewA("Girl01", -256, 192, 100, GIRL_R);
	wait(20);
	enmNewA("Girl02", -272, 192, 100, GIRL_R);
	enmNewAM("Girl02", 224, 192, 100, GIRL_R);
	wait(20);
	enmNewAM("Girl01", 240, 192, 100, GIRL_R);
	wait(20);
	enmNewAM("Girl02", 256, 192, 100, GIRL_R);
	wait(20);
	enmNewAM("Girl01", 272, 192, 100, GIRL_R);
}

void Girl01() {
	setGirl();
	flagSet(FLAG_OFFSCR_LR);
	moveVel(rad(-22.5), 6);
	moveVelTime(60, LINEAR, 0, 0);
	wait(60);
	Girl01_at();
	moveVelTime(60, LINEAR, rad(22.5), 6);
	wait(6000);
	die();
}
void Girl01_at() {
	bmSet(0, AIM_AT_R, B32A, PINK32, diffI(6, 12, 20, 20), diffI(1, 1, 1, 2), 0, rad(2), diffF(1.3, 1.5, 2.5, 3.5), 1);
	bmCloud(0, CLOUD_MEDIUM);
	bmFast(0, TRANS_WAIT);
	bmShoot(0);
}

void Girl02() {
	setGirl();
	flagSet(FLAG_OFFSCR_LR);
	moveVel(rad(-22.5), 6);
	moveVelTime(60, LINEAR, 0, 0);
	wait(60);
	Girl02_at();
	moveVelTime(60, LINEAR, rad(22.5), 6);
	wait(6000);
	die();
}
void Girl02_at() {
	bmSet(0, AIM_AT, B32B, BLUE32, diffI(1, 5, 5, 5), diffI(1, 1, 2, 3), 0, rad(2), diffF(2.5, 3.0, 3.0, 5.0), 1);
	bmCloud(0, CLOUD_MEDIUM);
	bmFast(0, TRANS_WAIT);
	bmShoot(0);
}

// SUB 03

async MainSub03() {
	wait(220);
	enmNewA("Cir00", 0, -32, 400, CIR_B);
	wait(60);
	enmNewA("Cir00", 128, -32, 400, CIR_B);
	wait(60);
	enmNewA("Cir00", -128, -32, 400, CIR_B);
	wait(60);
	enmNewA("Cir00", 64, -32, 400, CIR_B);
	wait(60);
	enmNewA("Cir00", -128, -32, 400, CIR_B);
	wait(60);
	enmNewA("Cir00", -64, -32, 400, CIR_B);
}

void Cir00() {
	setCir();
	moveVel(rad(90) + RF2 * rad(11.25), 12);
	moveVelTime(30, LINEAR, NEGF, 0);
	wait(30);
	Cir00_at();
	wait(30);
	loop (diffI(1, 1, 1, 2)) {
		moveVel(rad(-90) + RF2 * rad(11.25), 2);
		moveVelTime(60, LINEAR, NEGF, 0);
		wait(60);
		Cir00_at();
		diffWait(30, 30, 15, 10);

		moveVel(rad(90) + RF2 * rad(11.25), 2);
		moveVelTime(60, LINEAR, NEGF, 0);
		wait(60);
		Cir00_at();
		diffWait(30, 30, 15, 10);
	}
	moveVel(rad(-90), 6);
	wait(6000);
	die();
}
async Cir00_at() {
	bmSet(0, AIM_AT, B32B, BLUE32, 1, 1, rad(90), 0, 1.5, 1.0);
	bmCloud(0, CLOUD_MEDIUM);
	bmFast(0, TRANS_WAIT);
	float r = 0, s = 1.0;
	loop (diffI(4, 10, 12, 16)) {
		bmStepSet(0, 2, TRANS_WAIT, 45, 1, STEP_ANGPL, r, s);
		bmShoot(0);
		r += diffF(rad(18), rad(11.25), rad(8.6), rad(7.5));
		s += diffF(0.8, 0.3, 0.35, 0.3);
	}

	r = 0;
	s = 1.0;
	bmAngle(0, rad(-90), 0);
	loop(diffI(4, 10, 12, 16)) {
		bmStepSet(0, 2, TRANS_WAIT, 45, 1, STEP_ANGPL, r, s);
		bmShoot(0);
		r -= diffF(rad(18), rad(11.25), rad(8.6), rad(7.5));
		s += diffF(0.8, 0.3, 0.35, 0.3);
	}
}

// SUB 04

async MainSub04() {
	enmNewA("Cir01", 0, 160, 1800, CIR_P);
	wait(220);
	enmNewA("Cir01B", -32, 176, 1200, CIR_P);
	wait(220);
	enmNewA("Cir01", 32, 192, 1200, CIR_P);
}

void Cir01() {
	setCir();
	dropClear();
	dropArea(64, 64);
	dropExtra(IT_POINT, 6);
	dropExtra(IT_POWER, 2);
	moveEllipse(rad(180), rad(3), 224.0, -224.0 / 180, rad(90), 0.3);
	wait(180);

	moveVel(0, 0);
	Cir01_at(0, BLUE, 2.2, PI / 96);
	Cir01_at(1, CYAN, 1.3, PI / -64);
	wait(60);
	Cir01_at(0, BLUE, 2.2, PI / -96);
	Cir01_at(1, CYAN, 1.3, PI / 64);
	wait(120);

	moveEllipse(rad(180), rad(3), 0, 224.0 / 180, rad(90), 0.3);
	wait(180);
	die();
}
void Cir01B() {
	setCir();
	dropClear();
	dropArea(64, 64);
	dropExtra(IT_POINT, 6);
	dropExtra(IT_POWER, 2);
	moveEllipse(0, rad(-3), 224.0, -224.0 / 180, rad(90), 0.3);
	wait(180);

	moveVel(0, 0);
	Cir01_at(0, GREEN, 2.2, PI / 96);
	Cir01_at(1, LIME, 1.3, PI / -64);
	wait(60);
	Cir01_at(0, GREEN, 2.2, PI / -96);
	Cir01_at(1, LIME, 1.3, PI / 64);
	wait(120);

	moveEllipse(0, rad(-3), 0, 224.0 / 180, rad(90), 0.3);
	wait(180);
	die();
}
async Cir01_at(int bm, int col, float spd, float rd) {
	bmSet(bm, AIM_ST_R, B16A, col, diffI(16, 16, 22, 24), 1, rad(90), 0, spd, 1);
	bmCloud(bm, CLOUD_MEDIUM);
	bmFast(bm, TRANS_WAIT);
	float r = AIM;
	loop (6) {
		bmAngle(bm, r, 0);
		bmShoot(bm);
		r += rd;
		wait(10);
	}
}

// MBOSS

void MBoss() {
	//setBoss(0, "");
	
	dvmSFGirl(0);
	hurtbox(24, 24);
	hitbox(20, 20);
	lifeSet(7600);
	setInterrupt(0, 1000, 1800, "MBossCard1");
	setTimeout(0, "MBossEscape");
	lifeMarker(0, 1000, 0xFFFFA080);
	
	enmKillAll();
	bmClear(640, 0);
	CAPTURE = 1;
	distort(160, 0xFF80FF);

	loop {
		movePosTime(60, EASEOUT1, 0, 192);
		wait(70);
		MBoss_at1();
		wait(60);
	}
}
void MBoss_at1() {
	bmSet(0, AIM_AT_R, B16G, BLUE, diffI(1, 1, 1, 2), diffI(1, 1, 2, 1), 0, PI / 96, diffF(2.0, 2.0, 3.0, 3.0), 1.0);
	bmDist(0, 64);
	bmSound(0, SE_SHOT1, -1);
	bmCloud(0, CLOUD_LONG);
	bmStep(0, TRANS_WAIT, 60, 1, STEP_ANGPL, 0, 0);
	float ang1 = rad(-90);
	loop {
		float ang2 = -PI;
		loop(48) {
			bmStepSet(0, 2, TRANS_WAIT, 1, 1, STEP_ANGREL, ang1, 1.6);
			bmStepSet(0, 3, TRANS_WAIT, 60, 1, STEP_ANGREL, ang2, 0.8);
			bmAngle(0, ang2, 0);
			bmShoot(0);
			if (!DIFF) {
				ang2 += PI / 12;
				ang1 += PI / 24;
			} else {
				ang2 += PI / 24;
				ang1 += PI / 48;
			}
			diffWait(2, 1, 1, 1);
		}
	}
}

void MBossCard1() {
	setInterrupt(0, 0, 2040, "MBossEscape");
	//spell(0, 2040, "spellBgTest", "", "妬符「グリーンアイドモンスター」");
	if (CAPTURE) {
		dropMain(IT_LIFEPIECE);
		dropItems();
	}
	killAll();
	movePosTime(60, EASEOUT1, 128, 224);
	invuln(60);
	breakWave();
	moveLimit(0, 128, 280, 256);
	wait(60);
	flagSet(FLAG_NO_HIT);

	enmNew("MBossCard1At", 0, 0, 100, -1);
	wait(440);

	wait(25);
	movePos(-128, 224, 0);
	wait(25);
	enmNew("MBossCard1At2", 0, 0, 100, -1);
	wait(440);

	wait(25);
	movePos(128, 224, 0);
	wait(25);
	enmNew("MBossCard1At3", 0, 0, 100, -1);
	wait(440);

	loop wait(1000);
}
void MBossCard1At() {
	flagSet(FLAG_NO_HIT | FLAG_NO_HURT | FLAG_OFFSCREEN);
	movePos(-224, 32, 0);
	MBossCard1At_at();

	moveBezier(20, 0.0f, 0.0f, -64.0f, 100.0f, 256.0f, 0.0f);
	wait(20);
	moveBezier(20, 256.0f, 0.0f, -160.0f, 200.0f, -256.0f, 0.0f);
	wait(20);
	moveBezier(20, -256.0f, 0.0f, -64.0f, 300.0f, 256.0f, 0.0f);
	wait(20);
	moveBezier(20, 256.0f, 0.0f, -128.0f, 400.0f, 0.0f, 0.0f);
	wait(20);
	moveLimit(0.0f, 224.0f, 384.0f, 448.0f);

	float spd = diffF(2.3, 2.6, 2.6, 3.6);
	loop (9) {
		moveVel(AIM, spd);
		wait(40);
	}
	die();
}
void MBossCard1At2() {
	flagSet(FLAG_NO_HIT | FLAG_NO_HURT | FLAG_OFFSCREEN);
	movePos(160.0f, 32.0f, 0);
	MBossCard1At_at();

	moveBezier(20, 0.0f, 0.0f, 160.0f, 100.0f, 0.0f, 0.0f);
	wait(20);
	moveBezier(20, 0.0f, 0.0f, 140.0f, 160.0f, 0.0f, 0.0f);
	wait(20);
	moveBezier(20, 0.0f, 0.0f, 160.0f, 224.0f, 0.0f, 0.0f);
	wait(20);
	moveLimit(0.0f, 224.0f, 384.0f, 448.0f);

	float spd = diffF(2.4, 3.3, 3.6, 4.3);
	loop(12) {
		moveVel(AIM, spd);
		wait(30);
	}
	die();
}
void MBossCard1At3() {
	flagSet(FLAG_NO_HIT | FLAG_NO_HURT | FLAG_OFFSCREEN);
	MBossCard1At_at();

	movePos(-224.0f, 32.0f, 0);
	moveBezier(20, 0.0f, 0.0f, -160.0f, 64.0f, 256.0f, 0.0f);
	wait(20);
	moveBezier(20, 0.0f, 0.0f, -160.0f, 440.0f, 256.0f, 0.0f);
	wait(20);
	moveLimit(0.0f, 224.0f, 384.0f, 448.0f);

	float spd = diffF(3.0, 4.5, 4.5, 5.0);
	loop(20) {
		moveVel(AIM, spd);
		wait(20);
	}
	die();
}
async MBossCard1At_at() {
	wait(1);
	bmSet(0, AIM_AT_R, B16B, GREEN, 1, 1, 0, PI / 96, 0, 1);
	bmCloud(0, CLOUD_LONG);
	bmDeleteSet(0, 2, 1);
	int dly = 440;
	loop (60) {
		bmWaitSet(0, 1, dly);
		bmSprite(0, B16B, GREEN);
		bmOffsetRad(0, RRAD, RF * 32);
		bmShoot(0);
		dly -= 1;
		wait(1);

		bmWaitSet(0, 1, dly);
		bmSprite(0, B32A, GREEN32);
		bmOffsetRad(0, RRAD, RF * 32);
		bmShoot(0);
		dly -= 1;
		wait(1);
	}
	loop(60) {
		bmWaitSet(0, 1, dly);
		bmSprite(0, B16B, GREEN);
		bmOffsetRad(0, RRAD, RF * 32);
		bmShoot(0);
		dly -= 2;
		wait(2);

		bmWaitSet(0, 1, dly);
		bmSprite(0, B32A, GREEN32);
		bmOffsetRad(0, RRAD, RF * 32);
		bmShoot(0);
		dly -= 2;
		wait(2);
	}
}

void MBossEscape() {
	setInterrupt(0, -1, 0, "");
	setInterrupt(1, -1, 0, "");
	spellEnd();
	moveLimitReset();
	distort(0, 0xFF80FF);
	enmKillAll();
	wait(1);
	if (TIMEOUT) {
		bmClear(640, 0);
	} else {
		bmClear(640, 1);
		dropClear();
		if (CAPTURE)
			dropMain(IT_LIFEPIECE);
		dropExtra(IT_STAR, 4);
	}
	dropExtra(IT_POWER, 10);
	dropExtra(IT_POINT, 10);
	dropArea(48, 48);
	dropItems();

	lifeSet(100000);
	//setBoss(-1, "");
	wait(90);

	movePosTime(60, EASEOUT1, -32, -32);
	wait(60);
	die();
}

// SUB 05

void MainSub05() {
	loop (10) {
		enmNewAF("Girl03", -224, 96, 100, GIRL_G);
		wait(20);
		enmNewAF("Girl03", -240, 96, 100, GIRL_G);
		wait(20);
		enmNewAF("Girl03", -256, 96, 100, GIRL_G);
		wait(20);
		enmNewAF("Girl03", -272, 96, 100, GIRL_G);
		enmNewAMF("Girl03", 224, 96, 100, GIRL_B);
		wait(20);
		enmNewAMF("Girl03", 240, 96, 100, GIRL_B);
		wait(20);
		enmNewAMF("Girl03", 256, 96, 100, GIRL_B);
		wait(20);
		enmNewAMF("Girl03", 272, 96, 100, GIRL_B);
	}
}
void Girl03() {
	setGirl();
	flagSet(FLAG_OFFSCREEN);
	moveVel(PI / -8, 6.0);
	moveVelTime(60, LINEAR, 0, 0);
	wait(60);
	Girl03_at();
	flagClear(FLAG_OFFSCREEN);
	moveVelTime(60, LINEAR, PI / 8, 6.0);
	wait(6000);
}
void Girl03_at() {
	bmSet(0, AIM_AT_R, B32A, PINK32, diffI(6, 12, 14, 16), 1, 0, PI / 96, diffF(2.0, 3.5, 4.5, 5.5), 1);
	bmCloud(0, CLOUD_MEDIUM);
	bmFast(0, TRANS_WAIT);
	bmShoot(0);
}

// SUB 06

void MainSub06() {
	enmNewAF("Cir02", 0, 128, 100, CIR_B);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_R);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_B);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_R);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_B);
	wait(120);

	enmNewAF("Cir02B", 0, 128, 100, CIR_B);
	wait(20);
	enmNewAF("Cir02B", RF2 * 32, 128 + RF2 * 32, 100, CIR_R);
	wait(20);
	enmNewAF("Cir02B", RF2 * 32, 128 + RF2 * 32, 100, CIR_B);
	wait(20);
	enmNewAF("Cir02B", RF2 * 32, 128 + RF2 * 32, 100, CIR_R);
	wait(20);
	enmNewAF("Cir02B", RF2 * 32, 128 + RF2 * 32, 100, CIR_B);
	wait(120);

	enmNewAF("Cir02", 0, 128, 100, CIR_B);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_R);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_B);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_R);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_B);
	wait(20);

	enmNewAF("Cir02B", 0, 128, 100, CIR_B);
	wait(20);
	enmNewAF("Cir02B", RF2 * 32, 128 + RF2 * 32, 100, CIR_R);
	wait(20);
	enmNewAF("Cir02B", RF2 * 32, 128 + RF2 * 32, 100, CIR_B);
	wait(20);
	enmNewAF("Cir02B", RF2 * 32, 128 + RF2 * 32, 100, CIR_R);
	wait(20);
	enmNewAF("Cir02B", RF2 * 32, 128 + RF2 * 32, 100, CIR_B);
	wait(20);

	enmNewAF("Cir02", 0, 128, 100, CIR_B);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_R);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_B);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_R);
	wait(20);
	enmNewAF("Cir02", RF2 * 32, 128 + RF2 * 32, 100, CIR_B);
	wait(20);
}
void Cir02() {
	setCir();
	flagSet(FLAG_OFFSCREEN);
	Cir02_at();
	moveEllipse(PI, rad(6), 224, -224.0 / 90, rad(90), 0.3);
	wait(90);
	moveEllipse(PI, rad(6), 0, 224.0 / 90, rad(90), 0.3);
	wait(90);
	die();
}
async Cir02_at() {
	bmSet(0, AIM_AT_R, B16A, YELLOW, diffI(1, 1, 1, 4), diffI(2, 2, 3, 4), 0, 0, diffF(4, 4, 5, 6), 0.8);
	bmCloud(0, CLOUD_LONG);
	bmSet(1, AIM_AT_R, B16A, DARKGREEN, 1, 2, 0, 0, 2.0, 0.8);
	bmCloud(1, CLOUD_LONG);
	wait(20);
	loop (60) {
		bmShoot(0);
		wait(10);
		bmAngle(1, RRAD / 16, 0);
		bmShoot(1);
		diffWait(60, 30, 20, 4);
	}
}
void Cir02B() {
	setCir();
	flagSet(FLAG_OFFSCREEN);
	Cir02_at();
	moveEllipse(PI, rad(-6), 224, -224.0 / 90, rad(90), 0.3);
	wait(90);
	moveEllipse(PI, rad(-6), 0, 224.0 / 90, rad(90), 0.3);
	wait(90);
	die();
}

// main

void main() {
	dvmFile("dan_enemy");
	bgFile("sdm");
	bgCall("sdm1");

	wait(160);
	MainSub00();
	MainSub01();
	wait(800);
	MainSub03();
	wait(760);
	MainSub04();
	wait(800);
	enmNewA("MBoss", 128, -32, 40, IT_NONE);
	wait(800);
	MainSub05();
	bossWait();
	wait(100);
	MainSub06();

	loop wait(1000);
}