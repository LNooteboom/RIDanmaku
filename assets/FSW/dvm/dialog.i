import "dvm";

const int EVENT_FACE = 2;
const int EVENT_SPEAKER = 3;
const int EVENT_NOSPEAKER = 4;

inline void playerInit() {
	layer(LAYER_DIALOG_1);
	anchor(1, 1);
	pos(-224, 144, 0);
	//scale(0.25, 0.25, 1);
	//srcRect(0, 0, 1024, 1280);
	srcRect(1, 1, 254, 318);
	alpha(0);
	alphaTime(0, 10, LINEAR, 1);
}

inline void playerDelete() {
	alphaTime(0, 10, LINEAR, 0);
	wait(10);
	setDelete();
}

inline void playerSpeaker() {
	colorTime(1, 20, LINEAR, 1, 1, 1);
	posTime(2, 20, EASEOUT1, -224, 144, 0);
}

inline void playerNoSpeaker() {
	colorTime(1, 20, LINEAR, 0.3, 0.3, 0.3);
	posTime(2, 20, EASEOUT1, -256, 152, 0);
}

inline void bossInit() {
	layer(LAYER_DIALOG_1);
	anchor(1, 1);
	pos(-32, 144, 0);
	scale(0.25, 0.25, 1);
	srcRect(1, 1, 1022, 1278);
	alpha(0);
	alphaTime(0, 10, LINEAR, 1);
}

inline void bossDelete() {
	alphaTime(0, 10, LINEAR, 0);
	wait(10);
	setDelete();
}

inline void bossSpeaker() {
	colorTime(1, 20, LINEAR, 1, 1, 1);
	posTime(2, 20, EASEOUT1, -32, 144, 0);
}

inline void bossNoSpeaker() {
	colorTime(1, 20, LINEAR, 0.3, 0.3, 0.3);
	posTime(2, 20, EASEOUT1, 0, 152, 0);
}

void dialogPl00(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/char/Pl00no.png");
		playerInit();
		stop();
	case EVENT_DELETE:
		playerDelete();
	case EVENT_FACE:
		break;
	case EVENT_SPEAKER:
		playerSpeaker();
		break;
	case EVENT_NOSPEAKER:
		playerNoSpeaker();
		break;
	}
}


void dialogSt02Bs(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/char/St02no.png");
		bossInit();
		stop();
	case EVENT_DELETE:
		bossDelete();
	case EVENT_FACE:
		switch (I0) {
		case 0: texture(0, "tex/char/St02no.png"); break;
		case 1: texture(0, "tex/char/St02an.png"); break;
		case 2: texture(0, "tex/char/St02pr.png"); break;
		case 3: texture(0, "tex/char/St02lo.png"); break;
		}
		break;
	case EVENT_SPEAKER:
		bossSpeaker();
		break;
	case EVENT_NOSPEAKER:
		bossNoSpeaker();
		break;
	}
}


void dialogText(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_DIALOG_2);
		FLAGS |= FLAG_NO_CHILD_EVENT;
		pos(-176, 352, 0);
		anchor(1, 1);
		texture(0, "tex/White.png");
		srcRect(0, 0, 352, 48);
		colorMode(COLOR_LR);
		colorHex(0);
		color2Hex(0);
		alpha(0);
		alpha2(0);
		alphaTime(0, 10, LINEAR, 0.7);
		F0 = 2;
		stop();
	case EVENT_DELETE:
		childEvent(1);
		alphaTime(0, 10, LINEAR, 0);
		wait(10);
		setDelete();
	case 2:
		childNew("dialogTextLine", 8, F0 + I0, 0);
		childNew("dialogTextLine2", 8 + 1, F0 + I0 + 1, 0);
		F0 += 20;
		break;
	case 3:
		childEvent(3);
		F0 = 2;
		break;
	}
}

void dialogTextLine(int event) {
	int w, h;
	switch (event) {
	case EVENT_CREATE:
		textureTTF(0, 48, 4 << TTF_OUTLINE_SHIFT, STR0);
		getTextureSize(0, w, h);
		srcRect(0, 0, w, h);
		anchor(1, 1);
		scale(0.25, 0, 1);
		scaleTime(0, 10, EASEOUT1, 0.25, 0.25, 1);
		color(0, 0, 0);
		stop();
	case EVENT_DELETE:
	case 3:
		setDelete();
	}
}

void dialogTextLine2(int event) {
	int w, h;
	switch (event) {
	case EVENT_CREATE:
		textureTTF(0, 48, 0, STR0);
		getTextureSize(0, w, h);
		srcRect(0, 0, w, h);
		anchor(1, 1);
		scale(0.25, 0, 1);
		scaleTime(0, 10, EASEOUT1, 0.25, 0.25, 1);
		stop();
	case EVENT_DELETE:
	case 3:
		setDelete();
	}
}