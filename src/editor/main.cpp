#include "GlWindow.h"
#include "screens/ScreenManager.h"
#include "screens/LevelEditorScreen.h"

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 256;

int main() {
	GlWindow window = GlWindow("Level Editor", WINDOW_WIDTH, WINDOW_HEIGHT);
	ScreenManager manager = ScreenManager(window);

	LevelEditorScreen screen = LevelEditorScreen(window, manager);

	manager.setScreen(&screen);
	manager.run();
}