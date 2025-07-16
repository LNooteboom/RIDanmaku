import "dvm";

const int EVENT_START_LEFT = 2;
const int EVENT_END_LEFT = 3;
const int EVENT_START_RIGHT = 4;
const int EVENT_END_RIGHT = 5;

inline void color2Dmg() { color2Hex(0x8080FF); }

void enmSmall(int event, int y) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ENEMY_5);
		flagSet(FLAG_ROUNDED_POS);
		texture(0, "tex/dan/Enemies.png");
		color2Dmg();
	straight:
		while (1) {
			srcRect(0, y, 32, 32);
			wait(5);
			srcRect(32, y, 32, 32);
			wait(5);
			srcRect(64, y, 32, 32);
			wait(5);
			srcRect(96, y, 32, 32);
			wait(5);
		}
	side:
		noAsync();
		srcRect(128, y, 32, 32);
		wait(4);
		srcRect(160, y, 32, 32);
		wait(4);
		srcRect(192, y, 32, 32);
		wait(4);
		srcRect(224, y, 32, 32);
		wait(4);
		while (1) {
			srcRect(256, y, 32, 32);
			wait(5);
			srcRect(288, y, 32, 32);
			wait(5);
			srcRect(320, y, 32, 32);
			wait(5);
			srcRect(352, y, 32, 32);
			wait(5);
		}
	side_end:
		noAsync();
		srcRect(224, y, 32, 32);
		wait(4);
		srcRect(192, y, 32, 32);
		wait(4);
		srcRect(160, y, 32, 32);
		wait(4);
		srcRect(128, y, 32, 32);
		wait(4);
		goto straight;

	case EVENT_DELETE:
		noAsync();
		setDelete();

	case EVENT_START_LEFT:
		FLAGS |= FLAG_FLIP_SRC_X;
		goto side;
	case EVENT_END_LEFT:
		FLAGS |= FLAG_FLIP_SRC_X;
		goto side_end;
	case EVENT_START_RIGHT:
		FLAGS &= ~FLAG_FLIP_SRC_X;
		goto side;
	case EVENT_END_RIGHT:
		FLAGS &= ~FLAG_FLIP_SRC_X;
		goto side_end;
	}
}
void enmBGirl(int event) {
	enmSmall(event, 0);
}
void enmRGirl(int event) {
	enmSmall(event, 32);
}
void enmYGirl(int event) {
	enmSmall(event, 64);
}
void enmGGirl(int event) {
	enmSmall(event, 96);
}

void enmMagic(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ENEMY_5);
		texture(0, "tex/dan/Enemies.png");
		srcRect(0, 192, 64, 64);

		scale(0, 0, 1);
		scaleTime(0, 40, EASEOUT1, 0.9, 0.9, 1);
		alpha(0);
		alphaTime(1, 40, LINEAR, 1);
		wait(40);
		
		while (1) {
			const float amp = 0.1;
			scaleTime(0, 60, SMOOTHSTEP, 1 + amp, 1 + amp, 1);
			wait(60);
			scaleTime(0, 60, SMOOTHSTEP, 1 - amp, 1 - amp, 1);
			wait(60);
		}
	case EVENT_DELETE:
		scaleTime(0, 20, EASEIN1, 2, 2, 1);
		alphaTime(1, 20, LINEAR, 0);
		wait(20);
		setDelete();
	}
}
void enmMagicSpin(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ENEMY_5);
		texture(0, "tex/dan/Enemies.png");
		srcRect(0, 192, 64, 64);

		wvel(0, 0, rad(10));

		scale(0, 0, 1);
		scaleTime(0, 40, EASEOUT1, 0.9, 0.9, 1);
		alpha(0);
		alphaTime(1, 40, LINEAR, 1);
		wait(40);

		wvel(0, 0, rad(4));

		while (1) {
			const float amp = 0.1;
			scaleTime(0, 60, SMOOTHSTEP, 1 + amp, 1 + amp, 1);
			wait(60);
			scaleTime(0, 60, SMOOTHSTEP, 1 - amp, 1 - amp, 1);
			wait(60);
		}
	case EVENT_DELETE:
		scaleTime(0, 20, EASEIN1, 2, 2, 1);
		alphaTime(1, 20, LINEAR, 0);
		wait(20);
		setDelete();
	}
}

void enmCir(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ENEMY_5);
		flagSet(FLAG_COLOR_CHILDREN);
		texture(0, "tex/dan/Enemies.png");
		srcRect(192 + I0 * 32, 192, 32, 32);
		color2Dmg();
		childNew("enmCirRing", 0, 0, 0);
		childNew("enmCirRing2", 0, 0, 0);
		stop();
	case EVENT_DELETE:
		setDelete();
	case 2:
		alpha(0);
		scale(0, 0, 1);

		alphaTime(0, 30, LINEAR, 1);
		scaleTime(1, 20, EASEOUT1, 1.4, 1.4, 1);
		wait(20);
		scaleTime(1, 10, SMOOTHSTEP, 1, 1, 1);
		break;
	case 3:
		noAsync();
		scaleTime(0, 20, EASEIN1, 2, 2, 1);
		alphaTime(1, 20, LINEAR, 0);
		wait(20);
		setDelete();
	}
}
void enmCirRing(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/dan/Enemies.png");
		srcRect(192 + I0 * 32, 224, 32, 32);
		wvel(0, 0, rad(12));
		stop();
	case EVENT_DELETE:
		setDelete();
	case 2:
		alpha(0);
		scale(0, 0, 1);
		rotate(0, 0, -3 * PI);

		alphaTime(0, 30, LINEAR, 1);
		rotateTime(2, 20, SMOOTHSTEP, 0, 0, 0);
		wait(20);
		wvel(0, 0, rad(12));
		break;
	}
}
void enmCirRing2(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ENEMY_6);
		texture(0, "tex/dan/Enemies.png");
		srcRect(192 + I0 * 32, 224, 32, 32);
		blendMode(BLEND_ADD);
		wvel(0, 0, rad(-5));
		scale(1.5, 1.5, 1);
		stop();
	case EVENT_DELETE:
		setDelete();
	case 2:
		alpha(0);
		scale(0, 0, 1);
		rotate(0, 0, rad(180));

		alphaTime(0, 30, LINEAR, 0.8);
		scaleTime(1, 20, EASEOUT1, 2.0, 2.0, 1);
		rotateTime(2, 20, SMOOTHSTEP, 0, 0, 0);
		wait(20);
		wvel(0, 0, rad(-5));
		scaleTime(1, 10, SMOOTHSTEP, 1.5, 1.5, 1);
		break;
	case 3:
		noAsync();
		scaleTime(0, 20, EASEIN1, 2.5, 2.5, 1);
		alphaTime(1, 20, LINEAR, 0);
		wait(20);
		setDelete();
	}
}
void enmRCir(int event) {
	if (!event) I0 = 0;
	enmCir(event);
}
void enmGCir(int event) {
	if (!event) I0 = 1;
	enmCir(event);
}
void enmBCir(int event) {
	if (!event) I0 = 2;
	enmCir(event);
}
void enmPCir(int event) {
	if (!event) I0 = 3;
	enmCir(event);
}

void enmCircleEff(int event, int color) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ENEMY_4);
		texture(0, "tex/dan/Enemies.png");
		blendMode(BLEND_ADD);
		srcRect(64 + color * 32, 224, 32, 32);
		alpha(0.5);
		wvel(0, 0, rad(18));
		scale(0, 0, 1);
		loop {
			scaleTime(0, 20, EASEINOUT1, 1.4, 1.4, 1);
			wait(20);
			scaleTime(0, 20, EASEINOUT1, 1.0, 1.0, 1);
			wait(20);
		}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}
void enmCircleEffRed(int event) {
	enmCircleEff(event, 0);
}
void enmCircleEffGreen(int event) {
	enmCircleEff(event, 1);
}
void enmCircleEffBlue(int event) {
	enmCircleEff(event, 2);
}
void enmCircleEffPurple(int event) {
	enmCircleEff(event, 3);
}


void enmSFGirl(int event, int y) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ENEMY_3);
		flagSet(FLAG_ROUNDED_POS);
		texture(0, "tex/dan/Enemies.png");
		color2Dmg();
	straight:
		while (1) {
			srcRect(0, 128, 64, 64);
			wait(5);
			srcRect(64, 128, 64, 64);
			wait(5);
			srcRect(128, 128, 64, 64);
			wait(5);
			srcRect(192, 128, 64, 64);
			wait(5);
		}
	side:
		noAsync();
		srcRect(256, 128, 64, 64);
		wait(4);
		srcRect(320, 128, 64, 64);
		wait(4);
		srcRect(384, 128, 64, 64);
		wait(4);
		srcRect(448, 128, 64, 64);
		wait(4);
		while (1) {
			srcRect(384, 0, 64, 64);
			wait(5);
			srcRect(448, 0, 64, 64);
			wait(5);
			srcRect(384, 64, 64, 64);
			wait(5);
			srcRect(448, 64, 64, 64);
			wait(5);
		}
	side_end:
		noAsync();
		srcRect(448, 128, 64, 64);
		wait(4);
		srcRect(384, 128, 64, 64);
		wait(4);
		srcRect(320, 128, 64, 64);
		wait(4);
		srcRect(256, 128, 64, 64);
		wait(4);
		goto straight;
	case EVENT_DELETE:
		noAsync();
		setDelete();

	case EVENT_START_LEFT:
		FLAGS |= FLAG_FLIP_SRC_X;
		goto side;
	case EVENT_END_LEFT:
		FLAGS |= FLAG_FLIP_SRC_X;
		goto side_end;
	case EVENT_START_RIGHT:
		FLAGS &= ~FLAG_FLIP_SRC_X;
		goto side;
	case EVENT_END_RIGHT:
		FLAGS &= ~FLAG_FLIP_SRC_X;
		goto side_end;
	}
}

void enmSpiritParticle(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ENEMY_6);
		blendMode(BLEND_ADD);
		texture(0, "tex/dan/Enemies.png");
		srcRect((RI & 7) * 32, I1, 32, 32);

		posTime(0, 10, EASEOUT1, RF2 * 8, RF * -16 - 16, 0);
		scale(0.8, 0.8, 0.8);
		scaleTime(1, 10, LINEAR, 0, 0, 0);
		colorTime(2, 10, EASEOUT2,
			((I0 >> 16) & 0xFF) / 255.0, ((I0 >> 8) & 0xFF) / 255.0, (I0 & 0xFF) / 255.0);

		wait(10);
	case EVENT_DELETE:
		setDelete();
	}
}

void enmSpirit(int event) {
	switch (event) {
	case EVENT_CREATE:
		mode(MODE_NONE);
		layer(LAYER_ENEMY_6);
		blendMode(BLEND_ADD);
		texture(0, "tex/dan/Enemies.png");

		scale(0, 0, 0);
		scaleTime(0, 6, EASEOUT1, 2, 2, 2);
		srcRect(0, I1, 32, 32);
		wait(3);
		srcRect(32, I1, 32, 32);
		wait(3);
		scaleTime(0, 6, EASEIN1, 1, 1, 1);
		srcRect(64, I1, 32, 32);
		wait(3);
		srcRect(96, I1, 32, 32);
		wait(3);

		I2 = 0;
		loop {
			srcRect(I2, I1, 32, 32);
			I2 = (I2 + 32) & 0xFF;
			childNewRoot("enmSpiritParticle", RF2 * 8, RF * 8 - 4, 0);
			wait(1);
			childNewRoot("enmSpiritParticle", RF2 * 8, RF * 8 - 4, 0);
			wait(1);
			childNewRoot("enmSpiritParticle", RF2 * 8, RF * 8 - 4, 0);
			wait(1);
		}
	case EVENT_DELETE:
		setDelete();
	}
}

void enmRSpirit(int event) {
	I0 = 0xFF0000;
	I1 = 256;
	enmSpirit(event);
}
void enmGSpirit(int event) {
	I0 = 0x00FF00;
	I1 = 288;
	enmSpirit(event);
}
void enmBSpirit(int event) {
	I0 = 0x0000FF;
	I1 = 320;
	enmSpirit(event);
}
void enmYSpirit(int event) {
	I0 = 0xFFFF00;
	I1 = 352;
	enmSpirit(event);
}

void enmBook(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ENEMY_5);
		flagSet(FLAG_ROUNDED_POS);
		texture(0, "tex/dan/Enemies.png");
		srcRect(I0, I1, 32, 32);
		scale(0, 0, 0);
		scaleTime(0, 6, EASEOUT1, 1.5, 1.5, 1.5);
		wait(6);
		scaleTime(0, 6, EASEIN1, 1, 1, 1);
		wait(6);

		loop{
			posTime(1, 60, SMOOTHSTEP, 0, -2, 0);
			wait(60);
			posTime(1, 60, SMOOTHSTEP, 0, 2, 0);
			wait(60);
		}
	case EVENT_DELETE:
		setDelete();
	}
}

void enmRBook(int event) {
	I0 = 256;
	I1 = 256;
	enmBook(event);
}
void enmGBook(int event) {
	I0 = 288;
	I1 = 256;
	enmBook(event);
}
void enmBBook(int event) {
	I0 = 256;
	I1 = 288;
	enmBook(event);
}
void enmYBook(int event) {
	I0 = 288;
	I1 = 288;
	enmBook(event);
}