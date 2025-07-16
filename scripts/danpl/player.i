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
extern(18) float RF2;   // -1.0 to 1.0
extern(19) float RRAD;  // -PI to PI

// Movement
extern(20) float POS_X;
extern(21) float POS_Y;
extern(22) float POS_Z;

extern(23) float GAME_SPEED;
extern(24) int POWER;
extern(25) float TARGET_X;
extern(26) float TARGET_Y;
extern(27) int HAVE_TARGET;
extern(28) int SHOOT;
extern(29) int FOCUS;
extern(30) int DEATH;
extern(31) int GAUGE;
extern(32) int BOMB;
extern(33) int TIME;

const float PI = 3.14159265358979323846;

const int GAUGE_MAX = 800;

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

// Sound effects
const int SE_DAMAGE = 0;
const int SE_ENDEAD1 = 1;
const int SE_SHOT1 = 2;
const int SE_KIRA1 = 3;
const int SE_LASER1 = 4;
const int SE_TIME = 5;
const int SE_TIME2 = 6;
const int SE_CARD = 7;
const int SE_CHARGE = 8;
const int SE_DENY = 9;
const int SE_KIRA2 = 10;
const int SE_KIRA3 = 11;
const int SE_LASER2 = 12;
const int SE_POWER1 = 13;
const int SE_SHOT2 = 14;
const int SE_SLASH = 15;
const int SE_PLST = 16;
const int SE_PLDEAD = 17;
const int SE_ITEM = 18;
const int SE_POWERUP = 19;
const int SE_EXTEND = 20;
const int SE_GRAZE = 21;
const int SE_BOMB1 = 22;
const int SE_BOMB2 = 23;

/*
 * Player Controls
*/
extern(0) void log(string txt);
extern(1) void hitbox(float w);
extern(2) void grazebox(float w);
extern(3) void speed(float norm, float focus);
extern(4) float normalizeRot(float r);
extern(5) void onBomb(string fn);
extern(6) void onBombHit(string fn);
extern(7) void sfx(int idx);
extern(8) float modeLerp(float v0, float v1, float t, int mode);
extern(9) void dvmFile(string file);
extern(10) void dvmMain(string fn);
extern(11) void dvmEvent(int ev);
extern(12) void dvmPlay(string fn, float x, float y, float z);
extern(13) void dvmPlayAbs(string fn, float x, float y, float z);
extern(14) void optionNew(int idx, float x, float y, string dvm);
extern(15) void optionDelete(int idx);
extern(16) void optionEvent(int idx, int ev);
extern(17) void optionGetPos(int idx, ref float x, ref float y);
extern(18) void optionSetPos(int idx, float x, float y);
extern(19) entity bulletNew(int option, float x, float y, float r, float spd, float hitW, float hitH, int dmg, int flags, string dvm);
extern(20) void bombCircle(float x, float y, float r, int dmg, int cancel);
extern(21) void bombRect(float x, float y, float rot, float w, float h, int dmg, int cancel);
extern(22) void itemFallSpeed(float spd);
extern(23) void itemMagnetRange(float norm, float focus);
extern(24) void itemAutoCollectY(float y);
extern(25) void maxPower(int power, int powerItemValue);
extern(26) void bombAutoCollect(float time);
extern(27) void bombEnd();
extern(28) void sfxStop(int idx);
extern(29) void shake(float time, float startIntensity, float endIntensity);
extern(30) void itemNew(float x, float y, int type);

inline void circlePos(ref float x, ref float y, float r, float d) {
	x = cos(r) * d;
	y = sin(r) * d;
}