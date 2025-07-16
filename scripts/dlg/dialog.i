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

extern(0) void end();
extern(1) void playerShow(string dvm);
extern(2) void playerHide();
extern(3) void playerFace(int face);
extern(4) void playerEffect(int eff);
extern(5) void bossShow(string dvm);
extern(6) void bossHide();
extern(7) void bossFace(int face);
extern(8) void bossEffect(int eff);
extern(9) void textboxShow();
extern(10) void textboxHide();
extern(11) void speakerPlayer();
extern(12) void speakerBoss();
extern(13) void speakerNone();
extern(14) void intro(string dvm);
extern(15) void skippable(int skip);
extern(16) void textY(int y);
extern(17) void text(string txt);
extern(18) void next(int time);
extern(19) void bgmStop();
extern(20) void bgm(int trackIdx);
extern(21) void sfx(int s);
extern(22) void danContinue();