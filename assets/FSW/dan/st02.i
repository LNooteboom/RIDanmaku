import "danmaku";
import "st02mbs";
import "st02bs";

// SUB 00

async MainSub00() {
	enmNewA("Girl00", 224,  64, 100, GIRL_R); wait(20);
	enmNewA("Girl00", 224,  96, 100, GIRL_B); wait(20);
	enmNewA("Girl00", 224, 128, 100, GIRL_R); wait(20);
	enmNewA("Girl00", 224, 160, 100, GIRL_B); wait(20);
	enmNewA("Girl00", 224, 192, 100, GIRL_R); wait(40);

	enmNewAM("Girl00", -224, 64, 100, GIRL_R); wait(20);
	enmNewAM("Girl00", -224, 96, 100, GIRL_B); wait(20);
	enmNewAM("Girl00", -224, 128, 100, GIRL_R); wait(20);
	enmNewAM("Girl00", -224, 160, 100, GIRL_B); wait(20);
	enmNewAM("Girl00", -224, 192, 100, GIRL_R); wait(40);

	enmNewA("Girl00", 224, 224, 100, GIRL_B); wait(20);
	enmNewA("Girl00", 224, 256, 100, GIRL_R); wait(20);
	enmNewA("Girl00", 224, 288, 100, GIRL_B); wait(20);
	enmNewA("Girl00", 224, 320, 100, GIRL_R); wait(20);
	enmNewA("Girl00", 224, 352, 100, GIRL_B); wait(40);

	enmNewAM("Girl00", -224, 224, 100, GIRL_B); wait(20);
	enmNewAM("Girl00", -224, 256, 100, GIRL_R); wait(20);
	enmNewAM("Girl00", -224, 288, 100, GIRL_B); wait(20);
	enmNewAM("Girl00", -224, 320, 100, GIRL_R); wait(20);
	enmNewAM("Girl00", -224, 352, 100, GIRL_B); wait(40);
}
void Girl00() {
	setGirl();
	dropExtra(IT_POWER, 1);
	movePosTime(120, LINEAR, -224, 64);
	Girl00_at();
	wait(120);
	die();
}
async Girl00_at() {
	wait(10 + RI % 20);
	bmSet(0, AIM_ST, B16E, BLUE, diffI(1, 2, 2, 2), 1, rad(-90), rad(40), 3, 1);
	bmSound(0, SE_SHOT1, -1);
	bmCloud(0, CLOUD_SHORT);
	bmAccel(0, TRANS_WAIT, 60, diffF(4.5 / 60, 4.5 / 60, 5.0 / 60, 5.5 / 60), rad(90));

	loop (diffI(6, 8, 10, 15)) {
		bmAngle(0, rad(-90) + RRAD / 8, rad(40));
		bmShoot(0);
		//wait(5);
		diffWait(14, 10, 7, 5);
	}
}

// SUB 01

async MainSub01() {
	enmNewA("Cir00", -64, -32, 500, CIR_B); wait(20);
	enmNewA("Cir00B", 64, -32, 500, CIR_B); wait(180);
	enmNewA("Cir00", 0, -32, 500, CIR_B); wait(180);
	enmNewA("Cir00", -96, -32, 500, CIR_B); //wait(20);
	enmNewA("Cir00B", 96, -32, 500, CIR_B); wait(180);
	enmNewA("Cir00", 64, -32, 500, CIR_B); //wait(20);
	enmNewA("Cir00B", -64, -32, 500, CIR_B); wait(180);
}
void Cir00() {
	setCir();
	dropMain(IT_STAR);
	dropExtra(IT_POINT, 6);
	dropExtra(IT_POWER, 5);
	float x = POS_X;
	movePosTime(40, EASEOUT1, x, 128);
	wait(40);
	Cir00_at(rad(3));
	wait(60);
	movePosTime(40, EASEIN1, x, -32);
	wait(40);
	die();
}
void Cir00B() {
	setCir();
	dropMain(IT_STAR);
	dropExtra(IT_POINT, 6);
	dropExtra(IT_POWER, 5);
	float x = POS_X;
	movePosTime(40, EASEOUT1, x, 128);
	wait(40);
	Cir00_at(rad(-3));
	wait(90);
	movePosTime(40, EASEIN1, x, -32);
	wait(40);
	die();
}
void Cir00_at(float rd) {
	bmSet(0, AIM_ST_R, B16C, BLUE, diffI(2, 3, 4, 5), 1, 0, rad(10), 5, 1);
	bmSound(0, SE_SHOT1, -1);
	bmCloud(0, CLOUD_MEDIUM);
	bmStep(0, TRANS_WAIT, 20, 1, STEP_ANGREL, 0, 0);
	float r = 0;
	float s = diffF(1.5, 1.7, 2.0, 2.5);
	float r2 = rad(30);
	loop (diffI(15, 20, 20, 25)) {
		bmAngle(0, r, rad(10));
		bmStepSet(0, 2, TRANS_WAIT, 30, 1, STEP_ANGREL, r2, s);
		bmShoot(0);
		r += rd;
		wait(3);

		bmAngle(0, r, rad(10));
		bmStepSet(0, 2, TRANS_WAIT, 30, 1, STEP_ANGREL, -r2, s);
		bmShoot(0);
		r += rd;
		wait(3);
	}
}

//SUB 02
async MainSub02() {
	wait(60);
	enmNewA("Girl01", -128, -32, 100, GIRL_R); wait(20);
	enmNewA("Girl01", -128, -32, 100, GIRL_R); wait(20);
	enmNewA("Girl01", -128, -32, 100, GIRL_R); wait(20);
	enmNewA("Girl01", -128, -32, 100, GIRL_R); wait(20);
	enmNewA("Girl01", -128, -32, 100, GIRL_R); wait(20);
	enmNewA("Girl01", -128, -32, 100, GIRL_R); wait(80);
	enmNewAM("Girl01", 128, -32, 100, GIRL_B); wait(20);
	enmNewAM("Girl01", 128, -32, 100, GIRL_B); wait(20);
	enmNewAM("Girl01", 128, -32, 100, GIRL_B); wait(20);
	enmNewAM("Girl01", 128, -32, 100, GIRL_B); wait(20);
	enmNewAM("Girl01", 128, -32, 100, GIRL_B); wait(20);
	enmNewAM("Girl01", 128, -32, 100, GIRL_B); wait(80);
	enmNewA("Girl01", -128, -32, 100, GIRL_B); wait(20);
	enmNewA("Girl01", -128, -32, 100, GIRL_B); wait(20);
	enmNewA("Girl01", -128, -32, 100, GIRL_B); wait(20);
	enmNewA("Girl01", -128, -32, 100, GIRL_B); wait(20);
	enmNewA("Girl01", -128, -32, 100, GIRL_B); wait(20);
	enmNewA("Girl01", -128, -32, 100, GIRL_B); wait(80);
	enmNewAM("Girl01", 128, -32, 100, GIRL_R); wait(20);
	enmNewAM("Girl01", 128, -32, 100, GIRL_R); wait(20);
	enmNewAM("Girl01", 128, -32, 100, GIRL_R); wait(20);
	enmNewAM("Girl01", 128, -32, 100, GIRL_R); wait(20);
	enmNewAM("Girl01", 128, -32, 100, GIRL_R); wait(20);
	enmNewAM("Girl01", 128, -32, 100, GIRL_R); wait(80);
}
void Girl01() {
	setGirl();
	moveVel(rad(90), 4);
	if (DIFF != EASY)
		Girl01_at();
	wait(30);
	moveVelTime(40, LINEAR, rad(0), 4);
	loop wait(1000);
}
async Girl01_at() {
	bmSet(0, AIM_AT, B16B, RED, diffI(1, 1, 1, 3), 1, 0, rad(30), diffF(2, 2.5, 3.5, 4.5), 1);
	bmCloud(0, CLOUD_SHORT);
	wait(20 + RI % 20);
	loop (diffI(1, 1, 3, 5)) {
		bmShoot(0);
		wait(20);
	}
}

// SUB 03

async MainSub03() {
	loop (2) {
		enmNewA("Girl02",    0, -32, 100, GIRL_B | 4); wait(20);
		enmNewA("Girl02",  -16, -32, 100, GIRL_R);
		enmNewA("Girl02",   16, -32, 100, GIRL_R); wait(20);
		enmNewA("Girl02",  -32, -32, 100, GIRL_R);
		enmNewA("Girl02",   32, -32, 100, GIRL_R); wait(20);
		enmNewA("Girl02",  -48, -32, 100, GIRL_B);
		enmNewA("Girl02",   48, -32, 100, GIRL_B); wait(20);
		enmNewA("Girl02",  -64, -32, 100, GIRL_B);
		enmNewA("Girl02",   64, -32, 100, GIRL_B); wait(20);
		enmNewA("Girl02",  -96, -32, 100, GIRL_G);
		enmNewA("Girl02",   96, -32, 100, GIRL_G); wait(20);
		enmNewA("Girl02", -128, -32, 100, GIRL_G);
		enmNewA("Girl02",  128, -32, 100, GIRL_G); wait(20);
		enmNewA("Girl02", -144, -32, 100, GIRL_Y);
		enmNewA("Girl02",  144, -32, 100, GIRL_Y); wait(20);
		enmNewA("Girl02", -160, -32, 100, GIRL_Y);
		enmNewA("Girl02",  160, -32, 100, GIRL_Y); wait(20);
	}
}
void Girl02() {
	setGirl();
	dropExtra(IT_POWER, 1);
	float x, y;
	circlePos(x, y, RRAD, RF * 32);
	x += PLAYER_X;
	y = maxf(y + PLAYER_Y - 128, 128);
	moveBezier(90, 0, 512, x, y, 0, 0);
	Girl02_at();
	wait(90);
	moveVel(AIM + rad(180), 0);
	moveVelTime(60, LINEAR, NEGF, 4);
	loop wait(1000);
}
async Girl02_at() {
	wait(20 + RI % 20);
	bmSet(0, AIM_AT, B16A, RED, diffI(1, 1, 1, 3), 1, 0, rad(20), diffF(3, 4, 5, 6), 1);
	bmCloud(0, CLOUD_MEDIUM);
	loop (diffI(1, 5, 8, 10)) {
		bmShoot(0);
		diffWait(10, 10, 6, 4);
	}
}

// SUB 04

void MainSub04() {
	loop(10) {
		enmNewAF("Girl03", 224 + RF * 16, 0, 100, GIRL_R); wait(10);
		enmNewAF("Girl03", 224 + RF * 16, 0, 100, GIRL_B); wait(10);
	}
	wait(120);
	loop(10) {
		enmNewAMF("Girl03", -224 - RF * 16, 0, 100, GIRL_R); wait(10);
		enmNewAMF("Girl03", -224 - RF * 16, 0, 100, GIRL_B); wait(10);
	}
	wait(120);

	loop(10) {
		enmNewAF("Girl03", 224 + RF * 16, 0, 100, GIRL_R);
		enmNewAMF("Girl03", -224 - RF * 16, 16, 100, GIRL_R); wait(10);

		enmNewAF("Girl03", 224 + RF * 16, 0, 100, GIRL_B);
		enmNewAMF("Girl03", -224 - RF * 16, 16, 100, GIRL_B); wait(10);
	}
}
void Girl03() {
	setGirl();
	flagSet(FLAG_OFFSCR_LR);
	moveVel(rad(135), 5);
	//moveVelTime(30, EASEOUT1, rad(135), 5);
	if (DIFF)
		Girl03_at();
	wait(40);
	flagClear(FLAG_OFFSCR_LR);
	moveVelTime(120, LINEAR, rad(160) + TAU, 5);
	
	loop wait(1000);
}
async Girl03_at() {
	bmSet(0, AIM_AT, B32A, I0 ? BLUE32 : CYAN32, diffI(1, 1, 1, 3), 1, 0, diffF(0, 0, rad(20), rad(12)), diffF(4, 5, 6, 6), 1);
	bmCloud(0, CLOUD_MEDIUM);
	bmFast(0, TRANS_ASYNC);
	wait(10 + RI % 40);
	loop (diffI(6, 6, 7, 8)) {
		bmShoot(0);
		diffWait(30, 30, 25, 20);
	}
}

// SUB 05

async MainSub05() {
	loop(5) {
		enmNewA("Girl04", -224, 64 + RF * 64, 100, GIRL_R);
		enmNewAM("Girl04", 224, 64 - RF * 64, 100, GIRL_R);
		wait(10);
		enmNewA("Girl04", -224, 64 + RF * 64, 100, GIRL_B);
		enmNewAM("Girl04", 224, 64 - RF * 64, 100, GIRL_B);
		wait(10);
	}
	wait(120);
	loop(5) {
		enmNewA("Girl04", -224, 64 + RF * 64, 100, GIRL_R);
		enmNewAM("Girl04", 224, 64 - RF * 64, 100, GIRL_R);
		wait(10);
		enmNewA("Girl04", -224, 64 + RF * 64, 100, GIRL_B);
		enmNewAM("Girl04", 224, 64 - RF * 64, 100, GIRL_B);
		wait(10);
	}
	wait(120);
	loop(15) {
		enmNewA("Girl04", -224, 64 + RF * 64, 100, GIRL_R);
		enmNewAM("Girl04", 224, 64 - RF * 64, 100, GIRL_R);
		wait(10);
		enmNewA("Girl04", -224, 64 + RF * 64, 100, GIRL_B);
		enmNewAM("Girl04", 224, 64 - RF * 64, 100, GIRL_B);
		wait(10);
	}
}
void Girl04() {
	setGirl();
	if (DIFF >= NORMAL)
		onDeath("Girl04Dead");
	moveVel(rad(20), 5.0);
	Girl04_at();
	loop wait(1000);
}
async Girl04_at() {
	bmSet(0, AIM_MEEK, B16A, DARKBLUE + RI % 4, 1, 1, rad(-90), rad(45), 3, 2);
	bmSound(0, SE_SHOT1, -1);
	bmCloud(0, CLOUD_MEDIUM);
	bmAccel(0, TRANS_WAIT, 120, 6.0 / 120, rad(90));
	wait(10 + RI % 40);
	loop (diffI(6, 7, 8, 10)) {
		bmShoot(0);
		diffWait(16, 14, 12, 10);
	}
}
void Girl04Dead() {
	float s = diffF(4, 4, 5, 5);
	float rd = PI / 256;
	bmSet(0, AIM_AT_R, B16C, GREEN, 8, diffI(1, 3, 3, 5), diffF(0, -rd, -rd, rd * -2), rd, s, s);
	bmCloud(0, CLOUD_LONG);
	bmShoot(0);
}

// MAIN

void main() {
	dvmFile("dan_enemy");
	dvmFile("st02");
	bgFile("st02");
	bgCall("main");
	//bgm(4);
	wait(60);
	
	MainSub00();
	wait(700); //760
	MainSub02();
	MainSub01();
	wait(820); //1580
	MainSub03();
	wait(480); //2060

	enmNewA("MBoss", 128, -32, 10000, 0);
	wait(1400);
	MainSub04();
	wait(180);
	bossWait();
	MainSub05();
	wait(1000);

	dialogStart("st02a");
	dialogWait();
	enmNewA("Boss", 128, -32, 10000, 0);
	wait(60);
	bossWait();
	wait(600);
	end();

	loop wait(1000);
}