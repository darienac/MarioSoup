#include "screens/ScreenManager.h"
#include "screens/PlayLevelScreen.h"
#include "screens/LevelEditorScreen.h"
#include "GlWindow.h"

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 256;

int main() {
	GlWindow window = GlWindow("Level Editor", WINDOW_WIDTH, WINDOW_HEIGHT);
	ScreenManager manager = ScreenManager(window);

	PlayLevelScreen playScreen = PlayLevelScreen(window, manager);
	LevelEditorScreen editorScreen = LevelEditorScreen(window, manager, playScreen);
	playScreen.setEditorScreen(&editorScreen);

	manager.setScreen(&editorScreen);
	manager.run();
}