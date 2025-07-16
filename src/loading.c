#include "redefined.h"

enum LoadingType loadingType;
static enum LoadingType curLoadingType;
static int loadingActive;
static int loadPrefade;
static int loadPrefadeTotal;

static struct Texture *loadingTexture[3];
static int loadCounter;

struct LoadParticle {
	int counter;
	float size;
	float x, y, r;
	float spd, dir;
};
#define N_LOADPARTS 32
#define LOADPART_DURATION 30
#define LOADPART_FADE 10
#define LOADICON_X 256
#define LOADICON_Y 416
static struct LoadParticle loadParticles[N_LOADPARTS];

static void loadingStart(void *arg, int prefade) {
	(void)arg;
	loadingActive = -1;
	curLoadingType = loadingType;
	loadCounter = 0;
	for (int i = 0; i < N_LOADPARTS; i++) {
		loadParticles[i].counter = 0;
	}
	loadPrefade = prefade;
	loadPrefadeTotal = prefade;
}
static void loadingEnd(void *arg) {
	(void)arg;
	if (!loadingActive)
		return;
	loadingActive = 10;
}
static bool loadingUpdate(void *arg) {
	(void)arg;
	if (curLoadingType == LOAD_NONE)
		return false;
	int create = 1;
	for (int i = 0; i < N_LOADPARTS; i++) {
		if (!loadParticles[i].counter) {
			if (loadCounter % 2 == 0 && create) {
				create--;
				loadParticles[i].counter = 1;
				float x = randomFloat(-1, 1);
				loadParticles[i].size = randomFloat(0.5f, 1) + x * 0.3f;
				loadParticles[i].x = LOADICON_X + x * 160;
				loadParticles[i].y = randomFloat(LOADICON_Y - 64, LOADICON_Y);
				loadParticles[i].r = randomFloat(DEG2RAD(-10), DEG2RAD(10));
				loadParticles[i].dir = randomFloat(DEG2RAD(90), DEG2RAD(135));
				loadParticles[i].spd = randomFloat(2.0f, 3.0f);
			}
		} else {
			loadParticles[i].counter++;
			if (loadParticles[i].counter == LOADPART_DURATION) {
				loadParticles[i].counter = 0;
			}
		}
	}
	loadCounter++;
	return true;
}

static void loadingNormUpdate(void *arg) {
	(void) arg;
	if (loadingActive > 0) {
		loadingActive--;
		loadingUpdate(NULL);
	}
	if (loadPrefade > 0) {
		loadPrefade--;
		loadingUpdate(NULL);
	}
}

static void loadingDrawIcon(float alpha) {
	float a;
	drawTexture(0, loadingTexture[1]);

	/* Particles */
	drawSrcRect(0, 224, 128, 128);
	for (int i = 0; i < N_LOADPARTS; i++) {
		struct LoadParticle *p = &loadParticles[i];
		if (!p->counter)
			continue;
		a = 1.0f;
		if (p->counter < LOADPART_FADE) {
			a = p->counter / (float)LOADPART_FADE;
		} else if (p->counter >= LOADPART_DURATION - LOADPART_FADE) {
			a = 1.0f - (p->counter - (LOADPART_DURATION - LOADPART_FADE)) / (float)LOADPART_FADE;
		}
		if (p->size > 0.7f) {
			a *= 1 - (p->size - 0.7f);
		}

		drawMatIdentity();
		drawTranslate(p->x + cosf(p->dir) * p->spd * p->counter,
			p->y + sinf(p->dir) * p->spd * p->counter);
		drawScale(p->size, p->size);
		drawRotate(p->r * p->counter);
		drawColor(1, 1, 1, a * alpha);
		drawRect(32, 32);
	}

	/* JP Icon */
	drawMatIdentity();
	drawTranslate(LOADICON_X, LOADICON_Y);
	drawSrcRect(0, 0, 512, 128);
	int tmax = 20;
	int t = loadCounter % (tmax * 2);
	if (t < tmax) {
		a = t / (float)tmax;
	} else {
		a = 1.0f - (t - tmax) / (float)tmax;
	}
	a = lerp(0.2f, 1, a);
	drawColor(1, 1, 1, a * alpha);
	drawRect(170, 48);

	/* EN Icon */
	drawTranslate(32, 24);
	drawSrcRect(0, 144, 512, 64);
	drawColor(1, 1, 1, 1);
	drawRect(170, 24);
}

static void loadingDraw(void *arg) {
	(void) arg;
	if (!loadingActive)
		return;

	drawMatIdentity();
	drawTranslate(0, 240);
	float alpha = 1;
	if (loadingActive > 0)
		alpha *= loadingActive / 10.0f;
	if (loadPrefade > 0)
		alpha *= (loadPrefadeTotal - loadPrefade) / (float)loadPrefadeTotal;

	switch (curLoadingType) {
	case LOAD_NONE:
		break; /* Not possible */
	case LOAD_INTRO:
		/* Background */
		drawTexture(0, loadingTexture[0]);
		drawSrcRect(0, 0, 2560, 1440);
		drawColorMode(COLOR1);
		drawColor(1, 1, 1, alpha);
		drawRect(854, 480);

		loadingDrawIcon(alpha);
		break;
	case LOAD_BLACK:
		drawTexture(0, loadingTexture[2]);
		drawSrcRect(0, 0, 8, 8);
		drawColorMode(COLOR1);
		drawColor(0, 0, 0, alpha);
		drawRect(854, 480);

		loadingDrawIcon(alpha);
	}
}

void loadingInit(void) {
	loadingTexture[0] = loadTexture("tex/ui/Loading.jpg");
	loadingTexture[1] = loadTexture("tex/ui/Loading2.png");
	loadingTexture[2] = loadTexture("tex/White.png");
	loadingType = LOAD_INTRO;
	curLoadingType = LOAD_INTRO;
	loadingActive = -1;

	addUpdate(UPDATE_UI, loadingNormUpdate, NULL);
	addDrawUpdate(DRAW_LOADING, loadingDraw, NULL);
	loadFuncSet(loadingStart, loadingEnd, loadingUpdate, NULL);

	
}
void loadingFini(void) {
	loadFuncSet(NULL, NULL, NULL, NULL);
	removeUpdate(UPDATE_UI, loadingNormUpdate);
	removeDrawUpdate(DRAW_LOADING);

	deleteTexture(loadingTexture[0]);
	deleteTexture(loadingTexture[1]);
	deleteTexture(loadingTexture[2]);
}