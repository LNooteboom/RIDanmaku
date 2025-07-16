import "dialog";

void st02a() {
	skippable(0);
	playerShow("dialogPl00");
	speakerPlayer();
	textboxShow();
	wait(30);
	skippable(1);

	text("foo");
	text("bar");
	next(600);

	danContinue();
	bossShow("dialogSt02Bs");
	bossFace(0);
	speakerBoss();
	text("This is dialog text!");
	text("Hello, World!");
	next(600);

	bossFace(3);
	text("quux");
	next(600);

	textboxHide();
	wait(30);
	end();
}