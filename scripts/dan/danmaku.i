/*
 * STD
 */
// General-purpose
extern(0) int    I0;
extern(1) int    I1;
extern(2) int    I2;
extern(3) int    I3;
extern(4) float  F0;
extern(5) float  F1;
extern(6) float  F2;
extern(7) float  F3;
extern(8) float  F4;
extern(9) float  F5;
extern(10) float  F6;
extern(11) float  F7;
extern(12) string STR0;
extern(13) string STR1;
extern(14) string STR2;
extern(15) string STR3;

// Random
extern(16) int   RI;	// 0 to INT32_MAX (0x7FFF_FFFF)
extern(17) float RF;	// 0.0 to 1.0
extern(18) float RF2;    // -1.0 to 1.0
extern(19) float RRAD;   // -PI to PI

// Movement
extern(20) float POS_X;
extern(21) float POS_Y;
extern(22) float POS_Z;

/*
 * Variables (local)
 */
extern(23) float AIM; // Angle to player
extern(24) int   LIFE;
extern(25) int   FLAGS;
extern(26) int   TIME; // Frames since spawned
extern(27) float ABS_X;
extern(28) float ABS_Y;
extern(29) float ABS_SPEED;
extern(30) float ABS_ANGLE;
extern(31) float REL_X;
extern(32) float REL_Y;
extern(33) float REL_SPEED;
extern(34) float REL_ANGLE;
extern(35) int   LASER_INF_ID;

/* Variables (global) */
extern(100) float PLAYER_X;
extern(101) float PLAYER_Y;
extern(102) int   DIFF;
extern(103) float GAME_SPEED;
extern(104) int   TIMEOUT;
extern(105) float BOSS_X;
extern(106) float BOSS_Y;
extern(107) float BOSS_Z;
extern(108) int   BOSS_ALIVE;
extern(109) int   CAPTURE;
extern(110) int   DIALOG_WAIT;

/*
 * Constants
 */

const float PI   = 3.14159265358979323846;
const float TAU  = 6.28318530717958647693;
const int   NEG  = -999999;
const float NEGF = -999999.0;

const int EASY = 0;
const int NORMAL = 1;
const int HARD = 2;
const int LUNATIC = 3;
const int EXTRA = 4;

// Enemy flags
const int FLAG_NO_HURT		= 1;
const int FLAG_NO_HIT		= 2;
const int FLAG_INVINCIBLE	= 4;
const int FLAG_OFFSCR_LR	= 8;
const int FLAG_OFFSCR_UD	= 16;
const int FLAG_OFFSCREEN    = 24;
const int FLAG_MIRROR		= 32;
const int FLAG_HIDDEN		= 64;
const int FLAG_RESIST		= 128;
const int FLAG_MOVE_LIMIT	= 256;
const int FLAG_NO_DELETE	= 512;

// Interp modes
const int LINEAR = 0;
const int EASEIN1 = 1;
const int EASEIN2 = 2;
const int EASEIN3 = 3;
const int EASEOUT1 = 4;
const int EASEOUT2 = 5;
const int EASEOUT3 = 6;
const int SMOOTHSTEP = 7;
const int EASEINOUT1 = 8;
const int EASEINOUT2 = 9;
const int EASEINOUT3 = 10;
const int EASEOUTIN1 = 11;
const int EASEOUTIN2 = 12;
const int EASEOUTIN3 = 13;
const int STEP = 14;
const int EASEINBACK1 = 15;
const int EASEINBACK2 = 16;
const int EASEINBACK3 = 17;
const int EASEINBACK4 = 18;
const int EASEINBACK5 = 19;
const int EASEOUTBACK1 = 20;
const int EASEOUTBACK2 = 21;
const int EASEOUTBACK3 = 22;
const int EASEOUTBACK4 = 23;
const int EASEOUTBACK5 = 24;

// Aim modes
const int AIM_AT		=  0; // Fan aimed at player
const int AIM_ST		=  1; // Fan aimed with static angle
const int AIM_AT_R		=  2; // Ring aimed at player
const int AIM_ST_R		=  3; // Ring aimed at static angle
const int AIM_AWAY_R	=  4; // Ring aimed away from player
const int AIM_ST_R2		=  5; // Ring aimed away from static angle
const int AIM_RAND		=  6; // Fan with random angles
const int AIM_RAND_R	=  7; // Ring with random speeds
const int AIM_MEEK		=  8; // Complete random
const int AIM_AT_PYR	=  9; // Pyramid aimed at player
const int AIM_ST_PYR	= 10; // Pyramid aimed static

// Step modes
const int STEP_ANGREL		= 0; // Set speed to 'spd', add 'ang' to angle
const int STEP_ANGPL		= 1; // Set speed to 'spd', set angle to angle to player + ang
const int STEP_ANGSAVEPL	= 2; // Set speed to 'spd', set angle to angle to player from saved position + ang
const int STEP_ANGSAVE		= 3; // Set speed to 'spd', set angle to saved angle
const int STEP_ANG			= 4; // Set speed to 'spd', set angle to 'ang'
const int STEP_ANGRND		= 5; // Set speed to 'spd', add random float between (-ang, ang) to angle
const int STEP_ANGPLRND		= 6; // Set speed to 'spd', set angle to angle to player + random between (-ang, ang)
const int STEP_SPDRND		= 7; // Set speed to a random value between (-spd, spd), angle stays the same

// Bounce walls
const int BOUNCE_U = 1;
const int BOUNCE_D = 2;
const int BOUNCE_L = 4;
const int BOUNCE_R = 8;
const int BOUNCE_LR = 12;
const int BOUNCE_ULR = 13;
const int BOUNCE_UDLR = 15;

// Bullet types
const int B8A	= 0; // Ball
const int B8A2	= 1;
const int B8B	= 2; // Popcorn
const int B8B2  = 3; // Popcorn
const int B16A  = 4; // Ball
const int B16A2	= 5;
const int B16B  = 6; // Outline
const int B16B2	= 7;
const int B16C  = 8; // Rice
const int B16CS	= 9;
const int B16D  = 10; // Kunai
const int B16E  = 11; // Shard
const int B16ES	= 12;
const int B16F	= 13; // Amulet
const int B16G  = 14; // Scale
const int B16H	= 15; // Bullet
const int B16I	= 16; // Inverted rice
const int B16J	= 17; // Star
const int B16J2	= 18;
const int B16K	= 19; // Heart
const int B16L	= 20; // Shuriken
const int B16L2 = 21;
const int B16M	= 22; // Coin
const int B32A	= 23; // Ball
const int B32A2	= 24;
const int B32AP	= 25;
const int B32B	= 26; // Rice
const int B32C	= 27; // Knife
const int B32D	= 28; // Star
const int B32D2	= 29;
const int B32E	= 30; // Butterfly
const int B32F	= 31; // Heart
const int B64A	= 32; // Bubble
const int B32G	= 33; // Fireball
const int LINF  = 34; // Infinite laser

// Colors
const int BLACK			= 0;
const int DARKRED		= 1;
const int RED			= 2;
const int PURPLE		= 3;
const int PINK			= 4;
const int DARKBLUE		= 5;
const int BLUE			= 6;
const int DARKCYAN		= 7;
const int CYAN			= 8;
const int DARKGREEN		= 9;
const int GREEN			= 10;
const int LIME			= 11;
const int DARKYELLOW	= 12;
const int YELLOW		= 13;
const int ORANGE		= 14;
const int WHITE			= 15;
const int BLACK32		= 0;
const int RED32			= 1;
const int PINK32		= 2;
const int BLUE32		= 3;
const int CYAN32		= 4;
const int GREEN32		= 5;
const int YELLOW32		= 6;
const int WHITE32		= 7;
const int RED64			= 0;
const int BLUE64		= 1;
const int GREEN64		= 2;
const int YELLOW64		= 3;
const int COPPER		= 0;
const int SILVER		= 1;
const int GOLD			= 2;

// Delay cloud types
const int CLOUD_SHORT = 0;
const int CLOUD_MEDIUM = 1;
const int CLOUD_LONG = 2;

// Trans flags
const int TRANS_WAIT = 0;
const int TRANS_ASYNC = 1;

// Laser flags
const int LASER_ST_ALPHA = 1;
const int LASER_ST_NO_CREATE_BALL = 2;
const int LASER_ST_DELETE_BALL = 4;
const int LASER_INF_FADE = 1;

// Items
const int IT_NONE = -1;
const int IT_POWER = 0;
const int IT_POWER_BIG = 1;
const int IT_POINT = 2;
const int IT_STAR = 3;
const int IT_LIFE = 4;
const int IT_LIFEPIECE = 5;
const int IT_CANCEL = 6;

// Sound effects
const int SE_DAMAGE		= 0;
const int SE_ENDEAD1	= 1;
const int SE_SHOT1		= 2;
const int SE_KIRA1		= 3;
const int SE_LASER1		= 4;
const int SE_TIME		= 5;
const int SE_TIME2		= 6;
const int SE_CARD		= 7;
const int SE_CHARGE		= 8;
const int SE_DENY		= 9;
const int SE_KIRA2		= 10;
const int SE_KIRA3		= 11;
const int SE_LASER2		= 12;
const int SE_POWER1		= 13;
const int SE_SHOT2		= 14;
const int SE_SLASH		= 15;
const int SE_PLST		= 16;
const int SE_PLDEAD		= 17;
const int SE_ITEM		= 18;
const int SE_POWERUP	= 19;
const int SE_EXTEND		= 20;
const int SE_GRAZE		= 21;
const int SE_BOMB1		= 22;
const int SE_BOMB2		= 23;
const int SE_GAUGE		= 24;

// Spell diffmask
const int SPELL_ENHL = 15;
const int SPELL_EN = 3;
const int SPELL_NHL = 14;
const int SPELL_HL = 12;
const int SPELL_EX = 16;

const int FUNC_COND_ALL = 0;
const int FUNC_COND_PLNEAR = 1; // within f0 to player
const int FUNC_COND_CIRCLE = 2; // within f0 to f1,f2
const int FUNC_COND_RECT = 3; // f0<x<f1, f2<y<f3
const int FUNC_COND_MRKNEAR = 4; // within f0 to another bullet marked i0
const int FUNC_COND_ENMNEAR = 5; // within f0 to calling enemy
const int FUNC_COND_MRKLASERNEAR = 6; // within f0 to straight laser marked i0
const int FUNC_COND_INVERT = 0x80;

/*
 * Instructions
 */

// Difficulty: e = easy, n = normal, h = hard, l = lunatic
extern(1) void end();
extern(2) float modeLerp(float v0, float v1, float t, int mode);
extern(3) float normalizeRot(float r);
extern(4) int diffI(int e, int n, int h, int l);
extern(5) float diffF(float e, float n, float h, float l);
extern(6) void diffWait(float e, float n, float h, float l);
extern(7) void sfx(int slot);
extern(8) void bgm(int trackIdx);
extern(9) void bgmStop();

// Enemy
extern(10) entity enmNew(string fn, float x, float y, int hp, int type);
extern(11) entity enmNewA(string fn, float x, float y, int hp, int type);
extern(12) entity enmNewM(string fn, float x, float y, int hp, int type);
extern(13) entity enmNewAM(string fn, float x, float y, int hp, int type);
extern(14) void enmDelete(entity enemy);
extern(15) void setInterrupt(int slot, int hp, float time, string fn);
extern(16) void hurtbox(float w, float h);
extern(17) void hitbox(float w, float h);
extern(18) void lifeSet(int hp);
extern(19) void dropMain(int type);
extern(20) void dropExtra(int type, int amt);
extern(21) void dropArea(float w, float h);
extern(22) void dropItems();
extern(23) void invuln(float time);
extern(24) void dropClear();
extern(25) void enmKillAll();
extern(26) void die();
extern(27) void shake(float time, float intensity1, float intensity2);
extern(28) void onDeath(string fn);

// Movement
extern(30) void movePos(float x, float y, float z);
extern(31) void movePosTime(float time, int mode, float x, float y);
extern(32) void movePosRel(float x, float y, float z);
extern(33) void movePosRelTime(float time, int mode, float x, float y);

extern(34) void moveVel(float ang, float spd);
extern(35) void moveVelTime(float time, int mode, float ang, float spd);
extern(36) void moveVelRel(float ang, float spd);
extern(37) void moveVelRelTime(float time, int mode, float ang, float spd);

extern(38) void moveVelNM(float ang, float spd);
extern(39) void moveVelNMTime(float time, int mode, float ang, float spd);
extern(40) void moveVelNMRel(float ang, float spd);
extern(41) void moveVelNMRelTime(float time, int mode, float ang, float spd);

extern(42) void moveBezier(float time, float x1, float y1, float x2, float y2, float x3, float y3);
extern(43) void moveBezierRel (float time, float x1, float y1, float x2, float y2, float x3, float y3);

extern(44) void moveCircle(float ang, float spd, float rad, float radInc);
extern(45) void moveCircleTime(float time, int mode, float spd, float rad, float radInc);
extern(46) void moveCircleRel(float ang, float spd, float rad, float radInc);
extern(47) void moveCircleRelTime(float time, int mode, float spd, float rad, float radInc);

extern(48) void moveEllipse(float ang, float spd, float rad, float radInc, float ellDir, float ellRatio);
extern(49) void moveEllipseTime(float time, int mode, float spd, float rad, float radInc, float ellDir, float ellRatio);
extern(50) void moveEllipseRel(float ang, float spd, float rad, float radInc, float ellDir, float ellRatio);
extern(51) void moveEllipseRelTime(float time, int mode, float spd, float rad, float radInc, float ellDir, float ellRatio);

extern(52) void moveRand(float time, int mode, float spd);
extern(53) void moveRandRel(float time, int mode, float spd);

extern(54) void moveLimit(float x1, float x2, float y1, float y2);
extern(55) void moveLimitReset();

// DVM
extern(80) void dvmSet(int slot, string fn);
extern(81) void dvmSetMove(int slot, string fn);
extern(82) void dvmDelete(int slot);
extern(83) void dvmEvent(int slot, int ev);
extern(84) void dvmPlay(string fn, float x, float y, float z);
extern(85) void dvmPlayAbs(string fn, float x, float y, float z);
extern(86) void dvmDeath(string fn);
extern(87) void dvmFile(string file);
extern(88) void dvmSetVarI(int slot, int idx, int val);
extern(89) void dvmSetVarF(int slot, int idx, float val);

// Shooting
// Initializes a new bullet manager in slot
extern(100) void bmNew(int slot);
// Copies a bullet manager
extern(101) void bmCopy(int dst, int src);
// Shoots based on bm settings
extern(102) void bmShoot(int slot);
// Sets the aim
extern(103) void bmAim(int slot, int aim);
// Sets sprite and color
extern(104) void bmSprite(int slot, int sprite, int color);
// Sets opacity
extern(105) void bmOpacity(int slot, float opacity);
// Sets delay cloud before shooting
//extern(106) void bmDelay(int slot, float delay);
// Sets shooting position offset from enemy center
extern(107) void bmOffset(int slot, float x, float y);
// Sets position offset using polar form, stacks with bmOffset
extern(108) void bmOffsetRad(int slot, float ang, float dst);
// Sets the distance bullets will spawn based on current angle
extern(109) void bmDist(int slot, float dist);
// Sets speed1 and speed2 of the bm
extern(110) void bmSpeed(int slot, float spd1, float spd2);
// Sets angle1 and angle2
extern(111) void bmAngle(int slot, float ang1, float ang2);
// Sets count1 and count2
extern(112) void bmCount(int slot, int cnt1, int cnt2);
// Sets sound effects for this bm, soundShoot will play on spawn, soundTrans will play on certain transformations
extern(113) void bmSound(int slot, int soundShoot, int soundTrans);

// Adds bullet transformation
extern(114) void bmTrans(int slot, int flags, int type, int a, int b, float p, float q);
// Sets bullet transformation
extern(115) void bmTransSet(int slot, int idx, int flags, int type, int a, int b, float p, float q);
// Adds bullet transformation with extra arguments
extern(116) void bmTransEx(int slot, int flags, int type, int a, int b, int c, int d, float p, float q, float r, float s);
// Sets bullet transformation
extern(117) void bmTransSetEx(int slot, int idx, int flags, int type, int a, int b, int c, int d, float p, float q, float r, float s);

// Sets absolute offset (not relative to caller)
extern(118) void bmOffsetAbs(int slot, float x, float y);
// ORs signal mask
extern(119) void bmSignal(int signal);

// Shoot straight laser
extern(120) void bmLaserStShoot(int slot, int flags, float initLen, float maxLen, float range, float width);
// Shoot infinite laser
extern(121) entity bmLaserInfShoot(int slot, int flags, float length, float width, int startTime, int expandTime, int duration, int shrinkTime);
extern(122) void bmLaserInfRotate(entity en, float wvel);
extern(123) void bmLaserInfAngle(entity en, float ang);
extern(124) void bmLaserInfOffset(entity en, float x, float y);
extern(125) int bmLaserInfTest(entity en);
extern(126) void bmLaserInfBreak(entity en);
extern(127) entity bmLaserInfGet(int id);
extern(128) void bmLaserCuShoot(int slot, int flags, float width, int startTime, int expandTime, int duration);
extern(129) void bmClear(float radius, int item);
extern(130) void bmProtect(int slot, float range);

// BG
extern(140) void bgFile(string file);
extern(141) void bgCall(string fn);
extern(142) void sfxStop(int slot);

// Boss
extern(150) void spellEx(int idx, int time, int score, string bg);
extern(151) void spellEnd();
extern(152) void setBoss(int type, string name, string portrait);
extern(153) void distort(int radius, int color);
extern(154) void lifeMarker(int slot, float life, int color);
extern(155) void timerReset();
extern(156) entity enmNewAF(string fn, float x, float y, int hp, int item);
extern(157) entity enmNewAMF(string fn, float x, float y, int hp, int item);
extern(158) void setTimeout(int slot, string fn);
extern(159) void stars(int amt);
extern(160) void dialogStart(string fn);

extern(161) void funcCall(string fn, int marked, int cond); // fn signature: void fn(entity en, float x, float y)
extern(162) void funcSet(string fn, int marked, int cond);
extern(163) float bulletGetTime(entity en);
extern(164) void bulletSetPos(entity en, float x, float y);
extern(165) void bulletGetAngSpd(entity en, ref float rr, ref float ri, ref float spd);
extern(166) void bulletSetAngSpd(entity en, float rr, float ri, float spd);
extern(167) void bulletSetTrans(entity en, int transIdx, int clearRunning);
extern(168) int bulletGetMark(entity en);
extern(169) void bulletSetMark(entity en, int mrk);

// Init
extern(180) void initSfxLoad(int channel, string file);
extern(181) void initSfxSet(int slot, int channel, int volume, int panning);
extern(182) void initBullet(int slot, int x, int y, int w, int h, int border, int colors, int blend, int anim, float hitbox);
extern(183) void initSpell(int id, int diffMask, int stage, string name);
extern(184) void initStages(int nStages, int nExtraStages);

/*
 * Functions
 */
// Shorthand for setting BM: slot aim sprite color count ang spd
inline void bmSet(int slot, int aim, int sprite, int color, int cnt1, int cnt2, float ang1, float ang2, float spd1, float spd2) {
	bmNew(slot);
	bmAim(slot, aim);
	bmSprite(slot, sprite, color);
	bmCount(slot, cnt1, cnt2);
	bmAngle(slot, ang1, ang2);
	bmSpeed(slot, spd1, spd2);
}

/*
 * Transformations
 */
// Wait until 'time' frames has passed before moving on to the next transformation
inline void bmWait(int slot, int time) { bmTrans(slot, 0, 1, time, NEG, NEGF, NEGF); }
inline void bmWaitSet(int slot, int idx, int time) { bmTransSet(slot, idx, 0, 1, time, NEG, NEGF, NEGF); }
// Wait until signal
inline void bmWaitSignal(int slot, int delayMask) { bmTrans(slot, 0, 2, delayMask, NEG, NEGF, NEGF); }
inline void bmWaitSignalSet(int slot, int idx, int delayMask) { bmTransSet(slot, idx, 0, 2, delayMask, NEG, NEGF, NEGF); }
// Jumps to another transform, can execute 'cnt' times
inline void bmJump(int slot, int dest, int cnt) { bmTrans(slot, 0, 3, dest, cnt, NEGF, NEGF); }
inline void bmJumpSet(int slot, int idx, int dest, int cnt) { bmTransSet(slot, idx, 0, 3, dest, cnt, NEGF, NEGF); }
// Moves faster
inline void bmFast(int slot, int flags) { bmTrans(slot, flags, 4, NEG, NEG, NEGF, NEGF); }
inline void bmFastSet(int slot, int idx, int flags) { bmTransSet(slot, idx, flags, 4, NEG, NEG, NEGF, NEGF); }
// Sets the speed with 'p' (if not NEGF) and angle with 'q' (if not NEGF)
inline void bmVel(int slot, int flags, float ang, float spd) { bmTrans(slot, flags, 5, NEG, NEG, ang, spd); }
inline void bmVelSet(int slot, int idx, int flags, float ang, float spd) { bmTransSet(slot, idx, flags, 5, NEG, NEG, ang, spd); }
// Accelerates the bullet by 'p' into angle 'q', if 'q' is NEGF then accelerate in the current direction
inline void bmAccel(int slot, int flags, int time, float accel, float ang) { bmTrans(slot, flags, 6, time, NEG, accel, ang); }
inline void bmAccelSet(int slot, int idx, int flags, int time, float accel, float ang) { bmTransSet(slot, idx, flags, 6, time, NEG, accel, ang); }
// Accelerates the bullet by 'p' into angle current direction and applies angular velocity 'q'
inline void bmAccelAngv(int slot, int flags, int time, float accel, float wvel) { bmTrans(slot, flags, 7, time, NEG, accel, wvel); }
inline void bmAccelAngvSet(int slot, int idx, int flags, int time, float accel, float wvel) { bmTransSet(slot, idx, flags, 7, time, NEG, accel, wvel); }
// In 'time' frames, slow down to zero and do speed/angle modification based on 'type', 'spd' and 'ang', repeated 'cnt' times
inline void bmStep(int slot, int flags, int time, int cnt, int type, float ang, float spd) { bmTransEx(slot, flags, 8, time, cnt, type, NEG, ang, spd, NEGF, NEGF); }
inline void bmStepSet(int slot, int idx, int flags, int time, int cnt, int type, float ang, float spd) { bmTransSetEx(slot, idx, flags, 8, time, cnt, type, NEG, ang, spd, NEGF, NEGF); }
// Saves current position and angle for later use in bmStep
inline void bmSave(int slot, int flags) { bmTrans(slot, flags, 9, NEG, NEG, NEGF, NEGF); }
inline void bmSaveSet(int slot, int idx, int flags) { bmTransSet(slot, idx, flags, 9, NEG, NEG, NEGF, NEGF); }
// Bounces bullets on selected edge of the screen (with 'walls', 1 = up, 2 = down, 4 = left, 8 = right) 'cnt' times, speed will be set to 'spd' if its not NEGF
inline void bmBounce(int slot, int flags, int cnt, int walls, float spd) { bmTrans(slot, flags, 10, cnt, walls, spd, NEGF); }
inline void bmBounceSet(int slot, int idx, int flags, int cnt, int walls, float spd) { bmTransSet(slot, idx, flags, 10, cnt, walls, spd, NEGF); }
// Same as bmBounce, but with offset 'x' from l/r walls and 'y' from u/d walls
inline void bmBounceCustom(int slot, int flags, int cnt, int walls, float spd, float x, float y) { bmTransEx(slot, flags, 10, cnt, walls, NEG, NEG, spd, x, y, NEGF); }
inline void bmBounceCustomSet(int slot, int idx, int flags, int cnt, int walls, float spd, float x, float y) { bmTransSetEx(slot, idx, flags, 10, cnt, walls, NEG, NEG, spd, x, y, NEGF); }
// Like bmBounce, but wraps around to the opposite side instead
inline void bmWrap(int slot, int flags, int cnt, int walls, float spd) { bmTrans(slot, flags, 11, cnt, walls, spd, NEGF); }
inline void bmWrapSet(int slot, int idx, int flags, int cnt, int walls, float spd) { bmTransSet(slot, idx, flags, 11, cnt, walls, spd, NEGF); }
// Makes the bullet not delete when it goes offscreen for 'time' frames
inline void bmOffscreen(int slot, int time) { bmTrans(slot, 0, 12, time, NEG, NEGF, NEGF); }
inline void bmOffscreenSet(int slot, int idx, int time) { bmTransSet(slot, idx, 0, 12, time, NEG, NEGF, NEGF); }
// Makes the bullet not delete from bombs
inline void bmInvuln(int slot, int flags, int time) { bmTrans(slot, flags, 13, time, NEG, NEGF, NEGF); }
inline void bmInvulnSet(int slot, int idx, int flags, int time) { bmTransSet(slot, idx, flags, 13, time, NEG, NEGF, NEGF); }
// Changes bullet sprite
inline void bmChangeSprite(int slot, int type, int color) { bmTrans(slot, 0, 14, type, color, NEGF, NEGF); }
inline void bmChangeSpriteSet(int slot, int idx, int type, int color) { bmTransSet(slot, idx, 0, 14, type, color, NEGF, NEGF); }
// Changes bullet opacity over time using mode
inline void bmChangeOpacity(int slot, int flags, int time, int mode, float from, float to) { bmTrans(slot, flags, 15, time, mode, from, to); }
inline void bmChangeOpacitySet(int slot, int idx, int flags, int time, int mode, float from, float to) { bmTransSet(slot, idx, flags, 15, time, mode, from, to); }
// Changes bullet size over time using mode
inline void bmChangeSize(int slot, int flags, int time, int mode, float from, float to) { bmTrans(slot, flags, 16, time, mode, from, to); }
inline void bmChangeSizeSet(int slot, int idx, int flags, int time, int mode, float from, float to) { bmTransSet(slot, idx, flags, 16, time, mode, from, to); }
// Plays a sound
inline void bmPlaySound(int slot, int flags, int sound) { bmTrans(slot, flags, 17, sound, NEG, NEGF, NEGF); }
inline void bmPlaySoundSet(int slot, int idx, int flags, int sound) { bmTransSet(slot, idx, flags, 17, sound, NEG, NEGF, NEGF); }
// Deletes the bullet
inline void bmDelete(int slot, int eff) { bmTrans(slot, 0, 18, eff, NEG, NEGF, NEGF); }
inline void bmDeleteSet(int slot, int idx, int eff) { bmTransSet(slot, idx, 0, 18, eff, NEG, NEGF, NEGF); }
// Spawns new bullets
inline void bmSpawn(int slot, int flags, int transformIndex, int aim, int sprite, int color, int cnt1, int cnt2, float spd1, float spd2, float ang1, float ang2) {
	bmTransEx(slot, flags, 19, 0, 0, transformIndex, aim, spd1, spd2, NEGF, NEGF);
	bmTransEx(slot, flags, 20, sprite, color, cnt1, cnt2, ang1, ang2, NEGF, NEGF);
}
inline void bmSpawnSet(int slot, int idx, int flags, int transformIndex, int aim, int sprite, int color, int cnt1, int cnt2, float spd1, float spd2, float ang1, float ang2) {
	bmTransSetEx(slot, idx, flags, 19, 0, 0, transformIndex, aim, spd1, spd2, NEGF, NEGF);
	bmTransSetEx(slot, idx + 1, flags, 20, sprite, color, cnt1, cnt2, ang1, ang2, NEGF, NEGF);
}
// Spawns new bullets and destroys this one
inline void bmBreak(int slot, int flags, int transformIndex, int aim, int sprite, int color, int cnt1, int cnt2, float spd1, float spd2, float ang1, float ang2) {
	bmTransEx(slot, flags, 19, 1, 0, transformIndex, aim, spd1, spd2, NEGF, NEGF);
	bmTransEx(slot, flags, 20, sprite, color, cnt1, cnt2, ang1, ang2, NEGF, NEGF);
}
inline void bmBreakSet(int slot, int idx, int flags, int transformIndex, int aim, int sprite, int color, int cnt1, int cnt2, float spd1, float spd2, float ang1, float ang2) {
	bmTransSetEx(slot, idx, flags, 19, 1, 0, transformIndex, aim, spd1, spd2, NEGF, NEGF);
	bmTransSetEx(slot, idx + 1, flags, 20, sprite, color, cnt1, cnt2, ang1, ang2, NEGF, NEGF);
}
inline void bmCloud(int slot, int type) {
	bmTransSet(slot, 0, 0, 22, type, NEG, NEGF, NEGF);
}
inline void bmMark(int slot, int flags, int mrk) { bmTrans(slot, flags, 23, mrk, NEG, NEGF, NEGF); }
inline void bmMarkSet(int slot, int idx, int flags, int mrk) { bmTransSet(slot, idx, flags, 23, mrk, NEG, NEGF, NEGF); }


inline void flagSet(int fl) {
	FLAGS |= fl;
}
inline void flagClear(int fl) {
	FLAGS &= ~fl;
}

inline void circlePos(ref float x, ref float y, float r, float d) {
	x = cos(r) * d;
	y = sin(r) * d;
}

inline void spellE(int idx, int time, int score, string bg) {
	spellEx(idx + DIFF, time, score, bg);
}
inline void spellN(int idx, int time, int score, string bg) {
	spellEx((idx - 1) + DIFF, time, score, bg);
}
inline void spellH(int idx, int time, int score, string bg) {
	spellEx((idx - 2) + DIFF, time, score, bg);
}
inline void clearInterrupt(int slot) {
	setInterrupt(slot, -1, 0, "");
}

inline void bossWait() {
	while (BOSS_ALIVE) {
		wait(1);
	}
}

inline void dialogWait() {
	while (DIALOG_WAIT) {
		wait(1);
	}
	DIALOG_WAIT = 1;
}


inline void effChargeRed(float x, float y) { dvmPlay("effChargeRed", x, y, 0); }
inline void effChargeMagenta(float x, float y) { dvmPlay("effChargeMagenta", x, y, 0); }
inline void effChargeBlue(float x, float y) { dvmPlay("effChargeBlue", x, y, 0); }
inline void effChargeCyan(float x, float y) { dvmPlay("effChargeCyan", x, y, 0); }
inline void effChargeGreen(float x, float y) { dvmPlay("effChargeGreen", x, y, 0); }
inline void effChargeYellow(float x, float y) { dvmPlay("effChargeYellow", x, y, 0); }

inline void dvmRGirl(int slot) { dvmSetMove(slot, "enmRGirl"); dvmDeath("effExplodeRed"); }
inline void dvmGGirl(int slot) { dvmSetMove(slot, "enmGGirl"); dvmDeath("effExplodeGreen"); }
inline void dvmBGirl(int slot) { dvmSetMove(slot, "enmBGirl"); dvmDeath("effExplodeBlue"); }
inline void dvmYGirl(int slot) { dvmSetMove(slot, "enmYGirl"); dvmDeath("effExplodeYellow"); }
inline void dvmSFGirl(int slot) { dvmSetMove(slot, "enmSFGirl"); dvmDeath("effExplodeRed"); }
inline void dvmRCir(int slot) { dvmSet(slot, "enmRCir"); dvmDeath("effExplodeRed"); }
inline void dvmGCir(int slot) { dvmSet(slot, "enmGCir"); dvmDeath("effExplodeGreen"); }
inline void dvmBCir(int slot) { dvmSet(slot, "enmBCir"); dvmDeath("effExplodeBlue"); }
inline void dvmPCir(int slot) { dvmSet(slot, "enmPCir"); dvmDeath("effExplodeBlue"); }
inline void dvmREff(int slot) { dvmSet(slot, "enmCircleEffRed"); }
inline void dvmGEff(int slot) { dvmSet(slot, "enmCircleEffGreen"); }
inline void dvmBEff(int slot) { dvmSet(slot, "enmCircleEffBlue"); }
inline void dvmYEff(int slot) { dvmSet(slot, "enmCircleEffPurple"); }
inline void dvmRSpir(int slot) { dvmSet(slot, "enmRSpirit"); dvmDeath("effExplodeRed"); }
inline void dvmGSpir(int slot) { dvmSet(slot, "enmGSpirit"); dvmDeath("effExplodeGreen"); }
inline void dvmBSpir(int slot) { dvmSet(slot, "enmBSpirit"); dvmDeath("effExplodeBlue"); }
inline void dvmYSpir(int slot) { dvmSet(slot, "enmYSpirit"); dvmDeath("effExplodeYellow"); }
inline void dvmRBook(int slot) { dvmSet(slot, "enmRBook"); dvmDeath("effExplodeRed"); }
inline void dvmGBook(int slot) { dvmSet(slot, "enmGBook"); dvmDeath("effExplodeGreen"); }
inline void dvmBBook(int slot) { dvmSet(slot, "enmBBook"); dvmDeath("effExplodeBlue"); }
inline void dvmYBook(int slot) { dvmSet(slot, "enmYBook"); dvmDeath("effExplodeYellow"); }

inline void hitboxGirl() {
	hurtbox(24, 24);
	hitbox(16, 16);
}
inline void hitboxSFGirl() {
	hurtbox(40, 40);
	hitbox(16, 16);
}
inline void hitboxCir() {
	hurtbox(24, 24);
	hitbox(16, 16);
}
inline void hitboxSpir() {
	hurtbox(24, 24);
	hitbox(16, 16);
}

const int GIRL_R = 0;
const int GIRL_B = 1;
const int GIRL_G = 2;
const int GIRL_Y = 3;
const int GIRL_STAR = 4;
void setGirl() {
	switch (I0 & 3) {
	case GIRL_R:
	default:
		dvmRGirl(0);
		dvmREff(1);
		dropMain(IT_POWER);
		break;
	case GIRL_B:
		dvmBGirl(0);
		dvmBEff(1);
		dropMain(IT_POINT);
		break;
	case GIRL_G:
		dvmGGirl(0);
		dvmGEff(1);
		dropMain(IT_POWER);
		break;
	case GIRL_Y:
		dvmYGirl(0);
		dvmYEff(1);
		dropMain(IT_POINT);
		break;
	}
	if (I0 & 4)
		dropMain(IT_STAR);
	hitboxGirl();
}

const int CIR_R = 0;
const int CIR_B = 1;
const int CIR_G = 2;
const int CIR_P = 3;
const int CIR_STAR = 4;
void setCir() {
	switch (I0 & 3) {
	case CIR_R:
	default:
		dvmRCir(0);
		dropMain(IT_POWER);
		break;
	case CIR_B:
		dvmBCir(0);
		dropMain(IT_POINT);
		break;
	case CIR_G:
		dvmGCir(0);
		dropMain(IT_POWER);
		break;
	case CIR_P:
		dvmPCir(0);
		dropMain(IT_POINT);
		break;
	}
	if (I0 & 4)
		dropMain(IT_STAR);
	hitboxCir();
}

inline void setSFGirl() {
	dvmSFGirl(0);
	hitboxSFGirl();
}

void setSpir() {
	switch (I0 & 3) {
	case GIRL_R:
	default:
		dvmRSpir(0);
		dvmREff(1);
		dropMain(IT_POWER);
		break;
	case GIRL_B:
		dvmBSpir(0);
		dvmBEff(1);
		dropMain(IT_POINT);
		break;
	case GIRL_G:
		dvmGSpir(0);
		dvmGEff(1);
		dropMain(IT_POWER);
		break;
	case GIRL_Y:
		dvmYSpir(0);
		dvmYEff(1);
		dropMain(IT_POINT);
		break;
	}
	if (I0 & 4)
		dropMain(IT_STAR);
	hitboxSpir();
}

void enmBreakWave() {
	float r = 16;
	FLAGS |= FLAG_NO_DELETE;
	while (r < 640) {
		bmClear(r, I0);
		r += 10;
		wait(1);
	}
	die();
}
inline void breakWave() {
	enmNew("enmBreakWave", 0, 0, 9999, 1);
}
inline void breakWaveNi() {
	enmNew("enmBreakWave", 0, 0, 9999, 0);
}





// USER DATA
const int SPELL_ST2_1E = 0;
const int SPELL_ST2_2E = 4;

const int SPELL_BHA_1E = 0;
const int SPELL_BHA_2E = 4;
const int SPELL_BHA_3E = 8;
const int SPELL_BHA_4E = 12;
const int SPELL_BHA_5E = 16;

void initialize() {
	initStages(2, 0);

	initSfxLoad( 0, "sfx/dan_damage.wav");
	initSfxSet(SE_DAMAGE, 0, 64, 0);
	initSfxLoad( 1, "sfx/dan_endead1.wav");
	initSfxSet(SE_ENDEAD1, 1, 64, 0);
	initSfxLoad( 2, "sfx/dan_shot1.wav");
	initSfxSet(SE_SHOT1, 2, 32, 0);
	initSfxLoad( 3, "sfx/dan_kira1.wav");
	initSfxSet(SE_KIRA1, 3, 128, 0);
	initSfxLoad( 4, "sfx/dan_laser1.wav");
	initSfxSet(SE_LASER1, 4, 255, 0);
	initSfxLoad( 5, "sfx/dan_time.wav");
	initSfxSet(SE_TIME, 5, 96, 0);
	initSfxSet(SE_TIME2, 5, 192, 0);
	initSfxLoad( 6, "sfx/dan_card.wav");
	initSfxSet(SE_CARD, 6, 128, 0);
	initSfxLoad( 7, "sfx/dan_charge.wav");
	initSfxSet(SE_CHARGE, 7, 192, 0);
	initSfxLoad( 8, "sfx/dan_deny.wav");
	initSfxSet(SE_DENY, 8, 255, 0);
	initSfxLoad( 9, "sfx/dan_kira2.wav");
	initSfxSet(SE_KIRA2, 9, 128, 0);
	initSfxLoad(10, "sfx/dan_kira3.wav");
	initSfxSet(SE_KIRA3, 10, 128, 0);
	initSfxLoad(11, "sfx/dan_laser2.wav");
	initSfxSet(SE_LASER2, 11, 255, 0);
	initSfxLoad(12, "sfx/dan_power1.wav");
	initSfxSet(SE_POWER1, 12, 192, 0);
	initSfxLoad(13, "sfx/dan_shot2.wav");
	initSfxSet(SE_SHOT2, 13, 32, 0);
	initSfxLoad(14, "sfx/dan_slash.wav");
	initSfxSet(SE_SLASH, 14, 255, 0);
	initSfxLoad(15, "sfx/dan_plst.wav");
	initSfxSet(SE_PLST, 15, 64, 0);
	initSfxLoad(16, "sfx/dan_pldead1.wav");
	initSfxSet(SE_PLDEAD, 16, 128, 0);
	initSfxLoad(17, "sfx/dan_item.wav");
	initSfxSet(SE_ITEM, 17, 128, 0);
	initSfxLoad(18, "sfx/dan_powerup.wav");
	initSfxSet(SE_POWERUP, 18, 255, 0);
	initSfxLoad(19, "sfx/dan_extend.wav");
	initSfxSet(SE_EXTEND, 19, 255, 0);
	initSfxLoad(20, "sfx/dan_graze.wav");
	initSfxSet(SE_GRAZE, 20, 64, 0);
	initSfxLoad(21, "sfx/dan_bomb1.wav");
	initSfxSet(SE_BOMB1, 21, 255, 0);
	initSfxLoad(22, "sfx/dan_bomb2.wav");
	initSfxSet(SE_BOMB2, 22, 255, 0);
	initSfxLoad(23, "sfx/dan_gauge.wav");
	initSfxSet(SE_GAUGE, 23, 96, 0);

	initBullet(B8A,     0, 240,  8,  8, 0, 16, 0, 0,  2.0);
	initBullet(B8A2,    0, 240,  8,  8, 0, 16, 1, 0,  2.0);
	initBullet(B8B,     0, 248,  8,  8, 0, 16, 0, 2,  2.0);
	initBullet(B8B2,    0, 248,  8,  8, 0, 16, 1, 3,  2.0);
	initBullet(B16A,    0,  48, 16, 16, 0, 16, 0, 0,  3.0);
	initBullet(B16A2,   0,  48, 16, 16, 0, 16, 1, 0,  3.0);
	initBullet(B16B,    0,  32, 16, 16, 0, 16, 0, 0,  3.0);
	initBullet(B16B2,   0,  32, 16, 16, 0, 16, 1, 0,  3.0);
	initBullet(B16C,    0,  64, 16, 16, 2, 16, 0, 1,  2.0);
	initBullet(B16CS,   0,  64, 16, 16, 2, 16, 0, 2,  2.0);
	initBullet(B16D,    0,  80, 16, 16, 2, 16, 0, 1,  2.0);
	initBullet(B16E,    0,  96, 16, 16, 2, 16, 0, 1,  2.0);
	initBullet(B16ES,   0,  80, 16, 16, 2, 16, 0, 2,  2.0);
	initBullet(B16F,    0, 112, 16, 16, 1, 16, 0, 1,  2.0);
	initBullet(B16G,    0,  16, 16, 16, 3, 16, 0, 1,  2.0);
	initBullet(B16H,    0, 128, 16, 16, 3, 16, 0, 1,  2.0);
	initBullet(B16I,    0, 144, 16, 16, 2, 16, 0, 1,  2.0);
	initBullet(B16J,    0, 160, 16, 16, 0, 16, 0, 2,  3.0);
	initBullet(B16J2,   0, 160, 16, 16, 0, 16, 0, 3,  3.0);
	initBullet(B16K,    0, 192, 16, 16, 3, 16, 0, 1,  3.0);
	initBullet(B16L,    0, 208, 16, 16, 0, 16, 0, 2,  3.0);
	initBullet(B16L2,   0, 208, 16, 16, 0, 16, 0, 3,  3.0);
	initBullet(B16M,  128, 240, 16, 16, 0,  3, 0, 2,  3.0);
	initBullet(B32A,  256,  64, 32, 32, 0,  8, 0, 0,  5.0);
	initBullet(B32A2, 256,  64, 32, 32, 0,  8, 1, 0,  5.0);
	initBullet(B32AP, 256,  64, 32, 32, 0,  8, 0, 4,  5.0);
	initBullet(B32B,  256,  96, 32, 32, 3,  8, 0, 1,  4.0);
	initBullet(B32C,  256, 128, 32, 32, 3,  8, 0, 1,  4.0);
	initBullet(B32D,  256, 160, 32, 32, 3,  8, 0, 7,  4.0);
	initBullet(B32D2, 256, 160, 32, 32, 3,  8, 0, 8,  4.0);
	initBullet(B32E,  256, 192, 32, 32, 0,  8, 0, 1,  4.0);
	initBullet(B32F,  256, 224, 32, 32, 3,  8, 0, 1,  5.0);
	initBullet(B64A,  256, 256, 64, 64, 0,  4, 1, 5, 14.0);
	initBullet(B32G,  256, 320, 32, 32, 3,  4, 1, 6,  4.0);
	initBullet(LINF,    0,   0, 16, 16, 2, 16, 1, 1,  3.0);

	initSpell(SPELL_BHA_1E, SPELL_ENHL, 0, "酒符　「Spicy Bubbles」");
	initSpell(SPELL_BHA_2E, SPELL_ENHL, 0, "給仕　「Kicking Out Unwanted Guests」");
	initSpell(SPELL_BHA_3E, SPELL_ENHL, 0, "給仕　「Leave a Tip」");
	initSpell(SPELL_BHA_4E, SPELL_ENHL, 0, "酒符　「Morning Hangover」");
}