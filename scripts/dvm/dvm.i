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
 * DVM
 */

extern(23) int FLAGS;
extern(24) int LANG;
extern(25) int API;

extern(30) int ANIM_FLAGS;
extern(31) float ANIM_LENGTH;

const float PI = 3.14159265358979323846;

// Flags
const int FLAG_INVISIBLE = 1;
const int FLAG_FLIP_SRC_X = 2;
const int FLAG_FLIP_SRC_Y = 4;
const int FLAG_ANIM_EVENT = 8;
const int FLAG_NO_Z_BUFFER_WRITE = 16;
const int FLAG_NO_CHILD_EVENT = 32;
const int FLAG_TF_ROTATION = 64;
const int FLAG_UV_MODEL = 128;
const int FLAG_ALPHA_CHILDREN = 256;
const int FLAG_COLOR_CHILDREN = 512;
const int FLAG_TF_3D = 1024;
const int FLAG_SHADER_ARGS = 2048;
const int FLAG_NO_FCULL = 4096;
const int FLAG_ROUNDED_POS = 8192;

// Language
const int LANG_EN = 0;
const int LANG_JP = 1;

// API
const int API_OPENGL = 0;
const int API_DX11 = 1;

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

// Rotation modes
const int ROT_XYZ = 0;
const int ROT_XZY = 1;
const int ROT_YXZ = 2;
const int ROT_YZX = 3;
const int ROT_ZXY = 4;
const int ROT_ZYX = 5;

// Draw modes
const int MODE_NONE = 0;
const int MODE_RECT = 1;
const int MODE_MODEL = 2;
const int MODE_ELLIPSE = 3;
const int MODE_ARC = 4;
const int MODE_ANIM = 5;
const int MODE_PLANE = 6;
const int MODE_BILLBOARD = 7;

// Color modes
const int COLOR1 = 0;
const int COLOR2 = 1;
const int COLOR_LR = 2;
const int COLOR_UD = 3;

// Blend modes
const int BLEND_ALPHA = 0;
const int BLEND_MULTIPLY = 1;
const int BLEND_ADD = 2;
const int BLEND_SUBTRACT = 3;
const int BLEND_REPLACE = 4;
const int BLEND_SCREEN = 5;
const int BLEND_OVERLAY = 6;
const int BLEND_HSV_COLOR = 7;
const int BLEND_DISPLACE = 8;

// Shader
const int SHADER_2D = 0;
const int SHADER_3D = 1;
const int SHADER_3D_CLIP = 2;
const int SHADER_3D_ANIM = 3;

// Events
const int EVENT_CREATE = 0;
const int EVENT_DELETE = 1;
const int EVENT_ANIM = 2; // If FLAG_ANIM_EVENT is set

// Parent var mask
const int PV_I0 = 1;
const int PV_I1 = 2;
const int PV_I2 = 4;
const int PV_I3 = 8;
const int PV_F0 = 16;
const int PV_F1 = 32;
const int PV_F2 = 64;
const int PV_F3 = 128;
const int PV_F4 = 256;
const int PV_F5 = 512;
const int PV_F6 = 1024;
const int PV_F7 = 2048;
const int PV_STR0 = 4096;
const int PV_STR1 = 8192;
const int PV_STR2 = 16384;
const int PV_STR3 = 32768;

// TTF flags
const int TTF_BOLD = 1;
const int TTF_ITALIC = 2;
const int TTF_UNDERLINE = 4;
const int TTF_STRIKETHROUGH = 8;
const int TTF_OUTLINE_SHIFT = 4;

// Anim flags
const int ANIM_SINGLE = 1; // Plays animation once instead of looping
const int ANIM_ENDED = 2;

// Layers
const int LAYER_3D_BG_1		= 0;
const int LAYER_3D_BG_2		= 1;
const int LAYER_3D_BG_3		= 2;
const int LAYER_3D_BG_4		= 3;
const int LAYER_3D_MAIN		= 4;
const int LAYER_3D_CLIP		= 5;
const int LAYER_3D_1		= 6;
const int LAYER_3D_2		= 7;
const int LAYER_3D_3		= 8;
const int LAYER_3D_4		= 9;
const int LAYER_3D_5		= 10;
const int LAYER_3D_6		= 11;
const int LAYER_3D_OV_1		= 12;
const int LAYER_3D_OV_2		= 13;
const int LAYER_3D_OV_3		= 14;
const int LAYER_3D_OV_4		= 15;
// 2D lowres
const int LAYER_DISTORT		= 16;
const int LAYER_EFF_1		= 17;
const int LAYER_ENEMY_1		= 18;
const int LAYER_ENEMY_2		= 19;
const int LAYER_ENEMY_3		= 20;
const int LAYER_ENEMY_4		= 21;
const int LAYER_ENEMY_5		= 22;
const int LAYER_ENEMY_6		= 23;
const int LAYER_EFF_2		= 24;
const int LAYER_PLAYER_1	= 25;
const int LAYER_PLAYER_2	= 26;
const int LAYER_PLAYER_3	= 27;
const int LAYER_PLAYER_4	= 28;
const int LAYER_ITEM_TEXT   = 29;
// items, lasers and bullets draw here, between 29 and 30
const int LAYER_PLAYER_5    = 30;
const int LAYER_LOWRES_UI	= 31;
// 2D hires
const int LAYER_EFF_HIRES	= 32;
const int LAYER_HIRES_UI	= 33;
const int LAYER_BOSS_UI_1	= 34;
const int LAYER_BOSS_UI_2	= 35;
const int LAYER_DIALOG_1	= 36;
const int LAYER_DIALOG_2	= 37;
const int LAYER_PAUSE_1     = 38;
const int LAYER_PAUSE_2     = 39;
// UI
const int LAYER_UI_BG		= 40;
const int LAYER_UI_1		= 41;
const int LAYER_MENU_1		= 42;
const int LAYER_MENU_2		= 43;
const int LAYER_MENU_3		= 44;
const int LAYER_MENU_4		= 45;
const int LAYER_MENU_5		= 46;
const int LAYER_MENU_6		= 47;


// Control
extern(1) void setDelete();
extern(2) void setStatic();
extern(3) void stop();

extern(4) void layer(int l);
extern(5) void rotMode(int m);
extern(6) void mode(int m);
extern(7) void colorMode(int m);
extern(8) void blendMode(int blend);
extern(9) void points(int np);


// Properties
extern(10) void pos(float x, float y, float z);
extern(11) void anchor(int x, int y);
extern(12) void scale(float x, float y, float z);
extern(13) void rotate(float x, float y, float z);
extern(14) void grow(float x, float y, float z);
extern(15) void wvel(float x, float y, float z);
extern(16) void shader(int shad);
extern(17) void shaderCustom(string vert, string frag);
extern(18) void color(float r, float g, float b);
extern(19) void color2(float r, float g, float b);
extern(20) void alpha(float a);
extern(21) void alpha2(float a);
extern(22) void srcRect(float x, float y, float w, float h);
extern(23) void texture(int slot, string tex);
extern(24) void texturePos(int slot, float x, float y);
extern(25) void textureScale(int slot, float x, float y);
extern(26) void textureScroll(int slot, float x, float y);

// Interpolation
extern(27) void interp(int slot, int target, float time, int mode, float a, float b, float c);
inline void posTime(int slot, float time, int mode, float x, float y, float z) { interp(slot, 1, time, mode, x, y, z); }
inline void scaleTime(int slot, float time, int mode, float x, float y, float z) { interp(slot, 3, time, mode, x, y, z); }
inline void rotateTime(int slot, float time, int mode, float x, float y, float z) { interp(slot, 4, time, mode, x, y, z); }
// TODO grow and wvel
inline void colorTime(int slot, float time, int mode, float r, float g, float b) { interp(slot, 7, time, mode, r, g, b); }
inline void color2Time(int slot, float time, int mode, float r, float g, float b) { interp(slot, 8, time, mode, r, g, b); }
inline void alphaTime(int slot, float time, int mode, float a) { interp(slot, 9, time, mode, a, 0, 0); }
inline void alpha2Time(int slot, float time, int mode, float a) { interp(slot, 10, time, mode, a, 0, 0); }
inline void srcPosTime(int slot, float time, int mode, float x, float y) { interp(slot, 11, time, mode, x, y, 0); }
inline void srcSizeTime(int slot, float time, int mode, float w, float h) { interp(slot, 12, time, mode, w, h, 0); }
inline void texturePosTime(int slot, float time, int mode, int tSlot, float x, float y) { interp(slot, tSlot + 13, time, mode, x, y, 0); }
inline void textureScaleTime(int slot, float time, int mode, int tSlot, float x, float y) { interp(slot, tSlot + 21, time, mode, x, y, 0); }
inline void textureScrollTime(int slot, float time, int mode, int tSlot, float x, float y) { interp(slot, tSlot + 29, time, mode, x, y, 0); }

extern(28) void posBezier(float time, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
extern(29) void model(string name);
extern(30) void childNew(string fn, float x, float y, float z);
extern(31) void childNewRoot(string fn, float x, float y, float z);
extern(32) void getParentVars(int mask);
extern(33) void childEvent(int ev);
extern(34) float normalizeRot(float r);
extern(35) void getTextureSize(int slot, ref int w, ref int h);
extern(36) float modeLerp(float v0, float v1, float t, int mode);

extern(40) void animLoad(string poseFile);
extern(41) void animSet(string anim);
extern(42) void animTime(float time);
extern(43) void animSpeed(float speed);

extern(50) int strChar(ref int pos, string str);
extern(51) void textureTTF(int font, int size, int flags, string text);
extern(52) int ttfWidth(int c);
//extern(52) void textureTTF2(int font, int size, int flags, string text, int maxLen);

inline void noAsync() {
	killAll();
}

inline void colorHex(int col) {
	color((col >> 16 & 0xFF) / 255.0, (col >> 8 & 0xFF) / 255.0, (col & 0xFF) / 255.0);
}
inline void color2Hex(int col) {
	color2((col >> 16 & 0xFF) / 255.0, (col >> 8 & 0xFF) / 255.0, (col & 0xFF) / 255.0);
}

inline void flagSet(int flg) {
	FLAGS |= flg;
}
inline void flagClear(int flg) {
	FLAGS &= ~flg;
}