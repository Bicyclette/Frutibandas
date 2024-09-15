#ifndef GAME_HPP
#define GAME_HPP

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <cmath>
#include <thread>
#include <map>
#include <iterator>
#include "scene.hpp"
#include "graphics.hpp"
#include "color.hpp"
#include "user_interface.hpp"
#include "helpers.hpp"
#include "bandas.hpp"

class Game
{
	public:

		Game(int clientWidth, int clientHeight);
		~Game();
		void draw(float& delta, double& elapsedTime, int width, int height, DRAWING_MODE mode = DRAWING_MODE::SOLID, bool debug = false, bool debugPhysics = false);
		void sound_manager();
		void resizeScreen(int clientWidth, int clientHeight);
		Graphics& getGraphics();
		std::vector<Scene> & getScenes();
		int getActiveScene();
		void setActiveScene(int index);
		void updateSceneActiveCameraView(int index, const std::bitset<10> inputs, std::array<int, 3> mouse, float delta);

	private:

		int activeScene;
		std::vector<Scene> scenes;
		Graphics graphics;
    
    private:

		void drawUI(float& delta, double& elapsedTime, int width, int height, DRAWING_MODE mode = DRAWING_MODE::SOLID);

	private: // render passes
		void compositingPass();
		inline void sceneCompositing();
		inline void uiCompositing();

	public:

		Bandas m_bandas;
};

#endif
