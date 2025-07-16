import "dvm";

void rin(int event) {
	const float w = 32;
	const float h = 48;
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_3);
		flagSet(FLAG_ROUNDED_POS);
		color(1, 1, 1);
		color2(0.5, 0.5, 0.5);
		texture(0, "tex/dan/RinSatsuki.png");
		scale(0, 3, 1);
		scaleTime(0, 10, EASEOUT1, 1, 1, 1);
		alpha(0);
		alphaTime(1, 10, LINEAR, 1);
	straight:
		loop {
			srcRect(1, 1, 30, 46);
			wait(5);
			srcRect(33, 1, 30, 46);
			wait(5);
			srcRect(65, 1, 30, 46);
			wait(5);
			srcRect(97, 1, 30, 46);
			wait(5);
			srcRect(129, 1, 30, 46);
			wait(5);
			srcRect(161, 1, 30, 46);
			wait(5);
			srcRect(193, 1, 30, 46);
			wait(5);
			srcRect(225, 1, 30, 46);
			wait(5);
		}
	case EVENT_DELETE:
		scaleTime(0, 10, EASEIN1, 0, 3, 0);
		alphaTime(1, 10, LINEAR, 0);
		wait(10);
		setDelete();
	case 2: /* Start left */
		noAsync();
		srcRect(1, 49, 30, 46);
		wait(1);
		srcRect(33, 49, 30, 46);
		wait(1);
		srcRect(65, 49, 30, 46);
		wait(1);
		srcRect(97, 49, 30, 46);
		wait(1);
		loop {
			srcRect(129, 49, 30, 46);
			wait(5);
			srcRect(161, 49, 30, 46);
			wait(5);
			srcRect(193, 49, 30, 46);
			wait(5);
			srcRect(225, 49, 30, 46);
			wait(5);
		}
	case 3: /* End left */
		noAsync();
		srcRect(97, 49, 30, 46);
		wait(1);
		srcRect(65, 49, 30, 46);
		wait(1);
		srcRect(33, 49, 30, 46);
		wait(1);
		srcRect(1, 49, 30, 46);
		wait(1);
		goto straight;
	case 4: /* Start right */
		noAsync();
		srcRect(1, 97, 30, 46);
		wait(1);
		srcRect(33, 97, 30, 46);
		wait(1);
		srcRect(65, 97, 30, 46);
		wait(1);
		srcRect(97, 97, 30, 46);
		wait(1);
		loop{
			srcRect(129, 97, 30, 46);
			wait(5);
			srcRect(161, 97, 30, 46);
			wait(5);
			srcRect(193, 97, 30, 46);
			wait(5);
			srcRect(225, 97, 30, 46);
			wait(5);
		}
	case 5: /* End right */
		noAsync();
		srcRect(97, 97, 30, 46);
		wait(1);
		srcRect(65, 97, 30, 46);
		wait(1);
		srcRect(33, 97, 30, 46);
		wait(1);
		srcRect(1, 97, 30, 46);
		wait(1);
		goto straight;
	}
}

void rinOptionBase(int event, int srcX) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_4);
		texture(0, "tex/dan/RinSatsuki.png");
		srcRect(srcX, 144, 16, 16);
		blendMode(BLEND_ADD);

		scale(0, 0, 0);
		scaleTime(0, 10, EASEOUT1, 1.4, 1.4, 1.4);
		wait(10);
		scaleTime(0, 6, EASEIN1, 1, 1, 1);
		wait(6);

		blendMode(BLEND_ALPHA);
		loop {
			rotateTime(1, 30, SMOOTHSTEP, 0, 0, rad(-15));
			wait(30);
			rotateTime(1, 30, SMOOTHSTEP, 0, 0, rad(15));
			wait(30);
		}
	case EVENT_DELETE:
		scaleTime(0, 10, EASEIN1, 2, 2, 1);
		alphaTime(1, 10, LINEAR, 0);
		wait(10);
		setDelete();
	}
}
void rinOptionR(int event) { rinOptionBase(event, 0); }
void rinOptionB(int event) { rinOptionBase(event, 16); }
void rinOptionG(int event) { rinOptionBase(event, 32); }
void rinOptionP(int event) { rinOptionBase(event, 48); }

void rinBulletMain(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_2);
		FLAGS |= FLAG_TF_ROTATION;
		texture(0, "tex/dan/RinSatsuki.png");
		pos(-24, 0, 0);
		alpha(0);
		alphaTime(0, 4, LINEAR, 0.6);

		loop {
			srcRect(64, 145, 64, 14);
			wait(1);
			srcRect(128, 145, 64, 14);
			wait(1);
		}
	case EVENT_DELETE:
		noAsync();
		pos(0, 0, 0);
		anchor(0, 1);
		posTime(0, 30, LINEAR, 64, 0, 0);
		alpha(0.75);
		alphaTime(1, 30, LINEAR, 0);
		layer(LAYER_PLAYER_1);
		blendMode(BLEND_ADD);
		srcRect(192, 145, 32, 14);
		wait(10);
		srcRect(224, 145, 16, 14);
		wait(10);
		srcRect(240, 145, 16, 14);
		wait(10);
		setDelete();
	}
}

void rinBulletFlower(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_2);
		FLAGS |= FLAG_TF_ROTATION;
		texture(0, "tex/dan/RinSatsuki.png");
		srcRect(0, 160, 16, 16);
		scale(0.8, 0.8, 1);
		alpha(0);
		alphaTime(0, 4, LINEAR, 0.8);
		wvel(0, 0, rad(12));
		stop();
	case EVENT_DELETE:
		noAsync();
		posTime(0, 30, LINEAR, 64, 0, 0);
		//alpha(0.75);
		alphaTime(1, 30, LINEAR, 0);
		layer(LAYER_PLAYER_1);
		blendMode(BLEND_ADD);
		srcRect(16, 160, 16, 16);
		wait(10);
		srcRect(32, 160, 16, 16);
		wait(10);
		srcRect(48, 160, 16, 16);
		wait(10);
		setDelete();
	}
}

void rinBulletWind(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_3);
		FLAGS |= FLAG_TF_ROTATION;
		texture(0, "tex/dan/RinSatsuki.png");
		srcRect(64, 161, 32, 14);
		pos(-8, 0, 0);
		alpha(0);
		alphaTime(0, 4, LINEAR, 0.6);
		stop();
	case EVENT_DELETE:
		noAsync();
		pos(0, 0, 0);
		anchor(0, 1);
		posTime(0, 30, LINEAR, 64, 0, 0);
		alpha(0.75);
		alphaTime(1, 30, LINEAR, 0);
		layer(LAYER_PLAYER_1);
		blendMode(BLEND_ADD);
		srcRect(160, 161, 16, 14);
		wait(10);
		srcRect(176, 161, 16, 14);
		wait(10);
		srcRect(192, 161, 16, 14);
		wait(10);
		setDelete();
	}
}

void rinBombASub(int event) {
	switch (event) {
	case EVENT_CREATE:
		blendMode(BLEND_ADD);
		texture(0, "tex/dan/RinSatsuki.png");
		srcRect(0, 192, 64, 64);
		scale(1.5, 1.5, 1.5);
		wvel(0, 0, F0);
		wait(300);
	case EVENT_DELETE:
		wait(10);
		setDelete();
	}
}

void rinBombA(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_1);
		mode(MODE_ELLIPSE);
		points(32);
		blendMode(BLEND_SCREEN);
		texture(0, "tex/White.png");
		color(1, 1, 1);
		srcRect(0, 0, 64, 64);

		F0 = rad(3);
		childNew("rinBombASub", 0, 0, 0);
		//F0 = rad(-3);
		//childNew("rinBombASub", 0, 0, 0);

		scale(0, 0, 0);
		scaleTime(0, 10, EASEOUT1, 1.5, 1.5, 1.5);
		wait(10);
		scaleTime(0, 10, EASEIN1, 1.0, 1.0, 1.0);
		wait(10);
		scaleTime(0, 340, LINEAR, 2, 2, 2);
		wait(280);
	case EVENT_DELETE:
		scaleTime(0, 10, LINEAR, 4, 4, 4);
		alphaTime(1, 10, LINEAR, 0);
		wait(10);
		setDelete();
	}
}

void rinBombABig(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_1);
		mode(MODE_ELLIPSE);
		points(32);
		blendMode(BLEND_SCREEN);
		texture(0, "tex/White.png");
		color(1, 1, 1);
		srcRect(0, 0, 64, 64);

		F0 = rad(3);
		childNew("rinBombASub", 0, 0, 0);

		scale(0, 0, 0);
		scaleTime(0, 10, EASEOUT1, 2.5, 2.5, 2.5);
		wait(10);
		scaleTime(0, 10, EASEIN1, 2.0, 2.0, 2.0);
		wait(10);
		scaleTime(0, 340, LINEAR, 4, 4, 4);
		wait(280);
	case EVENT_DELETE:
		scaleTime(0, 10, LINEAR, 8, 8, 8);
		alphaTime(1, 10, LINEAR, 0);
		wait(10);
		setDelete();
	}
}

void rinBombB(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_5);
		blendMode(BLEND_ADD);
		texture(0, "tex/dan/RinSatsuki.png");
		srcRect(64, 192, 64, 64);
		colorMode(COLOR_UD);
		color(0, 0, 1);
		color2(0, 0.8, 1);

		wvel(0, 0, rad(8));
		scale(0, 0, 1);
		scaleTime(0, 20, EASEOUT1, 1.5, 1.5, 1);
		alpha(0);
		alphaTime(1, 30, LINEAR, 1);
		wait(20);
		scaleTime(0, 10, EASEIN1, 1.2, 1.2, 1);
		wait(10);

		wvel(0, 0, rad(1));
		scaleTime(0, 600, LINEAR, 0.5, 0.5, 5);
		colorTime(1, 600, LINEAR, 1, 0, 0);
		color2Time(2, 600, LINEAR, 1, 0, 0);
		wait(600);
	case EVENT_DELETE:
		noAsync();
		scaleTime(0, 10, LINEAR, 0, 0, 1);
		wait(10);
		setDelete();
	case 2:
		noAsync();
		scaleTime(0, 10, EASEIN1, 2, 2, 1);
		wait(10);
		setDelete();
	}
}