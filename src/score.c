#include "redefined.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void scoreStart(void) {
	struct ScoreController *s = &danmaku->score;
	struct DanmakuState *state = &danmaku->state;
	s->pointItems = 0;
	s->displayScore = 0;
	s->displayScoreInc = 0;
	s->graze = 0;
	s->hiscore = scoreGetLeaderBoard()[0].score;

	switch (danmaku->state.difficulty) {
	case D_EASY:
		if (!state->piv)
			state->piv = 25000;
		s->pivMax = 50000;
		break;
	case D_NORMAL:
		if (!state->piv)
			state->piv = 50000;
		s->pivMax = 100000;
		break;
	case D_HARD:
		if (!state->piv)
			state->piv = 100000;
		s->pivMax = 200000;
		break;
	case D_LUNATIC:
		if (!state->piv)
			state->piv = 200000;
		s->pivMax = 400000;
		break;
	case D_EXTRA:
		if (!state->piv)
			state->piv = 200000;
		s->pivMax = 400000;
		break;
	}

	s->uiGauge = newEntity();
	drawVmNew(s->uiGauge, "uiGauge");
	s->uiGaugeText = newEntity();
	drawVmNew(s->uiGaugeText, "uiGaugeText");

	s->scoreFile->nAttempts += 1;
	scoreSave();
}
void scoreShowEndScreen(void) {

}

static void gaugeAdd(int amt) {
	struct DanmakuState *s = &danmaku->state;
	if (s->gauge < GAUGE_MAX) {
		s->gauge += amt;
		if (s->gauge >= GAUGE_MAX) {
			s->gauge = GAUGE_MAX;
			danmakuPlaySfx(24);
		}
	}
}
void scoreGraze(void) {
	struct ScoreController *s = &danmaku->score;
	s->graze += 1;
	danmaku->state.piv = fminf(s->pivMax, danmaku->state.piv + 1);
	gaugeAdd(1);
}

static void valueText(float x, float y, int value, int color) {
	entity_t pv = newEntity();
	struct Transform *tf = newComponent(TRANSFORM, pv);
	tf->x = truncf(x);
	tf->y = truncf(y);
	struct DrawVm *d = drawVmNew(pv, "pointValue");
	struct IchigoLocals *l = getComponent(DRAW_VM_LOCALS, pv);
	l->i[0] = value;
	l->i[1] = color;
}

static float scoreGetMult(struct ScoreController *s) {
	return danmaku->state.gauge == GAUGE_MAX ? 2.0f : lerp(1.0f, 1.5f, danmaku->state.gauge / (float)GAUGE_MAX);
}
void scoreGetItem(struct Item *it, float x, float y) {
	struct ScoreController *s = &danmaku->score;
	switch (it->type) {
	case ITEM_POWER:
	case ITEM_POWER_BIG:
		break;
	case ITEM_POINT:
	{
		s->pointItems += 1;
		float value = danmaku->state.piv - (danmaku->state.piv % 10);
		int color;
		if (it->state == IS_MAGNET) {
			color = 0xFFFF00;
		} else {
			value *= lerp(0.25f, 0.75f, fmaxf(0, (448 - y) / (448 - danmaku->item.autoCollectY)));
			color = 0xFFFFFF;
		}
		value *= scoreGetMult(s);
		int iv = value;
		iv -= iv % 10;
		scoreAdd(iv);
		valueText(x, y, iv, color);
		break;
	}
	case ITEM_STAR:
		if (danmaku->state.gauge == GAUGE_MAX) {
			scoreAdd(50000);
			valueText(x, y, 50000, 0xFFFF00);
		} else {
			gaugeAdd(40);
			scoreAdd(10);
			valueText(x, y, 10, 0xFFFFFF);
		}
		break;
	case ITEM_CANCEL:
		danmaku->state.piv += 2;
		if (danmaku->state.piv > s->pivMax)
			danmaku->state.piv = s->pivMax;
		break;
	}
}

void scoreAdd(uint32_t score) {
	struct ScoreController *s = &danmaku->score;
	struct DanmakuState *state = &danmaku->state;
	state->score += score;
	if (state->score > s->hiscore)
		s->hiscore = state->score;
	uint32_t sa = (state->score - s->displayScore) / 20;
	sa -= sa % 10;
	if (sa < 20)
		sa = 20;
	if (sa > s->displayScoreInc)
		s->displayScoreInc = sa;
}

static void scoreUpdate(void *arg) {
	struct ScoreController *s = arg;
	struct DanmakuState *state = &danmaku->state;
	if (!danmaku->active)
		return;

	s->displayScore += s->displayScoreInc;
	if (s->displayScore >= state->score) {
		s->displayScoreInc = 0;
		s->displayScore = state->score;
	}

	struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, s->uiGauge);
	if (l) {
		l->f[0] = fminf(state->gauge / (float)GAUGE_MAX, 1);
	}
	drawVmEvent2(s->uiGauge, 2);

	l = getComponentOpt(DRAW_VM_LOCALS, s->uiGaugeText);
	if (l) {
		float mult = scoreGetMult(s);
		snprintf(s->gaugeText, 64, "%d x %.2f", state->piv - (state->piv % 10), mult);
		l->str[0] = s->gaugeText;
		drawVmEvent2(s->uiGaugeText, 2);
	}
}

struct ScoreLeaderBoardEntry *scoreGetLeaderBoard(void) {
	struct ScoreController *s = &danmaku->score;
	return &(s->scoreFile->leaderboards[danmaku->state.difficulty][danmaku->state.player][danmaku->state.shotType][0]);
}

int scoreGetPlace(struct ScoreLeaderBoardEntry *lb) {
	for (int i = 0; i < 10; i++) {
		if (danmaku->state.score >= lb[i].score) {
			return i;
		}
	}
	return -1;
}

void scoreInsert(struct ScoreLeaderBoardEntry *lbs, int place, struct ScoreLeaderBoardEntry *n) {
	for (int i = 9; i > place; i--) {
		memcpy(&lbs[i], &lbs[i - 1], sizeof(*n));
	}
	memcpy(&lbs[place], n, sizeof(*n));

	// Set Date
	time_t t;
	time(&t);
	struct tm *timeInfo = localtime(&t);
	lbs[place].year = timeInfo->tm_year % 100;
	lbs[place].month = timeInfo->tm_mon + 1;
	lbs[place].day = timeInfo->tm_mday;
}

void scoreSave(void) {
	struct ScoreController *s = &danmaku->score;
	s->scoreFile->hdr = SCOREF_HDR;
	struct Asset *a = assetUserOpen("score.dat", true);
	if (a) {
		assetUserWrite(a, s->scoreFile, sizeof(*s->scoreFile));
		assetClose(a);
	}
}

static void scoreNanashi(struct ScoreLeaderBoardEntry *e, uint64_t score) {
	strcpy(e->name, "Nanashi");
	e->score = score;
	e->stage = 255;
}

void scoreInit(struct Danmaku *game) {
	struct ScoreController *s = &game->score;
	addUpdate(UPDATE_LATE, scoreUpdate, s);

	// Load score file
	bool loaded = false;
	s->scoreFile = globalAlloc(sizeof(*s->scoreFile));
	struct Asset *a = assetUserOpen("score.dat", false);
	if (a) {
		size_t rd = assetRead(a, s->scoreFile, sizeof(*s->scoreFile));
		if (rd != sizeof(*s->scoreFile) || s->scoreFile->hdr != SCOREF_HDR) {
			memset(s->scoreFile, 0, sizeof(*s->scoreFile));
		} else {
			logDebug("Loaded score file");
			loaded = true;
		}
		assetClose(a);
	}

	if (!loaded) {
		struct ScoreLeaderBoardEntry *e = &s->scoreFile->leaderboards[0][0][0][0];
		for (int i = 0; i < 5 * 2 * 2; i++) {
			scoreNanashi(e++, 10000000);
			scoreNanashi(e++,  9000000);
			scoreNanashi(e++,  8000000);
			scoreNanashi(e++,  7000000);
			scoreNanashi(e++,  6000000);
			scoreNanashi(e++,  5000000);
			scoreNanashi(e++,  4000000);
			scoreNanashi(e++,  3000000);
			scoreNanashi(e++,  2000000);
			scoreNanashi(e++,  1000000);
		}
	}
}

void scoreFini(struct Danmaku *game) {
	removeUpdate(UPDATE_LATE, scoreUpdate);
}