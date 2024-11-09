import "player";

async ShootMain() {
	loop {
		if (SHOOT) {
			sfx(SE_PLST);
			bulletNew(-1, -8, -24, rad(-90), 24, 18, 48, 15, 0, "reimuBulletMain");
			bulletNew(-1, 8, -24, rad(-90), 24, 18, 48, 15, 0, "reimuBulletMain");
			wait(3);
		} else {
			wait(1);
		}
	}
}

async ShootOption() {
	loop{
		if (SHOOT) {
			switch (I0) {
			case 0:
				break;
			case 1:
				bulletNew(0, -4, 0, rad(-90), 24, 8, 48, 7, 0, "reimuBullet1A");
				bulletNew(0,  4, 0, rad(-90), 24, 8, 48, 7, 0, "reimuBullet1A");
				break;
			case 2:
				bulletNew(0, -4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				bulletNew(0,  4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				bulletNew(1, -4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				bulletNew(1,  4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				break;
			case 3:
				bulletNew(0, -4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				bulletNew(0,  4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				bulletNew(1, -4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				bulletNew(1,  4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				bulletNew(2, -4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				bulletNew(2,  4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				break;
			case 4:
				bulletNew(0, -4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				bulletNew(0,  4, 0, rad(-90), 24, 8, 48, 4, 0, "reimuBullet1A");
				bulletNew(1, -4, 0, rad(-90), 24, 8, 48, 4, 0, "reimuBullet1A");
				bulletNew(1,  4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				bulletNew(2, -4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				bulletNew(2,  4, 0, rad(-90), 24, 8, 48, 4, 0, "reimuBullet1A");
				bulletNew(3, -4, 0, rad(-90), 24, 8, 48, 4, 0, "reimuBullet1A");
				bulletNew(3,  4, 0, rad(-90), 24, 8, 48, 5, 0, "reimuBullet1A");
				break;
			}
			wait(3);
		} else {
			wait(1);
		}
	}
}

void Bomb() {
	sfx(SE_BOMB2);
	dvmPlay("reimuBomb1", 0, 0, 0);
	bombAutoCollect(200);

	const int dmg = 10;
	float x = POS_X, y = POS_Y;
	loop (60) {
		bombRect(x, y, 0, 1024, 128, 0, 1);
		bombRect(x, y, 0, 128, 1024, 0, 1);
		wait(1);
	}
	for (int i = 0; i < 180; i++) {
		float s = 512;
		if (i < 120)
			s = modeLerp(128, 512, i / 120.0, EASEOUT1);
		bombRect(x, y, 0, 1024, s, dmg, 1);
		bombRect(x, y, 0, s, 1024, dmg, 1);
		wait(1);
	}
	wait(20);
	bombEnd();
}

void main() {
	hitbox(1);
	grazebox(23);
	speed(4.5, 2);

	dvmFile("dan_reimu");
	dvmMain("reimu");

	onBomb("Bomb");

	ShootMain();
	ShootOption();

	I0 = 0; // curOptions
	float r = 0;
	float dst = 0;
	float x2 = POS_X, y2 = POS_Y;
	loop {
		int newOptions = DEATH ? 0 : (POWER > 400 ? 4 : POWER / 100);
		if (newOptions != I0) {
			for (int i = 0; i < I0; i++) {
				optionDelete(i);
			}
			for (int i = 0; i < newOptions; i++) {
				optionNew(i, POS_X, POS_Y, "reimuOrbP");
			}
			I0 = newOptions;
			dst = 0;
		}

		if (newOptions) {
			float r2 = r;
			float rd = (2 * PI) / newOptions;
			for (int i = 0; i < newOptions; i++) {
				float x, y;
				circlePos(x, y, r2, dst);
				x += x2;
				y += y2;
				optionSetPos(i, x, y);
				r2 += rd;
			}

			r = normalizeRot(r + rad(5));
			dst = lerp(dst, FOCUS ? 24 : 48, 0.2f);
		}
		x2 = lerp(x2, POS_X, 0.3f);
		y2 = lerp(y2, POS_Y, 0.3f);
		wait(1);
	}
}
