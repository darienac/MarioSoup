#include "screens/ScreenManager.h"
#include "screens/PlayLevelScreen.h"
#include "GlWindow.h"

const int WINDOW_WIDTH = 256;
const int WINDOW_HEIGHT = 240;

int main() {
	try {
		GlWindow window = GlWindow("Mario Soup", WINDOW_WIDTH, WINDOW_HEIGHT);
		ScreenManager manager = ScreenManager(window);

		PlayLevelScreen playScreen = PlayLevelScreen(window, manager);
		playScreen.loadLevel("demo.mwf");

		manager.setScreen(&playScreen);
		manager.run();
	} catch (const char* err) {
		std::cerr << err;
	}
}