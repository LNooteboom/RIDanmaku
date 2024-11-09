import "dvm";
import "ascii";

const int UI_X = 112;

inline void scaleUi() {
	scale(1/3.0, 1/3.0, 1);
}

void uiNumber(int event) {
	switch(event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Danui.png");
		srcRect(256 + (I0 / I1) % 10 * 64, 1441, 64, 78);
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		getParentVars(PV_I0);
		srcRect(256 + (I0 / I1) % 10 * 64, 1441, 64, 78);
		break;
	}
}
void uiHiScore(int event) {
	switch (event) {
	case EVENT_CREATE:
	{
		layer(LAYER_UI_1);
		texture(0, "tex/ui/Danui.png");
		srcRect(0, 1921, 256, 78);
		scaleUi();
		pos(UI_X + 256 / 6.0, 32, 0);

		I1 = 1;
		float x = 420;
		for (int i = 0; i < 9; i++) {
			childNew("uiNumber", x, -8, 0);
			x -= 36;
			I1 *= 10;
		}

		stop();
	}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}
void uiScore(int event) {
	switch (event) {
	case EVENT_CREATE:
	{
		layer(LAYER_UI_1);
		texture(0, "tex/ui/Danui.png");
		srcRect(0, 1441, 256, 78);
		scaleUi();
		pos(UI_X + 256/6.0, 56, 0);

		I1 = 1;
		float x = 420;
		for (int i = 0; i < 9; i++) {
			childNew("uiNumber", x, -8, 0);
			x -= 36;
			I1 *= 10;
		}

		stop();
	}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}

void uiLife(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Danui.png");
		srcRect(256 + I3*64, 1440+80, 64, 64);
		scale(0.8, 0.8, 1);
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}
void uiLives(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_UI_1);
		texture(0, "tex/ui/Danui.png");
		srcRect(0, 1441 + 80, 256, 78);
		scaleUi();
		pos(UI_X + 256 / 6.0, 128, 0);

	build_children:
	{
		float x = 132;
		for (int i = 0; i < I0; i++) {
			I3 = 8;
			childNew("uiLife", x, -9, 0);
			x += 44;
		}
		if (I2) {
			I3 = I2;
			childNew("uiLife", x, -9, 0);
		}
		stop();
	}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		childEvent(EVENT_DELETE);
		goto build_children;
	}
}

void uiNumberSmall(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Danui.png");
		srcRect(384 + (I0 / I1) % 10 * 32, 1616, 32, 32);
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		getParentVars(PV_I0);
		srcRect(384 + (I0 / I1) % 10 * 32, 1616, 32, 32);
		break;
	}
}
void uiPowerDot(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Danui.png");
		srcRect(256 + 64, 1440+152, 64, 64);

		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}
void uiPowerSlash(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Danui.png");
		srcRect(256, 1440 + 152, 64, 64);

		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}
void uiPower(int event) {
	switch (event) {
	case EVENT_CREATE:
	{
		layer(LAYER_UI_1);
		texture(0, "tex/ui/Danui.png");
		srcRect(0, 1441 + 80*3, 256, 78);
		scaleUi();
		pos(UI_X + 256 / 6.0, 128+24, 0);

		I1 = 100;
		childNew("uiNumber", 132, -8, 0);
		childNew("uiPowerDot", 160, -8, 0);
		I1 = 10;
		childNew("uiNumberSmall", 180, 0, 0);
		I1 = 1;
		childNew("uiNumberSmall", 200, 0, 0);

		childNew("uiPowerSlash", 240, -8, 0);

		I1 = 100000;
		childNew("uiNumber", 280, -8, 0);
		childNew("uiPowerDot", 280+28, -8, 0);
		I1 = 10000;
		childNew("uiNumberSmall", 280+28+20, 0, 0);
		I1 = 1000;
		childNew("uiNumberSmall", 280+28+40, 0, 0);

		stop();
	}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}

void uiGraze(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_UI_1);
		texture(0, "tex/ui/Danui.png");
		srcRect(0, 1441 + 80 * 4, 256, 78);
		scaleUi();
		pos(UI_X + 256 / 6.0, 192, 0);
		FLAGS |= FLAG_NO_CHILD_EVENT;

	build_children:
		{
			I1 = 10000000;
			float x = 132;
			for (int i = 7; i; --i) {
				if (I0 >= I1) {
					childNew("uiNumber", x, -8, 0);
					x += 36;
				}
				I1 /= 10;
			}
			childNew("uiNumber", x, -8, 0);
		}

		stop();
	case EVENT_DELETE:
		noAsync();
		childEvent(EVENT_DELETE);
		setDelete();
	case 2:
		childEvent(EVENT_DELETE);
		goto build_children;
	}
}
void uiPoints(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_UI_1);
		texture(0, "tex/ui/Danui.png");
		srcRect(0, 1441 + 80*5, 256, 78);
		scaleUi();
		pos(UI_X + 256 / 6.0, 192+24, 0);
		FLAGS |= FLAG_NO_CHILD_EVENT;

		build_children:
		{
			I1 = 10000000;
			float x = 132;
			for (int i = 7; i; --i) {
				if (I0 >= I1) {
					childNew("uiNumber", x, -8, 0);
					x += 36;
				}
				I1 /= 10;
			}
			childNew("uiNumber", x, -8, 0);
		}

		stop();
	case EVENT_DELETE:
		noAsync();
		childEvent(EVENT_DELETE);
		setDelete();
	case 2:
		childEvent(EVENT_DELETE);
		goto build_children;
	}
}

void uiDifficulty(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_UI_1);
		texture(0, "tex/ui/Danui.png");
		srcRect(1024, 1441 + I0 * 80, 256, 78);
		scaleUi();
		pos(224, 480 - 16, 0);
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		srcRect(1024, 1441 + I0 * 80, 256, 78);
		break;
	}
}

void uiBgBorderL(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_UI_BG);
		texture(0, "tex/ui/Danui2.jpg");
		srcRect(0, 0, 320, 1440);
		scale(1.1, 1.1, 1.1);
		pos(-1280 + 160, 0, 0);
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}
void uiBgBorderR(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_UI_BG);
		texture(0, "tex/ui/Danui2.jpg");
		srcRect(320, 0, 320, 1440);
		scale(1.1, 1.1, 1.1);
		pos(1280 - 160, 0, 0);
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}
void uiBackground(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_UI_BG);
		texture(0, "tex/ui/Danui.png");
		srcRect(0, 0, 1920, 1440);
		scaleUi();
		pos(0, 240, 0);

		childNew("uiBgBorderL", 0, 0, 0);
		childNew("uiBgBorderR", 0, 0, 0);
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}

const int SPELL_NAME_SIZE = 24;

void spellNameTextOutline(int event) {
	switch (event) {
	case EVENT_CREATE:
	{
		textureTTF(0, SPELL_NAME_SIZE, 4 << TTF_OUTLINE_SHIFT, STR0);
		int w = 0;
		int h = 0;
		getTextureSize(0, w, h);
		srcRect(0, 0, w, h);
		color(0, 0, 0);
		childNew("spellNameText", 0, 0, 0);
		pos(192 - w / 4, -12, 0);

		alpha(0);
		alphaTime(0, 20, LINEAR, 1);
		stop();
	}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}
void spellNameText(int event) {
	switch (event) {
	case EVENT_CREATE:
	{
		textureTTF(0, SPELL_NAME_SIZE, 0, STR0);
		int w = 0;
		int h = 0;
		getTextureSize(0, w, h);
		srcRect(0, 0, w, h);
		
		alpha(0);
		alphaTime(0, 20, LINEAR, 1);
		stop();
	}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}
void spellName(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_BOSS_UI_1);
		texture(0, "tex/ui/Danui.png");
		srcRect(1280, 1441, 576, 158);
		//scaleUi();
		pos(96, 400, 0);

		childNew("spellNameTextOutline", 0, 0, 0);

		alpha(0);
		alphaTime(0, 20, LINEAR, 1);
		scaleTime(1, 20, EASEOUT1, 1.0 / 3, 1.0 / 3, 1);
		wait(20);
		posTime(1, 60, EASEINOUT2, 96, 28, 0);

		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}

void spellPortrait(int event) {
	switch (event) {
	case EVENT_CREATE:
		stop();
		layer(LAYER_BOSS_UI_1);
		texture(0, "tex/char/reimu/no.png");
		srcRect(0, 0, 2048, 4096);
		//scale(0.125, 0.125, 1);
		//scaleUi();
		pos(0, 224, 0);
		scale(0.25, 0.25, 1);
		scaleTime(0, 30, EASEOUT1, 0.125, 0.125, 1);
		alpha(0);
		alphaTime(1, 30, LINEAR, 1);

		wait(300);
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}

void uiBossLifeBg(int event) {
	const float totalLength = 384 - 64;
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_BOSS_UI_1);
		texture(0, "tex/White.png");
		srcRect(0, 0, F1 <= 0 ? 0 : floor(F1 * totalLength) + 1, 3);
		pos(0, 1, 0);
		anchor(1, 0);
		color(0, 0, 0);
		stop();
	case EVENT_DELETE:
		setDelete();
	case 2:
		getParentVars(PV_F1);
		srcRect(0, 0, F1 <= 0 ? 0 : floor(F1 * totalLength) + 1, 3);
		break;
	}
}
void uiBossLife(int event) {
	const float totalLength = 384 - 64;
	switch (event) {
	case EVENT_CREATE:
		// F0 = start
		// F1 = length
		// I0 = color
		layer(LAYER_BOSS_UI_2);
		FLAGS |= FLAG_ALPHA_CHILDREN;

		texture(0, "tex/White.png");
		srcRect(0, 0, F1 <= 0? 0 : floor(F1 * totalLength), 2);
		anchor(1, 0);
		pos((-192 + 8) + floor(F0 * totalLength), 6, 0);
		colorHex(I0);
		alpha((I0 >> 24 & 0xFF) / 255.0f);
		childNew("uiBossLifeBg", 0, 0, 0);
		stop();
	case EVENT_DELETE:
		setDelete();
	case 2:
		pos((-192 + 8) + floor(F0 * totalLength), 6, 0);
		srcRect(0, 0, F1 <= 0 ? 0 : floor(F1 * totalLength), 2);
		colorHex(I0);
		break;
	}
}

void uiBossStar(int event) {
	const float scl = 0.18;
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_BOSS_UI_2);
		texture(0, "tex/ui/Danui.png");
		srcRect(768, 1440 + 80, 64, 64);
		pos(-192 + 12 + I0 * 10, 25, 0);
		scale(scl, scl, 1);

		colorMode(COLOR_UD);
		colorHex(0x8bff61);
		color2Hex(0x0b6e1f);
		alpha(0);
		alpha2(0);
		alphaTime(0, 10, LINEAR, 0.8);
		alpha2Time(1, 10, LINEAR, 0.8);
		stop();
	case EVENT_DELETE:
		noAsync();
		scaleTime(0, 10, EASEIN1, scl * 2, scl * 2, 1);
		alphaTime(1, 10, LINEAR, 0);
		alpha2Time(2, 10, LINEAR, 0);
		wait(10);
		setDelete();
	}
}

void uiBossTime(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_BOSS_UI_2);
		//scale(0.3, 0.3, 1);
		pos(192 - 36, 8, 0);

		I0 = 9999;
		FLAGS |= FLAG_ALPHA_CHILDREN;

		alpha(0);
		alphaTime(0, 20, LINEAR, 1);
		scaleTime(1, 20, EASEOUT1, 0.3, 0.3, 1);

	build_children:
		I1 = 1000;
		if (I0 >= I1)
			childNew("uiNumber", 0, 0, 0);
		I1 = 100;
		childNew("uiNumber", 28, 0, 0);

		childNew("uiPowerDot", 48, 0, 0);

		I1 = 10;
		childNew("uiNumberSmall", 64, 8, 0);
		I1 = 1;
		childNew("uiNumberSmall", 82, 8, 0);

		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		childEvent(EVENT_DELETE);
		goto build_children;
	}
}

void uiBossNameFg(int event) {
	switch (event) {
	case EVENT_CREATE:
		anchor(1, 1);

		{
			textureTTF(0, 24, 0, STR0);
			int w = 0;
			int h = 0;
			getTextureSize(0, w, h);
			srcRect(0, 0, w, h);
		}

		colorHex(0x00cc63);

		stop();
	case EVENT_DELETE:
		setDelete();
	}
}
void uiBossName(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_BOSS_UI_2);
		//scaleUi();
		anchor(1, 1);
		pos(-192 + 8, 8, 0);

		colorHex(0);
		FLAGS |= FLAG_ALPHA_CHILDREN;
		alpha(0);
		alphaTime(0, 20, LINEAR, 1);
		scaleTime(1, 20, EASEOUT1, 1.0 / 3, 1.0 / 3, 1);

		{
			textureTTF(0, 24, 1 << TTF_OUTLINE_SHIFT, STR0);
			int w = 0;
			int h = 0;
			getTextureSize(0, w, h);
			srcRect(0, 0, w, h);
		}

		childNew("uiBossNameFg", 0, 0, 0);

		stop();
	case EVENT_DELETE:
		setDelete();
	}
}

void uiFps(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_BOSS_UI_2);
		scale(0.25, 0.25, 1);
		pos(192 - 12, 448-8, 0);

		I0 = 9999;

	build_children:
		I1 = 10;
		if (I0 >= I1)
			childNew("uiNumber", 0, 0, 0);
		I1 = 1;
		childNew("uiNumber", 28, 0, 0);

		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		childEvent(EVENT_DELETE);
		goto build_children;
	}
}

void uiGaugeMax(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Danui.png");
		srcRect(1024, 1936+48, 256, 48);
		anchor(1, 0);
		alpha(0);
		alphaTime(0, 5, LINEAR, 1);
		stop();
	case EVENT_DELETE:
	case 3:
		noAsync();
		setDelete();
	}
}
void uiGaugeFg(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Danui.png");
		anchor(1, 0);
		alpha(0.8);
		F1 = F0;

	set_value:
		srcRect(1024, 1936, floor(F0 * 256), 48);
		stop();
	case EVENT_DELETE:
		setDelete();
	case 2:
		noAsync();
		childEvent(2);
		getParentVars(PV_F0);
		while (F1 < F0) {
			srcRect(1024, 1936, floor(F1 * 256), 48);
			F1 += 0.01f;
			wait(1);
		}
		F1 = F0;
		goto set_value;
	}
}
void uiGauge(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_HIRES_UI);
		texture(0, "tex/ui/Danui.png");
		srcRect(1024, 1936, 256, 48);
		pos(-188, 432, 0);
		scaleUi();
		anchor(1, 0);
		alpha(0.4);
		colorHex(0x404040);
		childNew("uiGaugeFg", 0, 0, 0);
		I0 = 0;

	create_max:
		if (F0 >= 1) {
			childNew("uiGaugeMax", 0, 0, 0);
			I0 = 1;
		}
		stop();
	case EVENT_DELETE:
		setDelete();
	case 2:
		if (I0) {
			if (F0 < 1) {
				childEvent(3);
				I0 = 0;
			}
		} else {
			if (F0 >= 1)
				goto create_max;
		}
		break;
	}
}

void uiGaugeText(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_HIRES_UI);
		mode(MODE_NONE);
		pos(-176, 444, 0);
		scale(0.5, 0.5, 1);

		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		noAsync();
		childEvent(EVENT_DELETE);
		F0 = 0;
		I1 = 0;
		while (I0 = strChar(I1, STR0)) {
			childNew("ascii2Char", F0, 0, 0);
			F0 += 11;
		}
		break;
	}
}

void pointValueChar(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Ascii1x.png");
		I0 -= 32;
		srcRect(I0 % 32 * 8, I0 / 32 * 8 + 128, 8, 8);
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}
void pointValue(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ITEM_TEXT);
		mode(MODE_NONE);
		flagSet(FLAG_ALPHA_CHILDREN | FLAG_COLOR_CHILDREN | FLAG_ROUNDED_POS);
		color(((I1 >> 16) & 0xFF) / 255.0, ((I1 >> 8) & 0xFF) / 255.0, (I1 & 0xFF) / 255.0);

		F0 = 0;
		I1 = I0;
		I2 = 10000000;
		I3 = 0;
		while (I2) {
			I0 = (I1 / I2) % 10;
			if (I0 || I3) {
				I0 += 48;
				childNew("pointValueChar", F0, 0, 0);
				I3 = 1;
				F0 += 7;
			}
			I2 /= 10;
		}
		
		F0 = floor(F0 * -0.5);
		pos(F0, -16, 0);
		posTime(1, 20, EASEOUT1, F0, -48 + RF * 32, 0);
		wait(10);
		alphaTime(0, 10, LINEAR, 0);
		wait(10);
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}