#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>

#include "screens/ScreenManager.h"
#include "screens/PlayLevelScreen.h"
#include "screens/LevelEditorScreen.h"
#include "GlWindow.h"

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 256;

int main() {
	try {
		GlWindow window = GlWindow("Level Editor", WINDOW_WIDTH, WINDOW_HEIGHT);
		ScreenManager manager = ScreenManager(window);

		PlayLevelScreen playScreen = PlayLevelScreen(window, manager);
		LevelEditorScreen editorScreen = LevelEditorScreen(window, manager, playScreen);
		playScreen.setEditorScreen(&editorScreen);

		manager.setScreen(&editorScreen);
		manager.run();
	} catch (const char* err) {
		std::cerr << err;
	}
}