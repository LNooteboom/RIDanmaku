#ifndef REDEFINED_H
#define REDEFINED_H

#include <audio.h>
#include <gfx/drawvm.h>
#include <physics/collision.h>
#include <main.h>

#include "danmaku.h"
#include "menu.h"
#include "keymap.h"
#include "loading.h"
#include "dialog.h"

#ifdef __linux__
#define min(a, b) ((a) < (b)? (a) : (b))
#define max(a, b) ((a) > (b)? (a) : (b))
#endif

struct SaveController;

enum GameComponentLists {
	CL_DUMMY,
	RPG_TRIGGER,
	RPG_NPC,

	DAN_PLAYER_BULLET,
	DAN_ENEMY,
	DAN_BULLET,
	DAN_BULLET_MGR,
	DAN_BULLET_SPAWNER,
	DAN_BULLET_MARK,
	DAN_LASER_ST,
	DAN_LASER_INF,
	DAN_LASER_CU,
	DAN_ITEM
};

/* Draw priorities */
#define DRAW_ITEM		2901
#define DRAW_LASER		2902
#define DRAW_BULLET_1	2903
#define DRAW_BULLET_2	2904
#define DRAW_BULLET_3	2905
#define DRAW_BULLET_4	2906
#define DRAW_LOADING	4701


struct Settings {
	struct EngineSettings engine;
};

struct Redefined {
	struct Danmaku danmaku;
	struct MenuController menu;
	struct KeymapController keys;
	struct DialogController dialog;
};

extern struct Redefined *game;
extern struct Settings settings;

void gamePause(void);
void gameUnpause(void);

const int *getNextResolution(int w, int h, int change);
void saveSettings(void);

#endif
