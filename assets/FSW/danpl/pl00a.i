import "player";

async MainShot() {
	loop {
		if (SHOOT) {
			sfx(SE_PLST);
			bulletNew(-1, -6, -24, rad(-90), 24, 18, 48, 15, 0, "rinBulletMain");
			bulletNew(-1, 6, -24, rad(-90), 24, 18, 48, 15, 0, "rinBulletMain");
			wait(3);
		} else {
			wait(1);
		}
	}
}

async OptionShot() {
	const float s = 16; // speed
	string dvm = "rinBulletFlower";
	loop {
		if !(SHOOT) {
			wait(1);
			continue;
		}

		if (FOCUS) {
			switch (I0) {
			case 0:
				wait(3);
				break;
			case 1:
				bulletNew(0, 0, 0, rad(-95), s, 16, 16, 16, 0, dvm);
				wait(3);
				bulletNew(0, 0, 0, rad(-85), s, 16, 16, 16, 0, dvm);
				break;
			case 2:
				bulletNew(0, 0, 0, rad(-90), s, 16, 16, 12, 0, dvm);
				bulletNew(1, 0, 0, rad(-90), s, 16, 16, 12, 0, dvm);
				wait(3);
				bulletNew(0, 0, 0, rad(-98), s, 16, 16, 12, 0, dvm);
				bulletNew(1, 0, 0, rad(-82), s, 16, 16, 12, 0, dvm);
				break;
			case 3:
				bulletNew(0, 0, 0, rad(-90), s, 16, 16, 11, 0, dvm);
				bulletNew(1, 0, 0, rad(-90), s, 16, 16, 11, 0, dvm);
				bulletNew(2, 0, 0, rad(-95), s, 16, 16, 11, 0, dvm);
				wait(3);
				bulletNew(0, 0, 0, rad(-100), s, 16, 16, 11, 0, dvm);
				bulletNew(1, 0, 0, rad(-80), s, 16, 16, 11, 0, dvm);
				bulletNew(2, 0, 0, rad(-85), s, 16, 16, 11, 0, dvm);
				break;
			case 4:
				bulletNew(0, 0, 0, rad(-89), s, 16, 16, 10, 0, dvm);
				bulletNew(1, 0, 0, rad(-91), s, 16, 16, 10, 0, dvm);
				bulletNew(2, 0, 0, rad(-95), s, 16, 16, 10, 0, dvm);
				bulletNew(3, 0, 0, rad(-85), s, 16, 16, 10, 0, dvm);
				wait(3);
				bulletNew(0, 0, 0, rad(-95), s, 16, 16, 10, 0, dvm);
				bulletNew(1, 0, 0, rad(-85), s, 16, 16, 10, 0, dvm);
				bulletNew(2, 0, 0, rad(-100), s, 16, 16, 10, 0, dvm);
				bulletNew(3, 0, 0, rad(-80), s, 16, 16, 10, 0, dvm);
				break;
			}
		} else {
			switch (I0) {
			case 0:
				wait(3);
				break;
			case 1:
				bulletNew(0, 0, 0, rad(-100), s, 16, 16, 16, 0, dvm);
				wait(3);
				bulletNew(0, 0, 0, rad(-80), s, 16, 16, 16, 0, dvm);
				break;
			case 2:
				bulletNew(0, 0, 0, rad(-90), s, 16, 16, 12, 0, dvm);
				bulletNew(1, 0, 0, rad(-90), s, 16, 16, 12, 0, dvm);
				wait(3);
				bulletNew(1, 0, 0, rad(-80), s, 16, 16, 12, 0, dvm);
				bulletNew(0, 0, 0, rad(-100), s, 16, 16, 12, 0, dvm);
				break;
			case 3:
				bulletNew(0, 0, 0, rad(-100), s, 16, 16, 11, 0, dvm);
				bulletNew(1, 0, 0, rad(-80), s, 16, 16, 11, 0, dvm);
				bulletNew(2, 0, 0, rad(-95), s, 16, 16, 11, 0, dvm);
				wait(3);
				bulletNew(0, 0, 0, rad(-105), s, 16, 16, 11, 0, dvm);
				bulletNew(1, 0, 0, rad(-75), s, 16, 16, 11, 0, dvm);
				bulletNew(2, 0, 0, rad(-85), s, 16, 16, 11, 0, dvm);
				break;
			case 4:
				bulletNew(0, 0, 0, rad(-90), s, 16, 16, 10, 0, dvm);
				bulletNew(1, 0, 0, rad(-90), s, 16, 16, 10, 0, dvm);
				bulletNew(2, 0, 0, rad(-100), s, 16, 16, 10, 0, dvm);
				bulletNew(3, 0, 0, rad(-80), s, 16, 16, 10, 0, dvm);
				wait(3);
				bulletNew(0, 0, 0, rad(-98), s, 16, 16, 10, 0, dvm);
				bulletNew(1, 0, 0, rad(-82), s, 16, 16, 10, 0, dvm);
				bulletNew(2, 0, 0, rad(-105), s, 16, 16, 10, 0, dvm);
				bulletNew(3, 0, 0, rad(-75), s, 16, 16, 10, 0, dvm);
				break;
			}
		}
		wait(3);
	}
}

void Bomb() {
	float sx = POS_X, sy = POS_Y;
	float dy = sy - 128;
	if (dy < 96)
		dy = 96;

	float scale;
	if (0 && GAUGE >= GAUGE_MAX) {
		GAUGE = 0;
		scale = 2.0;
		optionNew(4, sx, sy, "rinBombABig");
	} else {
		scale = 1.0;
		optionNew(4, sx, sy, "rinBombA");
	}
	
	speed(4.5 / 2.0, 2 / 2.0);
	sfx(SE_SLASH);
	shake(300, 1, 1);
	bombAutoCollect(320);
	for (int i = 0; i < 300; i++) {
		if (i == 10)
			sfx(SE_BOMB2);

		float y = modeLerp(sy, dy, i / 300.0, EASEOUT1);
		optionSetPos(4, sx, y);
		bombCircle(sx, y, lerp(32, 64, i / 300.0) * scale, 10, 1);

		wait(1);
	}

	sfxStop(SE_BOMB2);
	sfx(SE_SLASH);
	shake(20, 20, 0);
	for (int i = 0; i < 20; i++) {
		bombCircle(sx, dy, lerp(64, 128, i / 20.0) * scale, 40, 1);
		wait(1);
	}

	speed(4.5, 2);
	optionDelete(4);
	wait(60);
	bombEnd();
}

void optionMovePos(int opt, float xoffs, float yoffs, float strength) {
	float curX, curY;
	optionGetPos(opt, curX, curY);
	float dstX = POS_X + xoffs, dstY = POS_Y + yoffs;
	optionSetPos(opt, lerp(curX, dstX, strength), lerp(curY, dstY, strength));
}

void main() {
	hitbox(1);
	grazebox(23);
	speed(4.5, 2);
	maxPower(400, 5);
	itemMagnetRange(32, 48);

	dvmFile("danpl_rin");
	dvmMain("rin");

	onBomb("Bomb");

	MainShot();
	OptionShot();

	I0 = 0; // curOptions
	loop {
		int newOptions = DEATH ? 0 : (POWER > 400 ? 4 : POWER / 100);
		if (newOptions != I0) {
			for (int i = 0; i < I0; i++) {
				optionDelete(i);
			}
			for (int i = 0; i < newOptions; i++) {
				optionNew(i, POS_X, POS_Y, "rinOptionG");
			}
			I0 = newOptions;
		}

		const float strength = 0.3f;
		if (FOCUS) {
			switch (I0) {
			case 0:
			default:
				break;
			case 1:
				optionMovePos(0, 0, 24, strength);
				break;
			case 2:
				optionMovePos(0, -16, 0, strength);
				optionMovePos(1, +16, 0, strength);
				break;
			case 3:
				optionMovePos(0, -16, 0, strength);
				optionMovePos(1, +16, 0, strength);
				optionMovePos(2, 0, 24, strength);
				break;
			case 4:
				optionMovePos(0, -16, 0, strength);
				optionMovePos(1, +16, 0, strength);
				optionMovePos(2, -32, 6, strength);
				optionMovePos(3, +32, 6, strength);
				break;
			}
		} else {
			switch (I0) {
			case 0:
			default:
				break;
			case 1:
				optionMovePos(0, 0, 24, strength);
				break;
			case 2:
				optionMovePos(0, -24, 0, strength);
				optionMovePos(1, +24, 0, strength);
				break;
			case 3:
				optionMovePos(0, -24, 0, strength);
				optionMovePos(1, +24, 0, strength);
				optionMovePos(2, 0, 24, strength);
				break;
			case 4:
				optionMovePos(0, -24, 0, strength);
				optionMovePos(1, +24, 0, strength);
				optionMovePos(2, -40, 6, strength);
				optionMovePos(3, +40, 6, strength);
				break;
			}
		}
		wait(1);
	}
}
