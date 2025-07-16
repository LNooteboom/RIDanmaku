#ifndef SCORE_H
#define SCORE_H

#include "items.h"
#include "danmaku.h"

#define GAUGE_MAX 800

#define SCOREF_HDR 0x45524353
#define SCOREF_MAX_PLAYER 2
#define SCOREF_MAX_SHOTTYPE 2
#define SCOREF_NAME_LEN 8

struct ScoreController {
	uint64_t hiscore;
	uint64_t displayScore;
	uint64_t displayScoreInc;
	uint32_t pointItems;
	uint32_t graze;

	uint32_t pivMax;

	entity_t uiGauge;
	entity_t uiGaugeText;
	char gaugeText[64];

	struct ScoreFile *scoreFile;
};

struct ScoreLeaderBoardEntry {
	char name[SCOREF_NAME_LEN];
	uint64_t score;
	uint8_t day, month, year, stage;
};

struct ScoreFile {
	uint32_t hdr;
	uint32_t nAttempts;
	uint32_t nClears;
	uint32_t playTime; // in seconds
	uint32_t extraUnlocked;
	char currentName[SCOREF_NAME_LEN];
	uint8_t currentDiff;
	uint8_t currentPlayer;
	uint8_t currentShot;
	struct ScoreLeaderBoardEntry leaderboards[5][SCOREF_MAX_PLAYER][SCOREF_MAX_SHOTTYPE][10];
};



void scoreStart(void);
void scoreShowEndScreen(void);

void scoreGraze(void);
void scoreGetItem(struct Item *it, float x, float y);
void scoreAdd(uint32_t score);

void scoreSave(void);

struct ScoreLeaderBoardEntry *scoreGetLeaderBoard(void);
int scoreGetPlace(struct ScoreLeaderBoardEntry *lb);
void scoreInsert(struct ScoreLeaderBoardEntry *lbs, int place, struct ScoreLeaderBoardEntry *n);

void scoreInit(struct Danmaku *game);
void scoreFini(struct Danmaku *game);

#endif