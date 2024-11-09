import "danmaku";

void Boss() {
	dvmSet(0, "bhaBoss");
	dvmSet(1, "effHex");
	hurtbox(40, 40);
	hitbox(16, 16);
	setBoss(0, "Miyoi Okunoda", "");
	invuln(60);
	distort(160, 0xFF80FF);

	lifeSet(8600);
	movePosTime(60, EASEOUT1, 0, 128);
	wait(60);
	dialogWait();
    bgm(5);
	
    bgCall("boss");
	Boss1();
	//BossCard3();

	die();
}

// NON 1

void Boss1() {
	stars(2);
    CAPTURE = 1;
	lifeMarker(0, 1400, 0xFFFFA080);
	setInterrupt(0, 1400, 35 * 60, "BossCard1");
	moveLimit(0, 128, 256, 32);
	F1 = 1;
	loop {
        Boss1_at();
        wait(10);
        Boss1_at2();
        wait(120);

        moveRand(60, LINEAR, 1);
        wait(60);

        Boss1_at2();
        wait(10);
        Boss1_at();
        wait(90);
	}
}

async Boss1_at() {
    bmSet(0, AIM_ST_R, B16G, PINK, 5, 4, 0, 0, 3, 1);
    bmCloud(0, CLOUD_LONG);
    //bmAccel(0, TRANS_WAIT, 180, 3.0 / 180, rad(90));

    float r1 = rad(-45), r2 = 0;
    loop (8) {
        bmOffsetRad(0, r1, 48);
        bmAngle(0, r2, rad(-10));
        bmShoot(0);
        wait(5);
        r1 += rad(270) / 7;
        r2 += rad(10);
    }
}

async Boss1_at2() {
    bmSet(1, AIM_ST_R, B16G, BLUE, 5, 4, 0, 0, 3, 1);
    bmCloud(1, CLOUD_LONG);
    //bmAccel(1, TRANS_WAIT, 180, 3.0 / 180, rad(90));

    float r1 = rad(-135), r2 = 0;
    loop (8) {
        bmOffsetRad(1, r1, 64);
        bmAngle(1, r2, rad(10));
        bmShoot(1);
        wait(5);
        r1 -= rad(270) / 7;
        r2 -= rad(10);
    }
}

// SPELL 1

void BossCard1() {
    setInterrupt(0, 0, 35 * 60, "Boss2");
    spellEx(SPELL_BHA_2E, 35 * 60, 500000, "st2CardA");
    enmKillAll();
    killAll();
    invuln(60);
    movePosTime(60, EASEOUT1, 0, 128);
    moveLimit(0, 128, 128, 64);
    breakWave();
    if (CAPTURE) {
        dropMain(IT_LIFEPIECE);
        dropItems();
    }
    CAPTURE = 1;

    wait(90);

    loop {
        BossCard1_at2(1);
        wait(60);
        BossCard1_at();
        wait(60);
        moveRand(60, LINEAR, 1.0);
        wait(60);
        BossCard1_at2(-1);
        wait(120);
    }
}

async BossCard1_at() {
    bmSet(0, AIM_ST, B32C, RED32, 1, 1, 0, 0, 4, 1);
    bmSound(0, SE_SHOT1, -1);
    bmCloud(0, CLOUD_MEDIUM);
    bmStep(0, TRANS_WAIT, 60, 1, STEP_ANGPL, 0, 0);
    bmPlaySound(0, TRANS_WAIT, SE_KIRA1);
    bmAccel(0, TRANS_WAIT, 60, 3.0 / 60, NEGF);

    float r = 0;
    loop (10) {
        bmAngle(0, r, 0);
        float s = 2.0;
        loop (5) {
            bmAccelSet(0, 3, TRANS_WAIT, 60, s / 60, NEGF);
            bmShoot(0);
            s += 1.0;
        }
        wait(2);
        r -= PI / 10;
    }
}

async BossCard1_at2(float dir) {
    bmSet(1, AIM_ST_R, B32C, PINK32, 3, 2, 0, 0, 3, 1);
    bmSound(1, SE_SHOT1, -1);
    bmDist(1, 32);
    bmCloud(1, CLOUD_MEDIUM);
    bmOffscreen(1, 600);
    bmWrap(1, TRANS_WAIT, 3, BOUNCE_LR | BOUNCE_D, NEGF);

    float r = 0;
    loop (18) {
        bmAngle(1, r, rad(10));
        bmShoot(1);
        wait(1);
        r += rad(19) * dir;
    }
}

// NON 2

void Boss2() {
    timerReset();
    bmClear(640, !TIMEOUT);
    spellEnd();
    moveLimitReset();
    enmKillAll();
    killAll();

    stars(1);
    lifeSet(8800);
    lifeMarker(0, 1600, 0xFFFFA080);
    setInterrupt(0, 1600, 35 * 60, "BossCard2");
    movePosTime(60, EASEOUT1, 0, 128);
    invuln(60);
    wait(90);

    moveLimit(0, 128, 256, 32);
    loop{
        Boss2_at();
        wait(20);
        Boss2_at2();
        wait(60);
        Boss2_at2();
        wait(20);
        Boss2_at();
        wait(120);
    }
}

async Boss2_at() {
    bmSet(0, AIM_ST_R, B16G, RED, 5, 4, 0, 0, 5, 1);
    bmCloud(0, CLOUD_LONG);
    //bmAccel(0, TRANS_WAIT, 180, 3.0 / 180, rad(90));

    float r1 = AIM, r2 = 0;
    float s = 1.5;
    loop(8) {
        bmOffsetRad(0, r1, 48);
        bmAngle(0, r2, rad(-0));
        bmSpeed(0, s, 1);
        bmShoot(0);
        wait(5);
        r1 += TAU / 8;
        r2 += rad(10);
        s += 0.4;
    }
}

async Boss2_at2() {
    bmSet(0, AIM_ST_R, B16G, ORANGE, 5, 4, 0, 0, 3, 1);
    bmCloud(0, CLOUD_LONG);
    //bmAccel(0, TRANS_WAIT, 180, 3.0 / 180, rad(90));

    float r1 = AIM, r2 = 0;
    float s = 3.0;
    loop(8) {
        bmOffsetRad(0, r1, 48);
        bmAngle(0, r2, rad(-0));
        bmSpeed(0, s, 1);
        bmShoot(0);
        wait(5);
        r1 -= TAU / 8;
        r2 += rad(10);
        s += 0.4;
    }
}

// SPELL 2

void BossCard2() {
    setInterrupt(0, 0, 35 * 60, "BossCard3");
    spellEx(SPELL_BHA_3E, 35 * 60, 500000, "st2CardA");
    enmKillAll();
    killAll();
    invuln(60);
    movePosTime(60, EASEOUT1, 0, 128);
    moveLimit(0, 128, 128, 64);
    breakWave();
    if (CAPTURE) {
        dropMain(IT_LIFEPIECE);
        dropItems();
    }
    CAPTURE = 1;
    dvmPlayAbs("bhaCardPortrait", 0, 0, 0);

    wait(90);

    I0 = 1;
    F0 = 4;
    funcSet("BossCard2_func", 2, FUNC_COND_MRKLASERNEAR);
    loop {
        BossCard2_at();
        wait(120);
        BossCard2_at2();
        wait(120);
        moveRand(60, LINEAR, 1);
        wait(120);
    }
}

void BossCard2_func(entity en, float x, float y) {
    bulletSetMark(en, 3);
    bulletSetTrans(en, 4, 1);
    bmOffsetAbs(2, x, y);
    bmAngle(2, RRAD, 0);
    bmShoot(2);
}

async BossCard2_at() {
    dvmPlayAbs("effChargeYellow", PLAYER_X, PLAYER_Y, 0);
    sfx(SE_CHARGE);
    wait(60);

    bmSet(2, AIM_ST_R, B8A, WHITE, 4, 1, 0, 0, 1, 1);
    bmCloud(2, CLOUD_SHORT);

    bmSet(0, AIM_ST_R, B16M, GOLD, 8, 1, 0, 0, 2.5, 1);
    bmDist(0, 64);
    bmCloud(0, CLOUD_MEDIUM);
    bmMark(0, TRANS_WAIT, 2);
    bmBounce(0, TRANS_WAIT, 2, BOUNCE_ULR, 1.5);

    bmDeleteSet(0, 4, 2);
    float r = 0;
    for (int i = 0; i < 15; i++) {
        bmSprite(0, B16M, i % 3);
        bmAngle(0, r, 0);
        bmOffsetAbs(0, PLAYER_X, PLAYER_Y);
        bmShoot(0);
        wait(2);
        r += rad(10);
    }
}

async BossCard2_at2() {
    bmSet(1, AIM_ST, B16C, PINK, 1, 1, 0, 0, 2.0, 1);
    bmDist(1, 32);
    bmMark(1, TRANS_WAIT, 1);
    bmAccel(1, TRANS_WAIT, 60, 2.0 / 60, NEGF);

    sfx(SE_LASER1);
    float r = AIM - PI / 10;
    loop(10) {
        bmAngle(1, r, 0);
        bmLaserStShoot(1, 0, 0, 128, 0, 16);
        wait(5);
        r += TAU / 10;
    }
}

// CARD 3

void BossCard3() {
    lifeSet(2200);
    lifeMarker(0, 2200, 0xFFFFA080);
    funcSet("", 0, 0);

    timerReset();
    bmClear(640, !TIMEOUT);
    spellEnd();
    moveLimitReset();
    enmKillAll();
    killAll();
    stars(0);

    setInterrupt(0, 0, 35 * 60, "BossDead");
    spellEx(SPELL_BHA_4E, 35 * 60, 500000, "st2CardA");
    enmKillAll();
    killAll();
    invuln(60);
    movePosTime(60, EASEOUT1, 0, 128);
    moveLimit(0, 128, 128, 64);
    breakWave();
    if (CAPTURE) {
        dropMain(IT_LIFEPIECE);
        dropItems();
    }
    CAPTURE = 1;
    dvmPlayAbs("bhaCardPortrait", 0, 0, 0);

    wait(90);

    loop{
        BossCard3_at();
        wait(180);
        BossCard3_at2();
        wait(60);
        moveRand(60, LINEAR, 1);
        wait(60);
    }

    loop wait(1000);
}

async BossCard3_at() {
    bmSet(0, AIM_ST_R, B16A, BLUE, 2, 1, 0, 0, 3, 1);
    bmDist(0, -32);
    bmSound(0, SE_SHOT1, -1);
    bmCloud(0, CLOUD_MEDIUM);
    bmStep(0, TRANS_WAIT, 60, 1, STEP_ANGREL, 0, 0);
    bmWait(0, 30);
    bmPlaySound(0, TRANS_WAIT, SE_KIRA1);
    bmAccel(0, TRANS_WAIT, 60, 1.5 / 60, NEGF);

    float r1 = 0;
    loop(50) {
        bmAngle(0, r1, 0);
        float r2 = 0;
        loop (5) {
            bmStepSet(0, 1, TRANS_WAIT, 100, 1, STEP_ANGREL, r2, 0);
            bmShoot(0);
            r2 += TAU / 5;
        }
        wait(3);
        r1 += rad(13);
    }
}

async BossCard3_at2() {
    float rd = rad(60);
    int t = 30;
    bmSet(1, AIM_ST, B16C, PINK, 1, 1, 0, 0, 3, 1);
    bmSound(1, SE_SHOT1, -1);
    bmAccelAngv(1, TRANS_WAIT, t, 0, rd * -2 / t);
    bmAccelAngv(1, TRANS_WAIT, t, 0, rd * 2 / t);
    bmJump(1, -1, 10);

    bmSet(2, AIM_ST, B16C, PINK, 1, 1, 0, 0, 3, 1);
    bmSound(2, SE_SHOT1, -1);
    bmAccelAngv(2, TRANS_WAIT, t, 0, rd * 2 / t);
    bmAccelAngv(2, TRANS_WAIT, t, 0, rd * -2 / t);
    bmJump(2, -1, 10);

    float r = AIM;
    sfx(SE_LASER1);
    loop(12) {
        bmAngle(1, r + rd, 0);
        bmLaserCuShoot(1, 0, 24, 10, 60, 1000);
        bmAngle(2, r - rd, 0);
        bmLaserCuShoot(2, 0, 24, 10, 60, 1000);
        r += TAU / 12;
    }
}

// DEAD

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