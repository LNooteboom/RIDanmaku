#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <ecs.h>
#include <mem.h>

struct Redefined;

enum MenuState {
	MENU_NONE,
	MENU_INGAME,

	/* Main menu */
	MENU_MAIN,
	MENU_SETTINGS,
	MENU_DIFFICULTY,
	MENU_PLAYER,
	MENU_SHOTTYPE,
	MENU_STAGE_PRAC,
	MENU_SPELL_PRAC,
	MENU_MUSIC_ROOM,

	MENU_SCORE,

	MENU_SCRIPTS,

	MENU_PAUSE,
	MENU_PAUSE_RETRY,
	MENU_PAUSE_QUIT,

	MENU_END,
	MENU_END_RETRY,
	MENU_END_QUIT,
	MENU_END_SCORE,
};

struct MenuController {
	enum MenuState state;

	entity_t background[16];

	int selected;
	entity_t buttons[16];
	int nButtons;
	int selectMin, selectMax;

	bool leftRight;

	int settingsBgm;

	int scriptScroll;
	struct Vector scriptFiles;
	char buttonFns[32][16];

	entity_t keyboardButtons[91];
	int keyboardX, keyboardY, keyboardCursor;
	int scorePlace;
	entity_t scoreText[10];
	char scoreTextContent[10][64];
	bool allowContinue;
};

void menuSetIngame(void);

void menuEndScreen(bool allowContinue, bool cleared);

void menuInit(struct Redefined *game);
void menuFini(struct Redefined *game);

#endif
