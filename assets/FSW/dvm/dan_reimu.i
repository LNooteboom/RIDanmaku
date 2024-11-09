import "dvm";

inline void plFrames(float x, float y, float w, int n) {
	float x2 = x * 32;
	float y2 = y * 48;
	for (int i = n; i; --i) {
		srcRect(x2 + 1, y2 + 1, 30, 46);
		x2 += 32;
		wait(w);
	}
}
inline void plFramesR(float x, float y, float w, int n) {
	float x2 = (x + (n - 1)) * 32;
	float y2 = y * 48;
	for (int i = n; i; --i) {
		srcRect(x2 + 1, y2 + 1, 30, 46);
		x2 -= 32;
		wait(w);
	}
}
void reimu(int event) {
	const float w = 32;
	const float h = 48;
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_3);
		color(1, 1, 1);
		color2(0.5, 0.5, 0.5);
		texture(0, "tex/dan/Reimu.png");
		scale(0, 3, 1);
		scaleTime(0, 10, EASEOUT1, 1, 1, 1);
		alpha(0);
		alphaTime(1, 10, LINEAR, 1);
	straight:
		while (1) {
			plFrames(0, 0, 5, 8);
		}
	case EVENT_DELETE:
		scaleTime(0, 10, EASEIN1, 0, 3, 0);
		alphaTime(1, 10, LINEAR, 0);
		wait(10);
		setDelete();
	case 2: /* Start left */
		noAsync();
		plFrames(0, 1, 1, 4);
		while (1) {
			plFrames(4, 1, 5, 4);
		}
	case 3: /* End left */
		noAsync();
		plFramesR(0, 1, 1, 4);
		goto straight;
	case 4: /* Start right */
		noAsync();
		plFrames(0, 2, 1, 4);
		while (1) {
			plFrames(4, 2, 5, 4);
		}
	case 5: /* End right */
		noAsync();
		plFramesR(0, 2, 1, 4);
		goto straight;
	case 6: /* Invuln blink on */
		colorMode(COLOR2);
		break;
	case 7: /* Invuln blink off */
		colorMode(COLOR1);
		break;
	}
}

void reimuOrbBase(int event, int srcX) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_4);
		texture(0, "tex/dan/Reimu.png");
		srcRect(srcX, 144, 16, 16);
		blendMode(BLEND_ADD);
		wvel(0, 0, rad(18));

		scale(0, 0, 0);
		scaleTime(0, 10, EASEOUT1, 1.4, 1.4, 1.4);
		wait(10);
		scaleTime(0, 6, EASEIN1, 1, 1, 1);
		wait(6);

		blendMode(BLEND_ALPHA);
		wvel(0, 0, rad(6));
		stop();
	case EVENT_DELETE:
		scaleTime(0, 10, EASEIN1, 2, 2, 1);
		alphaTime(1, 10, LINEAR, 0);
		wait(10);
		setDelete();
	case 2:
		wait(16);
		rotate(0, 0, rad(18) * 16);
		break;
	}
}
void reimuOrbR(int event) {
	reimuOrbBase(event, 0);
}
void reimuOrbG(int event) {
	reimuOrbBase(event, 16);
}
void reimuOrbB(int event) {
	reimuOrbBase(event, 32);
}
void reimuOrbP(int event) {
	reimuOrbBase(event, 48);
}

void reimuBulletMain(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_2);
		FLAGS |= FLAG_TF_ROTATION;
		texture(0, "tex/dan/Reimu.png");
		srcRect(0, 161, 64, 14);
		pos(-24, 0, 0);
		alpha(0);
		wait(1);
		alphaTime(0, 4, LINEAR, 0.6);
		stop();
	case EVENT_DELETE:
		noAsync();
		posTime(0, 30, LINEAR, 64, 0, 0);
		alpha(0.75);
		alphaTime(1, 30, LINEAR, 0);
		layer(LAYER_PLAYER_1);
		blendMode(BLEND_ADD);
		srcRect(16, 176, 16, 16);
		wait(10);
		srcRect(32, 176, 16, 16);
		wait(10);
		srcRect(48, 176, 16, 16);
		wait(10);
		setDelete();
	}
}
void reimuBullet0A(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_2);
		FLAGS |= FLAG_TF_ROTATION;
		texture(0, "tex/dan/Reimu.png");
		srcRect(65, 145, 14, 14);
		alpha(0);
		alphaTime(0, 4, LINEAR, 0.8);
		stop();
	case EVENT_DELETE:
		noAsync();
		posTime(0, 30, LINEAR, 64, 0, 0);
		//alpha(0.75);
		alphaTime(1, 30, LINEAR, 0);
		layer(LAYER_PLAYER_1);
		blendMode(BLEND_ADD);
		srcRect(81, 145, 14, 14);
		wait(10);
		srcRect(97, 145, 14, 14);
		wait(10);
		srcRect(113, 145, 14, 14);
		wait(10);
		setDelete();
	}
}
void reimuBullet1A(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_2);
		FLAGS |= FLAG_TF_ROTATION;
		texture(0, "tex/dan/Reimu.png");
		srcRect(64, 160, 64, 8);
		pos(-24, 0, 0);
		alpha(0);
		wait(1);
		alphaTime(0, 4, LINEAR, 0.5);
		stop();
	case EVENT_DELETE:
		noAsync();
		posTime(0, 20, LINEAR, 64, 0, 0);
		alpha(0.75);
		alphaTime(1, 20, LINEAR, 0);
		layer(LAYER_PLAYER_1);
		blendMode(BLEND_ADD);
		wait(20);
		setDelete();
	}
}

void reimuBomb0ASub(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/dan/Reimu.png");
		srcRect(1, 193, 62, 62);
		blendMode(BLEND_ADD);

		alpha(0);
		alphaTime(1, 20, LINEAR, 1);
		color(((I0 >> 16) & 0xFF) / 255.0, ((I0 >> 8) & 0xFF) / 255.0, (I0 & 0xFF) / 255.0);
		rotate(0, 0, RRAD);
		wvel(0, 0, rad(10));
		scale(1.5, 1.5, 1);
		
		F0 = 0;
		F1 = 0;
		while (1) {
			float a = RRAD;
			float x = cos(a) * 96;
			float y = sin(a) * 96;
			posBezier(20, F0, F1, 0, 0, 0, 0, x, y, 0);
			F0 = x;
			F1 = y;
			wait(20);
		}
	case EVENT_DELETE:
		noAsync();
		alphaTime(1, 10, LINEAR, 0);
		wait(10);
		setDelete();
	}
}
void reimuBomb0A(int event) {
	switch (event) {
	case EVENT_CREATE:
		FLAGS |= FLAG_NO_CHILD_EVENT;
		layer(LAYER_PLAYER_1);
		texture(0, "tex/dan/Reimu.png");
		srcRect(1, 193, 62, 62);
		blendMode(BLEND_ADD);

		//scale(0.5, 0.5, 1);
		alpha(0);
		alphaTime(1, 20, LINEAR, 1);
		scale(0, 0, 1);
		scaleTime(2, 60, EASEOUT1, 1, 1, 1);
		rotate(0, 0, RRAD);

		I0 = 0xFF0000;
		childNew("reimuBomb0ASub", 0, 0, 0);
		wait(5);
		I0 = 0x00FF00;
		childNew("reimuBomb0ASub", 0, 0, 0);
		wait(5);
		I0 = 0x0000FF;
		childNew("reimuBomb0ASub", 0, 0, 0);
		wait(5);
		I0 = 0x00FFFF;
		childNew("reimuBomb0ASub", 0, 0, 0);
		wait(5);
		I0 = 0xFF00FF;
		childNew("reimuBomb0ASub", 0, 0, 0);

		stop();
	case EVENT_DELETE:
		noAsync();
		childEvent(EVENT_DELETE);
		alphaTime(1, 10, LINEAR, 0);
		scaleTime(2, 10, EASEIN1, 2, 2, 2048);
		wait(10);
		
		setDelete();
	}
}

void reimuBomb1Sub(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/dan/Reimu.png");
		srcRect(65, 193, 62, 62);
		blendMode(BLEND_ADD);

		alpha(0);
		alphaTime(1, 20, LINEAR, 1);
		color(((I0 >> 16) & 0xFF) / 255.0, ((I0 >> 8) & 0xFF) / 255.0, (I0 & 0xFF) / 255.0);
		//color(1, 0, 0);

		//pos(8 * F1, 8 * F2, 0);
		rotate(0, 0, F0);
		scale(2, 16, 1);

		wait(60);
		posTime(0, 120, EASEOUT1, 192 * F1, 192 * F2, 0);
		scaleTime(2, 120, EASEOUT1, 4, 16, 1);
		
		wait(180);
	case EVENT_DELETE:
		noAsync();
		alphaTime(1, 20, LINEAR, 0);
		wait(20);
		setDelete();
	}
}
void reimuBomb1(int event) {
	switch (event) {
	case EVENT_CREATE:
	{
		layer(LAYER_PLAYER_1);
		string fn = "reimuBomb1Sub";
		I0 = 0xFF2020;
		F0 = rad(90);
		F1 = 0;
		F2 = -1;
		childNew(fn, 0, 0, 0);
		F0 = rad(-90);
		F2 = 1;
		childNew(fn, 0, 0, 0);

		I0 = 0x4040FF;
		F0 = 0;
		F1 = -1;
		F2 = 0;
		childNew(fn, 0, 0, 0);
		F0 = rad(180);
		F1 = 1;
		childNew(fn, 0, 0, 0);

		wait(270);
	}
	case EVENT_DELETE:
		setDelete();
	}
}