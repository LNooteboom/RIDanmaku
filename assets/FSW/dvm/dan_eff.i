import "dvm";

void effFocus(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PLAYER_5);
		texture(0, "tex/dan/Effects.png");
		srcRect(64, 64, 64, 64);
		wvel(0, 0, rad(2));
		alpha(0);
		alphaTime(0, 8, LINEAR, 1);
		scale(2, 2, 1);
		scaleTime(1, 8, EASEOUT1, 1, 1, 1);
		stop();
	case EVENT_DELETE:
		alphaTime(0, 8, LINEAR, 0);
		scaleTime(1, 8, EASEIN1, 2, 2, 1);
		wait(8);
		setDelete();
	}
}
void effFocus2(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_EFF_2);
		texture(0, "tex/dan/Effects.png");
		srcRect(64, 64, 64, 64);
		wvel(0, 0, rad(-1));
		alpha(0);
		alphaTime(0, 8, LINEAR, 0.5);
		scale(2, 2, 1);
		scaleTime(1, 8, EASEOUT1, 1, 1, 1);
		stop();
	case EVENT_DELETE:
		alphaTime(0, 8, LINEAR, 0);
		scaleTime(1, 8, EASEIN1, 2, 2, 1);
		wait(8);
		setDelete();
	}
}

const int SPELL_DECLARE_TIME = 80;
void spellDecl1(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_OV_4);
		texture(0, "tex/dan/Effects2.png");
		srcRect(96 + 16, 0, -16, -128 * 5);
		pos(192 - 64, 448 - 64, 0);

		mode(MODE_ARC);
		points(6);
		F0 = 0;
		F1 = 2 * PI;
		//F2 = 112;
		F3 = 20;
		rotateTime(0, SPELL_DECLARE_TIME, LINEAR, 0, 0, F4);
		alpha(0);
		alphaTime(1, 20, LINEAR, 0.5);
		wait(SPELL_DECLARE_TIME - 20);
		//stop();
	case EVENT_DELETE:
		alphaTime(1, 20, LINEAR, 0);
		wait(20);
		setDelete();
	}
}
void spellDecl2(int event) {
	const float angle = rad(-20);
	const float x = 128;
	const float y = x * 0.363970; // tan(rad(20))
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_OV_4);
		texture(0, "tex/dan/Effects2.png");
		srcRect(96, 0, 16, 128 * 4);
		rotate(0, 0, rad(90) + angle);

		alpha(0);
		wait(F1);

		if (I0) {
			pos(x, F0 - y, 0);
			posTime(0, SPELL_DECLARE_TIME, EASEOUTIN1, -x, F0 + y, 0);
		} else {
			pos(-x, F0 + y, 0);
			posTime(0, SPELL_DECLARE_TIME, EASEOUTIN1, x, F0 - y, 0);
		}

		alphaTime(1, 20, LINEAR, 0.5);
		wait(SPELL_DECLARE_TIME - 20);
		//stop();
	case EVENT_DELETE:
		alphaTime(1, 20, LINEAR, 0);
		wait(20);
		setDelete();
	}
}

void effSpellDeclare(int event) {
	switch (event) {
	case EVENT_CREATE:
		F2 = 96;
		F4 = rad(1) * SPELL_DECLARE_TIME;
		childNewRoot("spellDecl1", 0, 0, 0);
		F2 = 144;
		F4 = -F4;
		childNewRoot("spellDecl1", 0, 0, 0);

		F0 = 16;
		F1 = 0;
		for (int i = 5; i; --i) {
			I0 = 0;
			childNewRoot("spellDecl2", 0, 0, 0);
			F0 += 32;
			F1 += 4;

			I0 = 1;
			childNewRoot("spellDecl2", 0, 0, 0);
			F0 += 32;
			F1 += 4;
		}
		

		wait(SPELL_DECLARE_TIME);
	case EVENT_DELETE:
		setDelete();
	}
}

void effSpellCircleGraphic(int event) {
	switch (event) {
	case EVENT_CREATE:
		F4 = F0; // Duration
		F5 = 128 + F3; // Start size
		F6 = 32 + F3; // End size
		mode(MODE_ARC);
		//layer(LAYER_3D_OV_4);
		texture(0, "tex/dan/Effects2.png");
		srcRect(F1, 0, 16, 512);
		wvel(0, 0, F2);

		points(17);
		F0 = 0;
		F1 = 2 * PI;
		//F2 = 128;
		F3 = 16;

		//scaleTime(0, F4, LINEAR, 0.3, 0.3, 1);
		alpha(0);
		alphaTime(1, 60, LINEAR, 0.5);
		scale(0, 0, 1);
		scaleTime(0, 60, EASEOUT2, 1, 1, 1);
		{
			float inc = 1.0f / F4;
			for (float f = 0; f < 1; f += inc) {
				F2 = lerp(F5, F6, f);
				wait(1);
			}
		}
		
	case EVENT_DELETE:
		noAsync();
		alphaTime(1, 20, LINEAR, 0);
		wait(20);
		setDelete();
	}
}
void effSpellCircle(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_OV_4);
		F1 = 16;
		F2 = rad(2);
		F3 = 16;
		childNew("effSpellCircleGraphic", 0, 0, 0);
		F1 = 32;
		F2 = rad(-2);
		F3 = 0;
		childNew("effSpellCircleGraphic", 0, 0, 0);
		stop();
	case EVENT_DELETE:
		wait(20);
		setDelete();
	}
}

void effDistort(int event) {
	if (API == API_OPENGL) {
		switch (event) {
		case EVENT_CREATE:
			layer(LAYER_DISTORT);
			texture(0, "@0");
			texture(1, "tex/noise/Noise1.png");
			srcRect(192, 0, F0, F0);
			//shaderCustom(0);

			alpha(0);
			alphaTime(0, 20, LINEAR, 1);

			scale(0, 0, 1);
			scaleTime(2, 20, EASEOUT1, 1, 1, 1);

			colorTime(1, 20, LINEAR, ((I0 >> 16) & 0xFF) / 255.0, ((I0 >> 8) & 0xFF) / 255.0, (I0 & 0xFF) / 255.0);
			alpha2(0);
			colorMode(2);
			mode(MODE_ELLIPSE);
			points(8);

			FLAGS |= FLAG_UV_MODEL;
			{
				int w = 0;
				int h = 0;
				getTextureSize(0, w, h);
				texturePos(0, 0, 448.0 / h);
				textureScale(0, 1, -1);
				textureScale(1, w * 0.006, h * 0.006);
				textureScroll(1, 0.01, 0.005);
			}

			stop();
		case EVENT_DELETE:
			noAsync();
			alphaTime(0, 20, LINEAR, 0);
			scaleTime(2, 20, EASEIN1, 0, 0, 1);
			wait(20);
			setDelete();
		}
	} else {
		switch (event) {
		case EVENT_CREATE:
			layer(LAYER_DISTORT);
			mode(MODE_MODEL);
			model("DistortGrid");
			texture(0, "@0");
			FLAGS |= FLAG_SHADER_ARGS | FLAG_NO_FCULL;
			shaderCustom("shaders/distort_vs.cso", "shaders/std_ps_post_3d.cso");

			//I0 = 160; // radius
			//I1 = 0xFF0000; // color
			I2 = 0;
			F4 = 0;
			F5 = 0;

			loop {
				if (I2 <= 30) {
					F3 = modeLerp(0, I0, I2 / 30.0, EASEOUT1);
					I2++;
				}

				// scale
				float s = F3 + 20;
				//scale(s, s, 1);

				// UV
				float s2 = s * 2;
				int texW;
				int texH;
				getTextureSize(0, texW, texH);
				float h = s2 / texH;
				textureScale(0, s2 / texW, -h);
				texturePos(0, (POS_X + 192 - s) / texW, (POS_Y - s) / texH + h);

				F0 = 1.0 - ((I1 >> 16) & 0xFF) / 255.0; // r
				F1 = 1.0 - ((I1 >> 8) & 0xFF) / 255.0; // g
				F2 = 1.0 - (I1 & 0xFF) / 255.0; // b
				//F3 = I0; // radius

				F4 = normalizeRot(F4 + PI / 16); // x ripple
				F5 = normalizeRot(F5 + PI / 32); // y ripple

				wait(1);
			}
		case EVENT_DELETE:
			wait(20);
			setDelete();
		}
	}
}

void effHex(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_EFF_1);
		texture(0, "tex/dan/Effects3.png");
		srcRect(0, 0, 256, 256);
		wvel(0, 0, rad(12));
		//blendMode(BLEND_ADD);

		alpha(0);
		alphaTime(0, 30, LINEAR, 0.8);
		scale(0, 0, 1);
		scaleTime(1, 30, EASEOUT1, 1, 1, 1);
		wait(30);

		while (1) {
			const float t = 40;
			scaleTime(1, t, SMOOTHSTEP, 0.8, 0.8, 1);
			wait(t);
			scaleTime(1, t, SMOOTHSTEP, 1.0, 1.0, 1);
			wait(t);
		}
	case EVENT_DELETE:
		noAsync();
		alphaTime(0, 20, LINEAR, 0);
		wait(20);
		setDelete();
	}
}

void effGraze(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_EFF_2);
		texture(0, "tex/dan/Effects.png");
		srcRect(0, 16, 16, 16);
		blendMode(BLEND_ADD);
		colorHex(0xffab6b);

		rotate(0, 0, RRAD);
		scale(0.7, 0.7, 1);
		F0 = RRAD;
		F1 = cos(F0) * 64;
		F2 = sin(F0) * 64;
		posTime(0, 12, EASEOUT1, F1, F2, 0);
		scaleTime(1, 12, EASEIN1, 0, 0, 0);
		wait(12);
	case EVENT_DELETE:
		setDelete();
	}
}

void effChargeCircle(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/dan/Effects.png");
		srcRect(1, 33, 30, 30);
		color(((I0 >> 16) & 0xFF) / 255.0, ((I0 >> 8) & 0xFF) / 255.0, (I0 & 0xFF) / 255.0);
		scale(16, 16, 1);
		scaleTime(0, 60, LINEAR, 0, 0, 0);
		alpha(0);
		alphaTime(1, 20, LINEAR, 1);
		wait(60);
	case EVENT_DELETE:
		setDelete();
	}
}
void effChargeParticle(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/dan/Effects.png");
		srcRect(0, 128, 32, 32);
		color(((I0 >> 16) & 0xFF) / 255.0, ((I0 >> 8) & 0xFF) / 255.0, (I0 & 0xFF) / 255.0);

		rotate(0, 0, RRAD);
		wvel(0, 0, RF2 * rad(10));

		scale(0, 0, 0);
		scaleTime(0, 20, EASEOUT1, 2, 2, 2);
		F0 = RRAD;
		F3 = 256 + 64 * RF2;
		F1 = cos(F0) * F3;
		F2 = sin(F0) * F3;
		pos(F1, F2, 0);
		posTime(1, 30, EASEOUT1, 0, 0, 0);
		wait(10);
		alphaTime(2, 20, LINEAR, 0);
		wait(20);
	case EVENT_DELETE:
		setDelete();
	}
}

void effChargeColor(int event, int color) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_EFF_2);
		I0 = color;
		childNew("effChargeCircle", 0, 0, 0);
		for (int i = 1; i; --i) {
			childNew("effChargeParticle", 0, 0, 0);
		}
		for (int i = 50; i; --i) {
			childNew("effChargeParticle", 0, 0, 0);
			wait(1);
		}
		wait(30);
	case EVENT_DELETE:
		setDelete();
	}
}

void effChargeRed(int event) { effChargeColor(event, 0xFF8080); }
void effChargeMagenta(int event) { effChargeColor(event, 0xFF80FF); }
void effChargeBlue(int event) { effChargeColor(event, 0x8080FF); }
void effChargeCyan(int event) { effChargeColor(event, 0x80FFFF); }
void effChargeGreen(int event) { effChargeColor(event, 0x80FF80); }
void effChargeYellow(int event) { effChargeColor(event, 0xFFFF80); }

void effExplodeCircle(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/dan/Effects.png");
		srcRect(I0, I1, 64, 64);
		blendMode(BLEND_ADD);

		scale(0.2, 0.2, 1);
		scaleTime(1, 30, EASEOUT1, 2.5, 2.5, 1);
		alphaTime(0, 30, EASEOUT1, 0);
		wait(30);
	case EVENT_DELETE:
		setDelete();
	}
}

void effExplodeCircle2(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/dan/Effects.png");
		srcRect(I0, I1, 64, 64);
		blendMode(BLEND_ADD);

		rotate(0, 0, RRAD);
		scale(0.5, 0.5, 1);
		scaleTime(1, 30, EASEOUT1, 3.5, 0.2, 1);
		alphaTime(0, 30, EASEOUT1, 0);
		wait(30);
	case EVENT_DELETE:
		setDelete();
	}
}

void effExplodeParticle(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/dan/Effects.png");
		srcRect(0, 128, 32, 32);
		blendMode(BLEND_ADD);
		alpha(0.25);

		I0 = 20 + RI % 15;
		F0 = 0.5 + RF * 1;
		scale(F0, F0, 1);
		rotate(0, 0, RRAD);
		wvel(0, 0, RF2 * rad(90));

		F0 = RRAD;
		F1 = RF * 256;
		F2 = cos(F0) * F1;
		F3 = sin(F0) * F1;
		wait(5);
		posTime(0, I0, EASEOUT1, F2, F3, 0);
		scaleTime(1, I0, EASEOUT1, 0, 0, 1);
		wait(I0);
	case EVENT_DELETE:
		setDelete();
	}
}

void effExplodeColor(int event, int srcX, int srcY) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_EFF_1);
		I0 = srcX;
		I1 = srcY;
		loop (8) {
			childNew("effExplodeParticle", 0, 0, 0);
		}
		childNew("effExplodeCircle", 0, 0, 0);
		childNew("effExplodeCircle2", 0, 0, 0);
		wait(40);
	case EVENT_DELETE:
		setDelete();
	}
}

void effExplodeRed(int event) {
	effExplodeColor(event, 128, 0);
}
void effExplodeBlue(int event) {
	effExplodeColor(event, 192, 0);
}
void effExplodeGreen(int event) {
	effExplodeColor(event, 128, 64);
}
void effExplodeYellow(int event) {
	effExplodeColor(event, 192, 64);
}

void effExplodeInvertCircle(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/White.png");
		srcRect(0, 0, I0, I0);
		mode(MODE_ELLIPSE);
		points(16);
		blendMode(BLEND_SCREEN);

		pos(F0, F1, 0);
		posTime(0, 60, LINEAR, F0 / 2, F1 / 2, 0);

		scale(1, 1, 1);
		scaleTime(1, 60, EASEIN2, F2, F2, 1);

		wait(60);
	case EVENT_DELETE:
		setDelete();
	}
}
void effExplodeInvert(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_EFF_HIRES);

		{
			string ch = "effExplodeInvertCircle";
			I0 = 256;
			F0 = 0;
			F1 = 0;
			F2 = 1;
			childNew(ch, 0, 0, 0);
			
			float v = 32;
			I0 = 192;
			F0 = -v;
			F1 = 0;
			F2 = 1;
			childNew(ch, 0, 0, 0);
			F0 = v;
			childNew(ch, 0, 0, 0);
			F0 = 0;
			F1 = -v;
			childNew(ch, 0, 0, 0);
			F1 = v;
			childNew(ch, 0, 0, 0);

			F1 = 0;
			F2 = 4;
			I0 = 8;
			childNew(ch, 0, 0, 0);
		}

		scale(0, 0, 1);
		scaleTime(0, 60, EASEIN2, 32, 32, 1);

		wait(60);
	case EVENT_DELETE:
		setDelete();
	}
}

void spellBgTestSub(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/bg/testbg.jpg");
		colorHex(0x794a80);
		srcRect(0, 0, 512, 766);
		alpha(0);
		alphaTime(0, 40, LINEAR, 0.2);
		while (1) {
			srcRect(F0, 0, 512, 766);
			F0 -= 1;
			wait(1);
		}
		stop();
	case EVENT_DELETE:
		noAsync();
		alphaTime(0, 20, LINEAR, 0);
		wait(20);
		setDelete();
	}
}
void spellBgTest(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_3D_OV_1);
		texture(0, "tex/bg/testbg.jpg");
		colorHex(0x392545);
		srcRect(0, 0, 512, 766);
		pos(0, 224, 0);
		alpha(0);
		alphaTime(0, 40, LINEAR, 1);
		childNew("spellBgTestSub", 0, 0, 0);
		while (1) {
			srcRect(F0, F0, 512, 766);
			F0 += 1;
			wait(1);
		}
		stop();
	case EVENT_DELETE:
		noAsync();
		alphaTime(0, 20, LINEAR, 0);
		wait(20);
		setDelete();
	}
}