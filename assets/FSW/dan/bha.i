import "danmaku";
import "bha_mbs";
import "bha_bs";

async MainSub00() {
	string fn = "Girl00";

	enmNewA(fn, -128, -32, 40, GIRL_R); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_B); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_R); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_B); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_R); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_B); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_R); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_B); wait(15 + 120);

	enmNewAM(fn, 128, -32, 40, GIRL_R); wait(15);
	enmNewAM(fn, 128, -32, 40, GIRL_B); wait(15);
	enmNewAM(fn, 128, -32, 40, GIRL_R); wait(15);
	enmNewAM(fn, 128, -32, 40, GIRL_B); wait(15);
	enmNewAM(fn, 128, -32, 40, GIRL_R); wait(15);
	enmNewAM(fn, 128, -32, 40, GIRL_B); wait(15);
	enmNewAM(fn, 128, -32, 40, GIRL_R); wait(15);
	enmNewAM(fn, 128, -32, 40, GIRL_B); wait(15 + 120);

	enmNewA(fn, -128, -32, 40, GIRL_R); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_B); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_R); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_B); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_R); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_B); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_R); wait(15);
	enmNewA(fn, -128, -32, 40, GIRL_B); wait(15 + 120);
}

void Girl00() {
	setGirl();
	moveVel(rad(90), 3);
	Girl00_at();
	wait(20);
	moveVelTime(90, SMOOTHSTEP, rad(20), NEGF);

	loop wait(1000);
}

async Girl00_at() {
	bmSet(0, AIM_AT, B8A, RED, 3, 1, 0, PI / 64, 4, 2);
	bmCloud(0, CLOUD_MEDIUM);
	bmFast(0, TRANS_WAIT);
	wait(30 + RI % 30);

	loop(4) {
		bmShoot(0);
		wait(20);
	}
}

async MainSub01() {
	wait(60);
	enmNewA("Girl01", 64, -32, 600, 0); wait(240);
	enmNewAM("Girl01", -64, -32, 600, 0); wait(240);
	enmNewA("Girl01", 64, -32, 600, 0); wait(240);
}

void Girl01() {
	dvmSFGirl(0);
	hitboxSFGirl();
	dropMain(IT_STAR);
	dropExtra(IT_POWER, 4);
	dropExtra(IT_POINT, 3);
	moveVel(rad(90), 5);
	moveVelTime(60, LINEAR, NEGF, 0);
	wait(60);

	Girl01_at();

	wait(200);
	moveVel(rad(-90), 0);
	moveVelTime(60, LINEAR, NEGF, 3);

	loop wait(1000);
}

async Girl01_at() {
	bmSet(0, AIM_ST_R, B16B, ORANGE, 5, 1, 0, 0, 2, 1);
	bmSound(0, SE_SHOT2, -1);
	bmCloud(0, CLOUD_MEDIUM);
	bmFast(0, TRANS_WAIT);

	float r = 0;
	float rd = FLAGS & FLAG_MIRROR ? rad(-13) : rad(13);
	loop(12) {
		bmAngle(0, r, 0);

		float s = 0;
		loop(4) {
			bmAccelSet(0, 2, TRANS_WAIT, 60, s, NEGF);
			bmShoot(0);
			s += 1.0 / 60;
		}

		r += rd;
		wait(10);
	}
}

async MainSub02() {
	string fn = "Spir00";
	loop(6) {
		F0 = 1;
		enmNewA(fn, RF2 * 128, 96 + RF * 64, 200, GIRL_B);
		wait(30);

		F0 = -1;
		enmNewA(fn, RF2 * 128, 96 + RF * 64, 200, GIRL_B);
		wait(30);
	}
}

void Spir00() {
	setSpir();
	onDeath("Spir00_dead");
	dropExtra(IT_POWER, 2);
	dropExtra(IT_POINT, 2);

	wait(30);
	Spir00_at();
	wait(120);
	Spir00_dead();
	die();

	loop wait(1000);
}

async Spir00_at() {
	// bmSet(0, AIM_AT, B16H, BLUE, 5, 1, 0, 0, 2, 1);
	// bmCloud(0, CLOUD_MEDIUM);

	// float r1 = 0;
	// float r2 = rad(60);
	// float s = 2;
	// loop(20) {
	// 	bmAngle(0, r1, r2);
	// 	bmSpeed(0, s, 1);
	// 	bmShoot(0);
	// 	wait(3);
	// 	s += 0.2;
	// 	r2 -= rad(52) / 20;
	// }

	bmSet(0, AIM_ST_R, B16H, BLUE, 7, 1, 0, rad(20), 2, 1);
	bmCloud(0, CLOUD_MEDIUM);
	bmWait(0, 20);

	float r = 0;
	loop (5) {
		bmAngle(0, r, rad(20));
		float s = 0;
		loop (5) {
			bmAccelSet(0, 2, TRANS_WAIT, 60, s / 60, NEGF);
			bmShoot(0);
			s += 0.2;
		}
		wait(12);
		r += rad(10) * F0;
	}
}

void Spir00_dead() {
	bmSet(0, AIM_ST_R, B8A, CYAN, 12, 2, RRAD, 0, 2.5, 1);
	bmCloud(0, CLOUD_SHORT);
	bmShoot(0);
}

async MainSub03() {
	int hp = 40, hp2 = 720;
	string fn = "Girl03";
	enmNewA("Girl02", 0, -32, hp2, 0); wait(120);

	enmNewA(fn, -128, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, -96, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, -64, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, -32, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, 0, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, 32, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, 64, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, 96, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, 128, -32, hp, GIRL_B); wait(10);
	enmNewA("Girl02", 48, -32, hp2, 0); wait(60);

	enmNewAM(fn, 128, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, 96, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, 64, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, 32, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, 0, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, -32, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, -64, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, -96, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, -128, -32, hp, GIRL_B); wait(10);
	enmNewA("Girl02", -48, -32, hp2, 0); wait(60);

	enmNewA(fn, -128, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, -96, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, -64, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, -32, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, 0, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, 32, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, 64, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, 96, -32, hp, GIRL_B); wait(10);
	enmNewA(fn, 128, -32, hp, GIRL_B); wait(10);
	enmNewA("Girl02", 48, -32, hp2, 0); wait(60);

	enmNewAM(fn, 128, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, 96, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, 64, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, 32, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, 0, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, -32, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, -64, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, -96, -32, hp, GIRL_B); wait(10);
	enmNewAM(fn, -128, -32, hp, GIRL_B); wait(10);
	enmNewA("Girl02", -48, -32, hp2, 0); //wait(60);

	//enmNewA("Girl02", 48, -32, hp2, 0); wait(360);
	enmNewA("Girl02", 48, -32, hp2, 0);
}

void Girl02() {
	dvmSFGirl(0);
	hitboxSFGirl();
	dropMain(IT_STAR);
	dropExtra(IT_POWER, 5);
	dropExtra(IT_POINT, 4);
	moveVel(rad(90), 2);
	moveVelTime(120, EASEOUT1, NEGF, 0.5);
	wait(120);

	Girl02_at();

	loop wait(1000);
}

async Girl02_at() {
	float rd = rad(60);
	int n = 16;
	bmSet(0, AIM_AT_R, B16B, ORANGE, n, 1, 0, 0, 3, 1);
	bmCloud(0, CLOUD_MEDIUM);
	bmAccelAngv(0, TRANS_WAIT, 60, -3.0 / 60, rad(180) / 60);
	bmStep(0, TRANS_WAIT, 0, 1, STEP_ANGREL, -rd, 2.0);

	bmSet(1, AIM_AWAY_R, B16B, RED, n, 1, 0, 0, 3, 1);
	bmCloud(1, CLOUD_MEDIUM);
	bmAccelAngv(1, TRANS_WAIT, 60, -3.0 / 60, rad(-180) / 60);
	bmStep(1, TRANS_WAIT, 0, 1, STEP_ANGREL, rd, 2.0);

	loop(3) {
		float s = 2.0;
		loop(5) {
			bmStepSet(0, 2, TRANS_WAIT, 0, 1, STEP_ANGREL, -rd, s);
			bmShoot(0);
			bmStepSet(1, 2, TRANS_WAIT, 0, 1, STEP_ANGREL, rd, s);
			bmShoot(1);

			s += 0.3;
		}

		wait(90);
	}
}

void Girl03() {
	setGirl();
	moveVel(rad(90), 6);
	moveVelTime(60, EASEOUT1, NEGF, 0);

	wait(60);

	Girl03_at();

	wait(60);

	moveVel(rad(60), 0);
	moveVelTime(60, EASEIN1, NEGF, 1.5);

	loop wait(1000);
}

async Girl03_at() {
	bmSet(0, AIM_AT, B16C, BLUE, 3, 2, 0, rad(50), 5, 1);
	bmCloud(0, CLOUD_MEDIUM);

	loop(4) {
		bmShoot(0);
		wait(10);
	}
}

async MainSub04() {
	string fn = "Cir00";
	int hp = 120;
	I1 = 0; enmNewAF(fn, -224, 32, hp, CIR_G); wait(5);
	I1 = 32; enmNewAF(fn, -224, 32, hp, CIR_B); wait(5);
	I1 = 64; enmNewAF(fn, -224, 32, hp, CIR_G); wait(5);
	I1 = 96; enmNewAF(fn, -224, 32, hp, CIR_B); wait(5);
	I1 = 128; enmNewAF(fn, -224, 32, hp, CIR_G); wait(150);

	I1 = 0; enmNewAMF(fn, 224, 32, hp, CIR_B); wait(5);
	I1 = 32; enmNewAMF(fn, 224, 32, hp, CIR_G); wait(5);
	I1 = 64; enmNewAMF(fn, 224, 32, hp, CIR_B); wait(5);
	I1 = 96; enmNewAMF(fn, 224, 32, hp, CIR_G); wait(5);
	I1 = 128; enmNewAMF(fn, 224, 32, hp, CIR_B); wait(150);

	I1 = 0; enmNewAF(fn, -224, 32, hp, CIR_G); wait(5);
	I1 = 32; enmNewAF(fn, -224, 32, hp, CIR_B); wait(5);
	I1 = 64; enmNewAF(fn, -224, 32, hp, CIR_G); wait(5);
	I1 = 96; enmNewAF(fn, -224, 32, hp, CIR_B); wait(5);
	I1 = 128; enmNewAF(fn, -224, 32, hp, CIR_G); wait(150);

	I1 = 0; enmNewAMF(fn, 224, 32, hp, CIR_B); wait(5);
	I1 = 32; enmNewAMF(fn, 224, 32, hp, CIR_G); wait(5);
	I1 = 64; enmNewAMF(fn, 224, 32, hp, CIR_B); wait(5);
	I1 = 96; enmNewAMF(fn, 224, 32, hp, CIR_G); wait(5);
	I1 = 128; enmNewAMF(fn, 224, 32, hp, CIR_B); wait(150);
}

void Cir00() {
	setCir();
	movePosTime(60, EASEOUT1, -160 + I1 * 2, 224 - I1);
	wait(60);

	Cir00_at();
	Cir00_at2();
	wait(60);
	Cir00_at();
	Cir00_at2();
	moveVel(rad(-60), 0);
	moveVelTime(60, EASEIN1, NEGF, 4);

	loop wait(1000);
}

void Cir00_at() {
	const int bm = 0;
	const int t = 16;
	const float rd = rad(60);

	bmSet(bm, AIM_AT, B32B, GREEN32, 1, 1, 0, 0, 2, 1);
	bmSound(bm, SE_SHOT1, -1);
	bmCloud(bm, CLOUD_MEDIUM);
	bmStep(bm, TRANS_WAIT, 40, 1, STEP_ANGREL, 0, 0);
	bmPlaySound(bm, TRANS_WAIT, SE_KIRA1);
	bmStep(bm, TRANS_WAIT, 10, 1, STEP_ANGPL, 0, 5); // dummy
	bmAccelAngv(bm, TRANS_WAIT, t, 0, rd * 2 / t);
	bmAccelAngv(bm, TRANS_WAIT, t, 0, rd * -2 / t);
	bmJump(bm, 3, 6);
	
	int w = 1;
	loop (6) {
		bmStepSet(bm, 3, TRANS_WAIT, w, 1, STEP_ANGREL, -rd, 4);
		bmShoot(bm);
		w += 5;
	}
}
void Cir00_at2() {
	const int bm = 1;
	const int t = 16;
	const float rd = rad(-60);

	bmSet(bm, AIM_AT, B32B, GREEN32, 1, 1, 0, 0, 2, 1);
	bmSound(bm, SE_SHOT1, -1);
	bmCloud(bm, CLOUD_MEDIUM);
	bmStep(bm, TRANS_WAIT, 40, 1, STEP_ANGREL, 0, 0);
	bmPlaySound(bm, TRANS_WAIT, SE_KIRA1);
	bmStep(bm, TRANS_WAIT, 10, 1, STEP_ANGPL, 0, 5); // dummy
	bmAccelAngv(bm, TRANS_WAIT, t, 0, rd * 2 / t);
	bmAccelAngv(bm, TRANS_WAIT, t, 0, rd * -2 / t);
	bmJump(bm, 3, 6);

	int w = 1;
	loop(6) {
		bmStepSet(bm, 3, TRANS_WAIT, w, 1, STEP_ANGREL, -rd, 4);
		bmShoot(bm);
		w += 5;
	}
}


async MainSub05() {
	string fn = "Girl04";
	int hp = 400;
	enmNewA(fn, -144, -32, hp, 0); wait(300);
	enmNewAM(fn, 144, -32, hp, 0); wait(300);

	enmNewA(fn, -160, -32, hp, 0); wait(300);
	enmNewAM(fn, 160, -32, hp, 0); wait(300);
}

void Girl04() {
	dvmSFGirl(0);
	hitboxSFGirl();
	dropMain(IT_STAR);
	dropExtra(IT_POWER, 2);
	dropExtra(IT_POINT, 3);
	flagSet(FLAG_INVINCIBLE);

	moveVel(rad(90), 5);
	moveVelTime(120, EASEOUT1, rad(0), 0);
	wait(120);

	flagClear(FLAG_INVINCIBLE);
	Girl04_at();

	moveVel(rad(-90), 0);
	moveVelTime(60, EASEIN1, NEGF, 3);

	loop wait(1000);
}

void Girl04_at() {
	const int bm = 0;
	bmSet(bm, AIM_AT_R, B8B, BLUE, 5, 1, 0, 0, 3, 1);
	bmCloud(bm, CLOUD_MEDIUM);
	bmStep(bm, TRANS_WAIT, 60, 1, STEP_ANGREL, 0, 1);
	bmWait(bm, 30);
	bmAccelAngv(bm, TRANS_WAIT, 60, 1.5 / 60, 0);

	loop (3) {
		float r1 = 0;
		loop(5) {
			bmStepSet(bm, 1, TRANS_WAIT, 60, 1, STEP_ANGREL, r1, 0.5);
			float r2 = rad(-40);
			loop(5) {
				bmAccelAngvSet(bm, 3, TRANS_WAIT, 60, 0.5 / 60, r2 / 60);
				bmShoot(bm);
				bmAccelAngvSet(bm, 3, TRANS_WAIT, 60, 1.0 / 60, r2 / 60);
				bmShoot(bm);
				r2 += rad(20);
			}
			r1 += TAU / 5;
		}
		wait(120);
	}
}

async MainSub06() {
	string fn = "Girl05";
	int hp = 40, w = 30;
	wait(150);
	enmNewA(fn, 96, -32, hp, GIRL_R); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_B); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_R); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_B); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_R); wait(w);

	wait(150);
	enmNewAM(fn, -96, -32, hp, GIRL_B); wait(w);
	enmNewAM(fn, -96, -32, hp, GIRL_R); wait(w);
	enmNewAM(fn, -96, -32, hp, GIRL_B); wait(w);
	enmNewAM(fn, -96, -32, hp, GIRL_R); wait(w);
	enmNewAM(fn, -96, -32, hp, GIRL_B); wait(w);

	wait(150);
	enmNewA(fn, 96, -32, hp, GIRL_R); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_B); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_R); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_B); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_R); wait(w);

	wait(150);
	enmNewAM(fn, -96, -32, hp, GIRL_B); wait(w);
	enmNewAM(fn, -96, -32, hp, GIRL_R); wait(w);
	enmNewAM(fn, -96, -32, hp, GIRL_B); wait(w);
	enmNewAM(fn, -96, -32, hp, GIRL_R); wait(w);
	enmNewAM(fn, -96, -32, hp, GIRL_B); wait(w);

	wait(150);
	enmNewA(fn, 96, -32, hp, GIRL_R); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_B); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_R); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_B); wait(w);
	enmNewA(fn, 96, -32, hp, GIRL_R); wait(w);
}

void Girl05() {
	setGirl();
	moveVel(rad(90), 2);
	//moveVelTime(120, EASEOUT1, rad(120), NEGF);
	moveEllipseRel(rad(90), rad(4), 32, 0, rad(90), 0);

	Girl05_at();

	loop wait(1000);
}

async Girl05_at() {
	bmSet(0, AIM_AT, B32A2, RED32, 5, 1, 0, rad(20), 4, 1);
	bmCloud(0, CLOUD_MEDIUM);
	bmFast(0, TRANS_WAIT);

	wait(20 + RI % 60);
	loop (5) {
		bmShoot(0);
		wait(10);
	}
}

async MainSub07() {
	string fn = "Cir01";
	string fn2 = "Cir01B";
	const int w = 5;
	
	F0 = rad(170); enmNewA(fn, -128, 32, 1000, 0); wait(w);
	F0 = rad(150); enmNewA(fn, -96, 64, 1000, 0); wait(w);
	F0 = rad(130); enmNewA(fn, -64, 96, 1000, 0); wait(w);
	F0 = rad(110); enmNewA(fn, -32, 128, 1000, 0); wait(w);
	F0 = rad(90); enmNewA(fn, 0, 160, 1000, 0); wait(w);
	F0 = rad(70); enmNewA(fn, 32, 192, 1000, 0); wait(w);
	F0 = rad(50); enmNewA(fn, 64, 224, 1000, 0); wait(w);
	F0 = rad(30); enmNewA(fn, 96, 256, 1000, 0); wait(w);
	F0 = rad(10); enmNewA(fn, 128, 288, 1000, 0); wait(w + 30);

	F0 = rad(10); enmNewA(fn, 128, 32, 1000, 0); wait(w);
	F0 = rad(30); enmNewA(fn, 96, 64, 1000, 0); wait(w);
	F0 = rad(50); enmNewA(fn, 64, 96, 1000, 0); wait(w);
	F0 = rad(70); enmNewA(fn, 32, 128, 1000, 0); wait(w);
	F0 = rad(90); enmNewA(fn, 0, 160, 1000, 0); wait(w);
	F0 = rad(110); enmNewA(fn, -32, 192, 1000, 0); wait(w);
	F0 = rad(130); enmNewA(fn, -64, 224, 1000, 0); wait(w);
	F0 = rad(150); enmNewA(fn, -96, 256, 1000, 0); wait(w);
	F0 = rad(170); enmNewA(fn, -128, 288, 1000, 0); wait(w + 30);

	F0 = rad(90); enmNewA(fn, -96, 32, 1000, 0); wait(w);
	F0 = rad(-90); enmNewA(fn, -64, 416, 1000, 0); wait(w);
	F0 = rad(90); enmNewA(fn, -32, 32, 1000, 0); wait(w);
	F0 = rad(-90); enmNewA(fn, 0, 416, 1000, 0); wait(w);
	F0 = rad(90); enmNewA(fn, 32, 32, 1000, 0); wait(w);
	F0 = rad(-90); enmNewA(fn, 64, 416, 1000, 0); wait(w);
	F0 = rad(90); enmNewA(fn, 96, 32, 1000, 0); wait(w + 120);

	enmNewA(fn2, maxf(-192, PLAYER_X - 160), PLAYER_Y, 1000, 0); wait(5);
	enmNewA(fn2, PLAYER_X, maxf(0, PLAYER_Y - 160), 1000, 0); wait(5);
	enmNewA(fn2, minf(192, PLAYER_X + 160), PLAYER_Y, 1000, 0); wait(5);
	enmNewA(fn2, PLAYER_X, minf(448, PLAYER_Y + 160), 1000, 0); wait(5 + 30);

	enmNewA(fn2, maxf(-192, PLAYER_X - 128), maxf(0, PLAYER_Y - 128), 1000, 0); wait(5);
	enmNewA(fn2, minf(192, PLAYER_X + 128), maxf(0, PLAYER_Y - 128), 1000, 0); wait(5);
	enmNewA(fn2, minf(192, PLAYER_X + 128), minf(448, PLAYER_Y + 128), 1000, 0); wait(5);
	enmNewA(fn2, maxf(-192, PLAYER_X - 128), minf(448, PLAYER_Y + 128), 1000, 0); wait(5);
}

void Cir01() {
	dvmSet(0, "enmMagic");
	flagSet(FLAG_NO_HIT | FLAG_NO_HURT);
	sfx(SE_KIRA3);
	wait(60);

	Cir01_at();

	dvmDelete(0);
	wait(60);
	die();

	loop wait(1000);
}

void Cir01_at() {
	bmSet(0, AIM_ST, B8A, RED, 5, 3, F0, PI / 96, 3, 1);
	bmCloud(0, CLOUD_MEDIUM);
	bmShoot(0);
}

void Cir01B() {
	dvmSet(0, "enmMagic");
	flagSet(FLAG_NO_HIT | FLAG_NO_HURT);
	sfx(SE_KIRA3);
	wait(60);

	Cir01B_at();

	dvmDelete(0);
	wait(60);
	die();

	loop wait(1000);
}

void Cir01B_at() {
	bmSet(0, AIM_AT, B8A, BLUE, 5, 3, 0, PI / 96, 3, 1);
	bmCloud(0, CLOUD_MEDIUM);
	bmShoot(0);
}


void main() {
	dvmFile("dan_enemy");
	dvmFile("st02"); // TEMP
	dvmFile("bha");
	bgFile("bha");
	bgCall("main");
	bgm(4);
	wait(60);

	MainSub00();
	MainSub01();
	wait(740);

	MainSub02();
	wait(480);

	MainSub03();
	wait(1080);

	enmNewA("MBoss", 128, -32, 1000, 0);
	wait(1400);

	MainSub04();
	wait(720);
	bossWait();

	MainSub05();
	MainSub06();
	wait(1660);

	MainSub07();
	wait(720);

	bgmStop();
	enmNewA("Boss", -128, -32, 1000, 0);
	wait(120);
	bossWait();
	wait(300);
	end();

	loop wait(1000);
}