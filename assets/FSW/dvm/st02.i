import "dvm";

inline void color2Dmg() { color2Hex(0x8080FF); }

void st2Boss(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ENEMY_2);
		flagSet(FLAG_ROUNDED_POS);
		texture(0, "tex/char/St02sprite.png");
		color2Dmg();
	idle:
		loop {
			srcRect(  0, 0, 64, 64); wait(8);
			srcRect( 64, 0, 64, 64); wait(8);
			srcRect(128, 0, 64, 64); wait(8);
			srcRect(192, 0, 64, 64); wait(8);
		}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		flagSet(FLAG_FLIP_SRC_X);
	case 4:
		noAsync();
		srcRect(  0, 64, 64, 64); wait(4);
		srcRect( 64, 64, 64, 64); wait(4);
		srcRect(128, 64, 64, 64); wait(4);
		srcRect(192, 64, 64, 64);
		loop wait(1000);
	case 3:
	case 5:
		noAsync();
		srcRect(128, 64, 64, 64); wait(4);
		srcRect( 64, 64, 64, 64); wait(4);
		srcRect(  0, 64, 64, 64); wait(4);
		flagClear(FLAG_FLIP_SRC_X);
		goto idle;
	case 6:
		noAsync();
		srcRect(  0, 128, 64, 64); wait(4);
		srcRect( 64, 128, 64, 64); wait(4);
		srcRect(128, 128, 64, 64); wait(4);
		srcRect(192, 128, 64, 64);
		stop();
	case 7:
		noAsync();
		srcRect(128, 128, 64, 64); wait(4);
		srcRect( 64, 128, 64, 64); wait(4);
		srcRect(  0, 128, 64, 64); wait(4);
		goto idle;
	case 8:
		scaleTime(0, 20, EASEIN1, 0, 3, 1);
		break;
	case 9:
		scaleTime(0, 20, EASEOUT1, 1, 1, 1);
		break;
	}

}

void st2CardASub(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_OV_2);
		shader(SHADER_3D);
		blendMode(BLEND_MULTIPLY);
		texture(0, "tex/card/St02a.png");
		srcRect(0, 0, 384, 448);
		alpha(0);
		alphaTime(0, 60, LINEAR, 0.8);
		textureScroll(0, 0.001, 0.001);
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}

void st2CardA(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_OV_1);
		shader(SHADER_3D);
		texture(0, "tex/card/St02b.png");
		srcRect(0, 0, 256, 256);
		scale(384 / 256.0, 448 / 256.0, 1);
		pos(0, 224, 0);
		alpha(0);
		alphaTime(0, 60, LINEAR, 1);

		childNew("st2CardASub", 0, 0, 0);
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}

void st2BgWaterA(int event) {
	const float scl = 32;
	switch (event) {
	case EVENT_CREATE:
		mode(MODE_PLANE);
		scale(scl, scl, 1);

		flagSet(FLAG_SHADER_ARGS);
		shaderCustom("shaders/std_vs.cso", "shaders/water_ps.cso");
		texture(0, "tex/bg/St02d.png");
		srcRect(0, 0, scl * 16, scl * 16);
		texture(1, "tex/bg/St02b.png");
		F0 = 0.1; // Distortion amount

		textureScroll(0, 0, 0.0005);
		textureScale(1, 0.4876, 0.4876);
		textureScroll(1, 0.002, 0.0031);
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}
void st2BgWaterB(int event) {
	const float scl = 32;
	switch (event) {
	case EVENT_CREATE:
		mode(MODE_PLANE);
		scale(scl, scl, 1);

		flagSet(FLAG_SHADER_ARGS);
		shaderCustom("shaders/std_vs.cso", "shaders/water_ps.cso");
		texture(0, "tex/bg/St02a.png");
		srcRect(0, 0, scl * 32, scl * 32);
		texture(1, "tex/bg/St02b.png");
		F0 = 0.1; // Distortion amount

		textureScroll(0, 0.0002, 0);
		textureScale(1, 0.8876, 0.8876);
		textureScroll(1, -0.006, 0.0021);
		colorHex(0x586c85);
		alpha(0.6);
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}
void st2BgWaterC(int event) {
	const float scl = 32;
	switch (event) {
	case EVENT_CREATE:
		mode(MODE_PLANE);
		blendMode(BLEND_SUBTRACT);
		scale(scl, scl, 1);

		flagSet(FLAG_SHADER_ARGS);
		shaderCustom("shaders/std_vs.cso", "shaders/water_ps.cso");
		texture(0, "tex/bg/St02a.png");
		srcRect(0, 0, scl * 32, scl * 32);
		texture(1, "tex/bg/St02b.png");
		F0 = 0.4; // Distortion amount

		textureScroll(0, 0.001, 0.003);
		textureScale(1, 0.8876, 0.8876);
		textureScroll(1, 0.003, 0.0011);
		colorHex(0x32733e);
		alpha(0.3);
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}

void st2BgWater(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_MAIN);
		mode(MODE_NONE);
		childNew("st2BgWaterA", 0, 0, 0);
		childNew("st2BgWaterB", 0, 0, 0);
		childNew("st2BgWaterC", 0, 0, 0);
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}

void st2BgLeaves(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_1);
		mode(MODE_MODEL);
		model("St2Leaves");
		shader(SHADER_3D_CLIP);
		texture(0, "tex/bg/St02c.png");
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}

void st2BgCloud1(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_2);
		mode(MODE_BILLBOARD);
		shader(SHADER_3D);
		flagSet(FLAG_NO_Z_BUFFER_WRITE);
		texture(0, "tex/bg/St02e.png");
		srcRect(0, 0, 256, 256);
		scale(6, 6, 6);
		alpha(0.75);
		color(0.4, 0.4, 0.5);
		loop {
			posTime(0, 600, SMOOTHSTEP, 2, 0, 0); wait(600);
			posTime(0, 600, SMOOTHSTEP, -2, 0, 0); wait(600);
		}
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}

void st2BgCloud2(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_2);
		mode(MODE_BILLBOARD);
		shader(SHADER_3D);
		flagSet(FLAG_NO_Z_BUFFER_WRITE);
		texture(0, "tex/bg/St02e.png");
		srcRect(0, 0, 256, 256);
		scale(6, 6, 6);
		alpha(0.75);
		color(0.4, 0.4, 0.5);
		loop{
			posTime(0, 600, SMOOTHSTEP, -2, 0, 0); wait(600);
			posTime(0, 600, SMOOTHSTEP, 2, 0, 0); wait(600);
		}
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}