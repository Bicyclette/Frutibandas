#ifndef GAME_HPP
#define GAME_HPP

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

class Game
{
	public:

		Game(int clientWidth, int clientHeight);
		void draw(float& delta, double& elapsedTime, int width, int height, DRAWING_MODE mode = DRAWING_MODE::SOLID, bool debug = false, bool debugPhysics = false);
		void resizeScreen(int clientWidth, int clientHeight);
		void updateSceneActiveCameraView(int index, const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta);
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
        std::unique_ptr<Text> textRenderer;
        std::vector<SpriteGroup> sprite_group;

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

	private:

		const std::string card_name[12] = {
			"assets/cartes/enclume.tga",
			"assets/cartes/celerite.tga",
			"assets/cartes/confiscation.tga",
			"assets/cartes/renfort.tga",
			"assets/cartes/desordre.tga",
			"assets/cartes/petrification.tga",
			"assets/cartes/vachette.tga",
			"assets/cartes/conversion.tga",
			"assets/cartes/charge.tga",
			"assets/cartes/entracte.tga",
			"assets/cartes/solo.tga",
			"assets/cartes/piege.tga",
		};

		const std::string card_description[12] = {
			"Détruit définitivement une case du plateau et tout ce qui se trouve dessus.",
			"Le joueur a le droit de faire 2 mouvements au lieu d'un seul",
			"Si l'adversaire joue une carte au prochain tour, vous capturez cette carte.\nAttention, si l'adversaire ne joue aucune carte, la confiscation est perdue.",
			"3 nouveaux bandas rejoignent votre équipe sur des cases vides",
			"Inverse la direction jouée par l'adversiare au prochain tour",
			"Transforme un bandas en pierre bloquante. La pierre est détruite si la case sur\nlaquelle elle se trouve est détruite.",
			"Court sur une colonne du plateau et élimine tous les bandas touchés.",
			"Un bandas change de couleur et d'équipe.",
			"Vos bandas se déplaceront de 2 cases au lieu d'1 ce tour-ci.",
			"Vous passez un tour.",
			"Choisissez un bandas de votre équipe et il sera le seul à se déplacer pendant\nla phase de mouvement.",
			"La case libre ciblée est piégée. Si un bandas marche dessus, lui et la case seront\ndétruits. Votre adversaire ne voit pas le piège avant qu'un bandas n'y mette le pied dessus."
		};

		// -1 if no card on the slot
		// positive numbers are the index to fetch data in the arrays "card_name" and "card_description"
		int card_slot[4][2] = { 
			{-1,-1},
			{-1,-1},
			{-1,-1},
			{-1,-1},
		};

		// -1 : destroyed
		//  0 : orange
		//  1 : banane
		//  2 : empty
		int board[8][8] = {
			{2,2,2,2,2,2,2,2},
			{2,2,2,2,2,2,2,2},
			{2,2,2,2,2,2,2,2},
			{2,2,2,2,2,2,2,2},
			{2,2,2,2,2,2,2,2},
			{2,2,2,2,2,2,2,2},
			{2,2,2,2,2,2,2,2},
			{2,2,2,2,2,2,2,2}
		};

	private:

		NetworkClient m_client;
};

#endif
