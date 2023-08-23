// #define MUTE

#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#include <AL/al.h>
#include <AL/alc.h>
#include <stb/stb_image.h>
#include <stb/stb_vorbis.c>

#include <iostream>

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