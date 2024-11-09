import "dvm";

void sdmModel(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(4);
		shader(SHADER_3D);
		mode(MODE_MODEL);
		model(STR0);
		texture(0, STR1);
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}

void bgSDM1Hall(int event) {
	if (!event) {
		layer(4);

		STR0 = "SDM1~1";
		STR1 = "tex/bg/St1b.png";
		childNew("sdmModel", 0, 0, 0);

		STR0 = "SDM1~2";
		STR1 = "tex/bg/St1c.png";
		childNew("sdmModel", 0, 0, 0);

		STR0 = "SDM1~3";
		STR1 = "tex/bg/St1a2.png";
		childNew("sdmModel", 0, 0, 0);

		STR0 = "SDM1";
		STR1 = "tex/bg/St1a.png";
	}
	sdmModel(event);
}