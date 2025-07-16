#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <ecs.h>
#include <mem.h>

struct Redefined;

#define MENU_EVENT_DELETE 1
#define MENU_EVENT_SELECT 2
#define MENU_EVENT_UNSELECT 3
#define MENU_EVENT_CHOOSE 4
#define MENU_EVENT_FADE 5
#define MENU_EVENT_UNFADE 6

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
void menuDvmEventAll(struct MenuController *m, int selEvent, int unselEvent);
void menuSelect(struct MenuController *m, int sel);

/* menumain.c */
void mainMenuStartNoBg(struct MenuController *m);
void mainMenuStart(struct MenuController *m);
void mainMenuChoose(struct MenuController *m);
void scriptsMenuStart(struct MenuController *m);
void scriptsMenuKey(struct MenuController *m, int match);
void scriptsMenuEnd(struct MenuController *m);
void difficultyMenuStart(struct MenuController *m);
void difficultyMenuChoose(struct MenuController *m);
void difficultyMenuEnd(struct MenuController *m);
void shotMenuStart(struct MenuController *m);
void shotMenuChoose(struct MenuController *m);
void shotMenuEnd(struct MenuController *m);

/* menusettings.c */
void settingsMenuStart(struct MenuController *m);
void settingsMenuChoose(struct MenuController *m);
void settingsMenuValueChange(struct MenuController *m, int change);

/* menupause.c */
void pauseMenuStart(struct MenuController *m);
void pauseMenuChoose(struct MenuController *m);
void pauseMenuEnd(struct MenuController *m);
void endMenuChoose(struct MenuController *m);
void endMenuEnd(struct MenuController *m);
void menuEndScoreKey(struct MenuController *m, int key);

#endif
