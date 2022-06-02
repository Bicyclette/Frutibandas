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
#include "character.hpp"
#include "user_interface.hpp"
#include "network_client.hpp"
#include "helpers.hpp"
#include "bandas.hpp"

class Game
{
	public:

		Game(int clientWidth, int clientHeight);
		~Game();
		void draw(float& delta, double& elapsedTime, int width, int height, DRAWING_MODE mode = DRAWING_MODE::SOLID, bool debug = false, bool debugPhysics = false);
		void resizeScreen(int clientWidth, int clientHeight);
		void updateSceneActiveCameraView(int index, const std::bitset<10> & inputs, std::array<int, 3> & mouse, float delta);
		Graphics& getGraphics();
		std::vector<Scene> & getScenes();
		int getActiveScene();
		void setActiveScene(int index);
		int getActiveVehicle();
		void setActiveVehicle(int index);
		void setCharacterScene(int index);
		int getCharacterScene();
		void setCharacter(std::string filePath, glm::mat4 aModel, std::string aName, int index, glm::ivec2 scrDim);
		void removeCharacter();
		void characterDoActionWalk(Character::DIRECTION d, float delta);
		void characterDoActionRun(Character::DIRECTION d, float delta);
		void characterDoActionJump(bool forward, float delta);
		void characterDoActionIdle();
		void vehicleDrive(bool forward);
		void vehicleDriveReset();
		void vehicleSteering(VEHICLE_STEERING dir);
		void vehicleSteeringReset();
		void vehicleSetWheelTransform();
		void vehicleUpdateUpVector();

	private:

		int activeScene;
		int activeVehicle;
		std::vector<Scene> scenes;
		std::vector<WorldPhysics> worldPhysics;
		std::map<std::string, std::shared_ptr<Object>> loadedAssets;
		Graphics graphics;
		std::shared_ptr<Character> character;
    
    private:

		void drawUI(float& delta, double& elapsedTime, int width, int height, DRAWING_MODE mode = DRAWING_MODE::SOLID);

	private: // render passes

		void directionalShadowPass(int index, float delta, DRAWING_MODE mode = DRAWING_MODE::SOLID);
		void omnidirectionalShadowPass(int index, float delta, DRAWING_MODE mode = DRAWING_MODE::SOLID);
		void bloomPass(int width, int height, std::unique_ptr<Framebuffer> & in, int attachmentIndex, GLuint out);
		void GBufferPass(int index, int width, int height, float delta);
        void ssaoPass(int index, int width, int height, float delta);
		void colorMultisamplePass(int index, int width, int height, float delta, DRAWING_MODE mode = DRAWING_MODE::SOLID, bool debug = false);
		void volumetricsPass(int index, int width, int height, float delta, double elapsedTime);
		void motionBlurPass(int index, int width, int height);
		void compositingPass();
		inline void sceneCompositing();
		inline void uiCompositing();

	public:

		Bandas m_bandas;
};

#endif
