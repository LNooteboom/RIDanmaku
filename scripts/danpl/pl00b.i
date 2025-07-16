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
	string dvm = "rinBulletWind";
	loop {
		if !(SHOOT) {
			wait(1);
			continue;
		}

		if (FOCUS) {
			switch (I0) {
			case 0: break;
			case 1:
				bulletNew(0, 0, 0, rad(-90), s, 16, 32, 14, 0, dvm);
				break;
			case 2:
				bulletNew(0, 0, 0, rad(-85), s, 16, 32, 8, 0, dvm);
				bulletNew(1, 0, 0, rad(-95), s, 16, 32, 8, 0, dvm);
				break;
			case 3:
				bulletNew(0, 0, 0, rad(-83), s, 16, 32, 6, 0, dvm);
				bulletNew(1, 0, 0, rad(-97), s, 16, 32, 6, 0, dvm);
				bulletNew(2, 0, 0, rad(-90), s, 16, 32, 6, 0, dvm);
				break;
			case 4:
				bulletNew(0, 0, 0, rad(-83), s, 16, 32, 5, 0, dvm);
				bulletNew(1, 0, 0, rad(-97), s, 16, 32, 5, 0, dvm);
				bulletNew(2, 0, 0, rad(-85), s, 16, 32, 5, 0, dvm);
				bulletNew(3, 0, 0, rad(-95), s, 16, 32, 5, 0, dvm);
				break;
			}
		} else {
			switch (I0) {
			case 0: break;
			case 1:
				bulletNew(0, 0, 0, rad(-90), s, 16, 32, 14, 0, dvm);
				break;
			case 2:
				bulletNew(0, 0, 0, rad(-92), s, 16, 32, 8, 0, dvm);
				bulletNew(1, 0, 0, rad(-88), s, 16, 32, 8, 0, dvm);
				break;
			case 3:
				bulletNew(0, 0, 0, rad(-93), s, 16, 32, 6, 0, dvm);
				bulletNew(1, 0, 0, rad(-87), s, 16, 32, 6, 0, dvm);
				bulletNew(2, 0, 0, rad(-90), s, 16, 32, 6, 0, dvm);
				break;
			case 4:
				bulletNew(0, 0, 0, rad(-91), s, 16, 32, 5, 0, dvm);
				bulletNew(1, 0, 0, rad(-89), s, 16, 32, 5, 0, dvm);
				bulletNew(2, 0, 0, rad(-93), s, 16, 32, 5, 0, dvm);
				bulletNew(3, 0, 0, rad(-87), s, 16, 32, 5, 0, dvm);
				break;
			}
		}
		
		wait(4);
	}
}

void Bomb() {
	sfx(SE_SLASH);
	sfx(SE_BOMB1);
	optionNew(4, POS_X, POS_Y, "rinBombB");
	for (int i = 0; i < 640; i++) {
		if (i == 60)
			onBombHit("BombHit");
		optionMovePos(4, 0, 0, 0.3);
		wait(1);
	}
	onBombHit("");
	sfx(SE_SLASH);
	itemNew(POS_X, POS_Y - 24, 0);
	itemNew(POS_X - 24, POS_Y - 16, 0);
	itemNew(POS_X + 24, POS_Y - 16, 0);
	itemNew(POS_X - 48, POS_Y - 4, 0);
	itemNew(POS_X + 48, POS_Y - 4, 0);
	//wait(60);
	bombEnd();
}

void BombHit() {
	kill(BOMB);
	sfx(SE_DENY);
	sfx(SE_SLASH);
	optionEvent(4, 2);
	for (int i = 0; i < 10; i++) {
		bombCircle(POS_X, POS_Y, modeLerp(64, 128, i / 10.0, EASEIN1), 0, 1);
		optionMovePos(4, 0, 0, 0.3);
		wait(1);
	}
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
	loop{
		int newOptions = DEATH ? 0 : (POWER > 400 ? 4 : POWER / 100);
		if (newOptions != I0) {
			for (int i = 0; i < I0; i++) {
				optionDelete(i);
			}
			for (int i = 0; i < newOptions; i++) {
				optionNew(i, POS_X, POS_Y, "rinOptionP");
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
				optionMovePos(0, 0, -24, strength);
				break;
			case 2:
				optionMovePos(0, -12, -24, strength);
				optionMovePos(1, +12, -24, strength);
				break;
			case 3:
				optionMovePos(0, -16, -20, strength);
				optionMovePos(1, +16, -20, strength);
				optionMovePos(2, 0, -28, strength);
				break;
			case 4:
				optionMovePos(0, -8, -28, strength);
				optionMovePos(1, +8, -28, strength);
				optionMovePos(2, -24, -20, strength);
				optionMovePos(3, +24, -20, strength);
				break;
			}
		} else {
			switch (I0) {
			case 0:
			default:
				break;
			case 1:
				optionMovePos(0, 0, -36, strength);
				break;
			case 2:
				optionMovePos(0, -24, -16, strength);
				optionMovePos(1, +24, -16, strength);
				break;
			case 3:
				optionMovePos(0, -28, -16, strength);
				optionMovePos(1, +28, -16, strength);
				optionMovePos(2, 0, -36, strength);
				break;
			case 4:
				optionMovePos(0, -20, -20, strength);
				optionMovePos(1, +20, -20, strength);
				optionMovePos(2, -36, -12, strength);
				optionMovePos(3, +36, -12, strength);
				break;
			}
		}
		wait(1);
	}
}
