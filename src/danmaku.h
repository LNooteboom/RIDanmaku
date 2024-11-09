#ifndef DANMAKU_H
#define DANMAKU_H

#include <ich.h>

#include <stdbool.h>
#include "redefined.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "bulletmgr.h"
#include "bg.h"
#include "boss.h"
#include "laser.h"
#include "items.h"
#include "score.h"
#include "replay.h"

#include <assets.h>

#define EPSILON 0.0001f

#define DAN_MAX_INSTR 256
#define DAN_MAX_VAR 256

#define MAX_CONTINUES 5

#define NEG -999999
#define NEGF -999999.0f
#define IS_NEG(a) (a == -999999)
#define IS_NEGF(a) (a == -999999.0f)

enum Difficulty {
	D_EASY,
	D_NORMAL,
	D_HARD,
	D_LUNATIC,
	D_EXTRA,
};

struct DanmakuSfx {
	uint16_t channel;
	uint8_t volume;
	int8_t panning;
};

struct DanmakuStartParams {
	int stage;
	int difficulty;
	int replayIdx;
	int spellId;
	bool practice;

	/* Player */
	int player;
	int shotType;
	int power;
	int lives;
	int lifePieces;

	/* Score */
	uint64_t score;
	uint32_t gauge;
	uint32_t piv;
};

struct Danmaku {
	bool active;

	struct DanmakuStartParams startParams;
	char script[64];
	int scriptArg;

	/* Stats */
	int stage;
	enum Difficulty difficulty;
	int continuesUsed;

	float shakeDuration, shakeTime;
	float shakeIntensity1, shakeIntensity2;

	struct IchigoState danScript;
	//struct IchigoState bgScript;

	struct DanmakuSfx sfx[128];

	entity_t uiBg;
	entity_t uiHiScore;
	entity_t uiScore;
	entity_t uiGraze;
	entity_t uiPoints;
	entity_t uiFps;
	entity_t uiDifficulty;

#ifndef RELEASE
	entity_t uiDebug;
	char debugBuf[512];
#endif

	struct DanPlayerController player;
	struct EnemyController ec;
	struct BulletController bullet;
	struct BulletMgrControl bmc;
	struct DanBgController bg;
	struct BossController boss;
	struct LaserController laser;
	struct ItemController item;
	struct ScoreController score;
	struct ReplayController replay;
};

extern struct Danmaku *danmaku;

static inline void danmakuSetInstr(int idx, IchigoInstr *instr) {
	if (idx >= DAN_MAX_INSTR) {
		logNorm("Danmaku instruction out of range\n");
		return;
	}
	danmaku->danScript.instrs[idx] = instr;
}
static inline void danmakuSetVar(int idx, enum IchigoRegType type, void *get, void *set) {
	if (idx >= DAN_MAX_VAR) {
		logNorm("Danmaku variable out of range\n");
		return;
	}
	danmaku->danScript.vars[idx].regType = type;
	danmaku->danScript.vars[idx].get.i = get;
	danmaku->danScript.vars[idx].set.i = set;
}

void danmakuSwitch(enum LoadingType lt, const char *script);

void danmakuPlaySfx(int slot);
void danmakuStopSfx(int slot);
void i_sfx(struct IchigoVm *vm);
void i_sfxStop(struct IchigoVm *vm);
void i_shake(struct IchigoVm *vm);

void danmakuInit(struct Redefined *ri);
void danmakuFini(struct Redefined *ri);

#endif
