#include <mem.h>
#include <main.h>
#include <gfx/draw.h>
#include <assets.h>

#include "redefined.h"
#include "danmaku.h"
#include "menu.h"

struct Redefined *game;
struct Settings settings;

const char *gameDirName = "thFSW";

static const int settingRes[14] = {
	854, 480, 1366, 768, 1600, 900, 1708, 960, 1920, 1080, 2560, 1440, 3840, 2160
};

static const int *getRecommendedResolution(void) {
	int w, h;
	drawGetMonitorResolution(&w, &h);
	int found = 6;
	for (int i = 1; i < 7; i++) {
		if (settingRes[i * 2] > w || settingRes[i * 2 + 1] > h) {
			found = i - 1;
			break;
		}
	}
	return &settingRes[found * 2];
}
const int *getNextResolution(int w, int h, int change) {
	int found = 6;
	for (int i = 0; i < 7; i++) {
		if (settingRes[i * 2] > w) {
			found = i - 1;
			break;
		}
	}
	found += change;
	found = max(0, min(6, found));
	return &settingRes[found * 2];
}

void saveSettings(void) {
	struct Asset *a = assetUserOpen("settings.dat", true);
	if (!a) {
		logNorm("Failed to write settings\n");
		return;
	}
	assetUserWrite(a, &settings, sizeof(settings));
	assetClose(a);
}

struct EngineSettings *gameGetEngineSettings(void) {
	settings.engine = defaultEngineSettings;
	const int *res = getRecommendedResolution();
	struct Asset *a = assetUserOpen("settings.dat", false);
	if (a) {
		assetRead(a, &settings, sizeof(settings));
		assetClose(a);
		if (settings.engine.resW > res[0] || settings.engine.resH > res[1]) {
			/* Monitor size is smaller than saved settings */
			settings.engine.resW = res[0];
			settings.engine.resH = res[1];
		}
	} else {
		logNorm("Could not open settings file, using defaults\n");
		settings.engine.resW = res[0];
		settings.engine.resH = res[1];
	}

	settings.engine.gameTitle = "Touhou Fictional Spirit Wind";
	settings.engine.gameVersion = "1.00a";
	return &settings.engine;
}

void gamePause(void) {
	eventBlockUpdates = true;
	drawVmUpdateSkip = 38;
	audioPauseMusic(true);
}
void gameUnpause(void) {
	eventBlockUpdates = false;
	drawVmUpdateSkip = 0;
	audioPauseMusic(false);
}

void *gameInit(void) {
	struct Redefined *ri = globalAlloc(sizeof(*ri));
	game = ri;

	if (realWinW % 854 <= 212) {
		int div = realWinW / 854; /* floored */
		winW = realWinW / div;
		winH = realWinH / div;
	} else {
		/* Fallback, use uneven scaling */
		logNorm("WARN: uneven window scaling: %d, %d\n", realWinW, realWinH);
		winW = 854;
		winH = 480;
	}

	assetArchive(1, "ribgm.dat");
	loadingInit();
	keymapInit(ri);
	danmakuInit(ri);
	menuInit(ri);
	dialogInit(ri);

	return ri;
}

void gameFini(void *arg) {
	struct Redefined *ri = arg;
	dialogFini(ri);
	danmakuFini(ri);
	menuFini(ri);
	keymapFini(ri);
	loadingFini();
	game = NULL;
}

DEFINE_MAIN