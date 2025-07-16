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

// Movement (Do not use)
//extern(20) float POS_X;
//extern(21) float POS_Y;
//extern(22) float POS_Z;

extern(23) int DVM_COUNT;

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

extern(1) void pos(float x, float y, float z);
extern(2) void posTime(float time, int mode, float x, float y, float z);
extern(3) void dir(float x, float y, float z);
extern(4) void dirTime(float time, int mode, float x, float y, float z);
extern(5) void up(float x, float y, float z);
extern(6) void upTime(float time, int mode, float x, float y, float z);
extern(7) void fov(float fovy);
extern(8) void fovTime(float time, int mode, float fovy);
extern(9) void posBezier(float time, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
extern(10) void dirBezier(float time, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
extern(11) void dvm(string fn, float x, float y, float z);
extern(12) void dvmDelete(int slot);
extern(13) void dvmEvent(int slot, int ev);
extern(14) void dvmFile(string file);
extern(15) void modelFile(string file);
extern(16) void fogColor(float r, float g, float b);
extern(17) void fogDist(float near, float far);
extern(18) void fogColorTime(float time, int mode, float r, float g, float b);
extern(19) void fogDistTime(float time, int mode, float near, float far);
extern(20) void clearColor(float r, float g, float b);
extern(21) void dvmEventAll(int ev);
extern(22) void dirLight(float x, float y, float z, int ambient, int diffuse, int specular);
extern(23) void dvmRotate(int slot, float rx, float ry, float rz);
extern(24) float modeLerp(float v0, float v1, float t, int mode);

inline void fogColorHex(int c) {
	fogColor(((c >> 16) & 0xFF) / 255.0, ((c >> 8) & 0xFF) / 255.0, (c & 0xFF) / 255.0);
}
inline void fogColorTimeHex(float time, int mode, int c) {
	fogColorTime(time, mode, ((c >> 16) & 0xFF) / 255.0, ((c >> 8) & 0xFF) / 255.0, (c & 0xFF) / 255.0);
}
inline void clearColorHex(int c) {
	clearColor(((c >> 16) & 0xFF) / 255.0, ((c >> 8) & 0xFF) / 255.0, (c & 0xFF) / 255.0);
}

inline void dirLightDefault() {
	dirLight(0, 0, 1, 0x808080, 0, 0);
}

// Rocks smoothly from side to side in the xz plane
async rockingA(int time, float baseAngle, float maxAngle) {
	float r1 = baseAngle + maxAngle;
	float r2 = baseAngle - maxAngle;
	for (int t = 0; t < time; t++) {
		float ft = t;
		float r = modeLerp(baseAngle, r1, ft / time, SMOOTHSTEP);
		up(sin(r), 0, cos(r));
		wait(1);
	}
	loop {
		for (int t = 0; t < time; t++) {
			float ft = t;
			float r = modeLerp(r1, r2, ft / time, SMOOTHSTEP);
			up(sin(r), 0, cos(r));
			wait(1);
		}
		for (int t = 0; t < time; t++) {
			float ft = t;
			float r = modeLerp(r2, r1, ft / time, SMOOTHSTEP);
			up(sin(r), 0, cos(r));
			wait(1);
		}
	}
}