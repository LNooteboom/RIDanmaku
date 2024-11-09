import "dvm";

void bhaBgImg(int event) {
	const float scl = 1.1;
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_BG_1);
		mode(MODE_PLANE);
		shader(SHADER_3D);
		flagSet(FLAG_NO_Z_BUFFER_WRITE);
		texture(0, "tex/bg/bha_d.png");
		srcRect(0, 0, 256, 256);
		pos(0, 1, 0.2);
		rotate(rad(-90), 0, 0);
		scale(scl, scl, scl);
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}

void bhaBgParticleA(int event) {
	int w = 180;
	switch (event) {
	case EVENT_CREATE:
		blendMode(BLEND_ADD);
		texture(0, "tex/dan/Effects.png");
		srcRect(17, 17, 14, 14);
		alpha(0);
		scale(0.5, 0.3, 1);
		rotate(0, 0, RRAD);
		wvel(0, 0, RF2 * rad(2));
		colorHex(0xFF8000);
		posTime(0, w * 2, LINEAR, F0 + RF * 16, F1 + 192 + RF * 128, 0);
		alphaTime(1, w, LINEAR, 1);
		wait(w);
		alphaTime(1, w, LINEAR, 0);
		wait(w);
	case EVENT_DELETE:
		setDelete();
	}
}

void bhaBgParticles(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_OV_2);
		mode(MODE_NONE);
		flagSet(FLAG_NO_CHILD_EVENT);
		loop {
			F0 = RF2 * 192;
			F1 = RF * 128;
			childNew("bhaBgParticleA", F0, F1, 0);
			wait(16);
		}
	case EVENT_DELETE:
		noAsync();
		wait(180 * 2);
		setDelete();
	}
}

inline void bhaBgModel(int event, string mdl, string tex) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_MAIN);
		flagSet(FLAG_TF_3D);
		mode(MODE_MODEL);
		model(mdl);
		shader(SHADER_3D);
		texture(0, tex);
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}

void bhaBgRoad(int event) { bhaBgModel(event, "Road", "tex/bg/bha_c.png"); }

void bhaBgHouse1A(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_CLIP);
		flagSet(FLAG_TF_3D);
		mode(MODE_MODEL);
		model("House1~1");
		shader(SHADER_3D_CLIP);
		texture(0, "tex/bg/bha_b.png");
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}
void bhaBgHouse1(int event) {
	if (!event) {
		childNew("bhaBgHouse1A", 0, 0, 0);
	}
	bhaBgModel(event, "House1", "tex/bg/bha_a.png");
}

void bhaBgHouse2A(int event) { bhaBgModel(event, "House2~1", "tex/bg/bha_b.png"); }
void bhaBgHouse2(int event) {
	if (!event) {
		childNew("bhaBgHouse2A", 0, 0, 0);
	}
	bhaBgModel(event, "House2", "tex/bg/bha_a.png");
}

void bhaBgHouse3A(int event) { bhaBgModel(event, "House3~1", "tex/bg/bha_b.png"); }
void bhaBgHouse3(int event) {
	if (!event) {
		childNew("bhaBgHouse3A", 0, 0, 0);
	}
	bhaBgModel(event, "House3", "tex/bg/bha_a.png");
}

void bhaBgHouse4A(int event) { bhaBgModel(event, "House4~1", "tex/bg/bha_b.png"); }
void bhaBgHouse4(int event) {
	if (!event) {
		childNew("bhaBgHouse4A", 0, 0, 0);
	}
	bhaBgModel(event, "House4", "tex/bg/bha_a.png");
}

void bhaBgAlleyA(int event) { bhaBgModel(event, "Alley~1", "tex/bg/bha_a.png"); }
void bhaBgAlleyB(int event) { bhaBgModel(event, "Alley~2", "tex/bg/bha_c.png"); }
void bhaBgAlley(int event) {
	if (!event) {
		childNew("bhaBgAlleyA", 0, 0, 0);
		childNew("bhaBgAlleyB", 0, 0, 0);
	}
	bhaBgModel(event, "Alley", "tex/bg/bha_b.png");
}


inline void color2Dmg() { color2Hex(0x8080FF); }

void bhaBoss(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_ENEMY_2);
		flagSet(FLAG_ROUNDED_POS);
		texture(0, "tex/char/bha_sprite.png");
		color2Dmg();
	idle:
		loop{
			srcRect(0, 0, 64, 96); wait(8);
			srcRect(64, 0, 64, 96); wait(8);
			srcRect(128, 0, 64, 96); wait(8);
			srcRect(192, 0, 64, 96); wait(8);
		}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		flagSet(FLAG_FLIP_SRC_X);
	case 4:
		noAsync();
		srcRect(0, 64, 64, 64); wait(4);
		srcRect(64, 64, 64, 64); wait(4);
		srcRect(128, 64, 64, 64); wait(4);
		srcRect(192, 64, 64, 64);
		loop wait(1000);
	case 3:
	case 5:
		noAsync();
		srcRect(128, 64, 64, 64); wait(4);
		srcRect(64, 64, 64, 64); wait(4);
		srcRect(0, 64, 64, 64); wait(4);
		flagClear(FLAG_FLIP_SRC_X);
		goto idle;
	case 6:
		noAsync();
		srcRect(0, 128, 64, 64); wait(4);
		srcRect(64, 128, 64, 64); wait(4);
		srcRect(128, 128, 64, 64); wait(4);
		srcRect(192, 128, 64, 64);
		stop();
	case 7:
		noAsync();
		srcRect(128, 128, 64, 64); wait(4);
		srcRect(64, 128, 64, 64); wait(4);
		srcRect(0, 128, 64, 64); wait(4);
		goto idle;
	case 8:
		scaleTime(0, 20, EASEIN1, 0, 3, 1);
		break;
	case 9:
		scaleTime(0, 20, EASEOUT1, 1, 1, 1);
		break;
	}

}

void bhaCardPortrait(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_EFF_HIRES);
		texture(0, "tex/char/bha_card.png");
		srcRect(0, 0, 1024, 2048);
		scale(0.25, 0.25, 1);
		pos(192 + 128, 224 - 64, 0);
		posTime(1, 60, EASEOUTIN2, -192 - 128, 224 + 64, 0);
		alpha(0);
		alphaTime(0, 30, EASEOUT1, 1);
		wait(30);
	case EVENT_DELETE:
		noAsync();
		alphaTime(0, 30, EASEIN1, 0);
		wait(30);
		setDelete();
	}
}