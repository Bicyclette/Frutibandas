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
#include "mouse.hpp"
#include "tinyfiledialogs.hpp"

#define MARRON_CLAIR_HUE 0.04f
#define MARRON_CLAIR_SAT 1.0f

#define VIOLET_HUE 0.736f
#define VIOLET_SAT 1.0f

#define ROSE_HUE 0.868f
#define ROSE_SAT 0.73f

#define JAUNE_HUE 0.08f
#define JAUNE_SAT 1.0f

#define BLEU_HUE 0.666f
#define BLEU_SAT 1.0f

#define ROSE_FLASH_HUE 0.85f
#define ROSE_FLASH_SAT 1.0f

#define LBLEU_HUE 0.427f
#define LBLEU_SAT 1.0f

#define ROUGE_HUE 1.0f
#define ROUGE_SAT 1.0f

#define VERT_HUE 0.333f
#define VERT_SAT 1.0f

struct Avatar
{
	enum class GENDER
	{
		MALE,
		FEMALE
	};
	enum class MOUTH
	{
		PETITE,
		MOYENNE,
		GRANDE
	};
	enum class HAIR
	{
		MANGA,
		HERISSON,
		DECOIFFE,
		ARRIERE,
		MECHE_AVANT,
		MI_LONG,
		FRANGE,
		AU_BOL,
		RAIDE
	};
	enum class EYES
	{
		MANGA,
		AMANDE,
		GROS,
		EGYPTE,
		MASCARA
	};

	Avatar() :
		m_gender(Avatar::GENDER::MALE),
		m_hair(Avatar::HAIR::DECOIFFE),
		m_eyes(Avatar::EYES::MANGA),
		m_mouth(Avatar::MOUTH::MOYENNE),
		m_skin_hue{ MARRON_CLAIR_HUE, JAUNE_HUE, ROSE_HUE, ROSE_FLASH_HUE, LBLEU_HUE, BLEU_HUE, VIOLET_HUE, ROUGE_HUE, VERT_HUE},
		m_skin_saturation{ MARRON_CLAIR_SAT, JAUNE_SAT, ROSE_SAT, ROSE_FLASH_SAT, LBLEU_SAT, BLEU_SAT, VIOLET_SAT, ROUGE_SAT, VERT_SAT },
		m_hair_hue{ MARRON_CLAIR_HUE, JAUNE_HUE, ROSE_HUE, ROSE_FLASH_HUE, LBLEU_HUE, BLEU_HUE, VIOLET_HUE, ROUGE_HUE, VERT_HUE },
		m_hair_saturation{ MARRON_CLAIR_SAT, JAUNE_SAT, ROSE_SAT, ROSE_FLASH_SAT, LBLEU_SAT, BLEU_SAT, VIOLET_SAT, ROUGE_SAT, VERT_SAT },
		m_eyes_hue{ MARRON_CLAIR_HUE, JAUNE_HUE, ROSE_HUE, ROSE_FLASH_HUE, LBLEU_HUE, BLEU_HUE, VIOLET_HUE, ROUGE_HUE, VERT_HUE },
		m_eyes_saturation{ MARRON_CLAIR_SAT, JAUNE_SAT, ROSE_SAT, ROSE_FLASH_SAT, LBLEU_SAT, BLEU_SAT, VIOLET_SAT, ROUGE_SAT, VERT_SAT },
		m_skin_color_id(0),
		m_hair_color_id(0),
		m_eyes_color_id(0),
		m_shader("shaders/avatar/vertex.glsl", "shaders/avatar/fragment.glsl"),
		m_projection(glm::ortho(0.0f, 512.0f, 0.0f, 512.0f)),
		m_tex{
			createTexture("assets/avatar/sprites/visage.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/avatar/sprites/cheveux_manga_homme.tga", TEXTURE_TYPE::DIFFUSE, true),
			createTexture("assets/avatar/sprites/yeux_manga_homme.tga", TEXTURE_TYPE::DIFFUSE, true)
		}
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glm::vec2 m_pos(0.0f, 0.0f);
		glm::vec2 m_size(512.0f, 512.0f);
		float data[24] = {
			m_pos.x, m_pos.y + m_size.y, 0.0f, 1.0f,
			m_pos.x, m_pos.y, 0.0f, 0.0f,
			m_pos.x + m_size.x, m_pos.y, 1.0f, 0.0f,
			m_pos.x, m_pos.y + m_size.y, 0.0f, 1.0f,
			m_pos.x + m_size.x, m_pos.y, 1.0f, 0.0f,
			m_pos.x + m_size.x, m_pos.y + m_size.y, 1.0f, 1.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	void draw()
	{
		glBindVertexArray(m_vao);
		m_shader.use();
		m_shader.setMatrix("proj", m_projection);
		glActiveTexture(GL_TEXTURE0);
		m_shader.setInt("image", 0);

		// draw face
		glBindTexture(GL_TEXTURE_2D, m_tex[0].id);
		m_shader.setFloat("teinte", m_skin_hue[m_skin_color_id]);
		m_shader.setFloat("saturation", m_skin_saturation[m_skin_color_id]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// draw eyes
		glBindTexture(GL_TEXTURE_2D, m_tex[2].id);
		m_shader.setFloat("teinte", m_eyes_hue[m_eyes_color_id]);
		m_shader.setFloat("saturation", m_eyes_saturation[m_skin_color_id]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// draw mouth

		// draw hair
		glBindTexture(GL_TEXTURE_2D, m_tex[1].id);
		m_shader.setFloat("teinte", m_hair_hue[m_hair_color_id]);
		m_shader.setFloat("saturation", m_hair_saturation[m_skin_color_id]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
	}

	GENDER m_gender; // 0 = male, 1 = female
	HAIR m_hair;
	EYES m_eyes;
	MOUTH m_mouth;
	std::array<float, 9> m_skin_hue;
	std::array<float, 9> m_skin_saturation;
	std::array<float, 9> m_hair_hue;
	std::array<float, 9> m_hair_saturation;
	std::array<float, 9> m_eyes_hue;
	std::array<float, 9> m_eyes_saturation;
	int m_skin_color_id;
	int m_hair_color_id;
	int m_eyes_color_id;
	Shader m_shader;
	glm::mat4 m_projection;
	std::array<Texture, 3> m_tex; // 20 quand j'aurais toutes les textures
	GLuint m_vao;
	GLuint m_vbo;
};

struct Cards
{
	Cards(int screenW, int screenH) :
		m_sprite_orange{
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH)
		},
		m_sprite_banane{
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH),
			Sprite(0, glm::vec2(0), glm::vec2(0), screenW, screenH)
		}
	{

	}

	const std::string m_name[13] = {
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
			"assets/cartes/verso.tga",
	};

	const std::string m_description[13] = {
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
		"La case libre ciblée est piégée. Si un bandas marche dessus, lui et la case seront\ndétruits. Votre adversaire ne voit pas le piège avant qu'un bandas n'y mette le pied dessus.",
		"Carte adverse."
	};

	std::array<Sprite, 8> m_sprite_orange;
	std::array<Sprite, 8> m_sprite_banane;

	// -1 if no card on the slot
	// positive numbers are the index to fetch data in the arrays "name" and "description"
	int m_slot[2][4][2] = {
		{
			{-1,-1},
			{-1,-1},
			{-1,-1},
			{-1,-1},
		},
		{
			{-1,-1},
			{-1,-1},
			{-1,-1},
			{-1,-1},
		} 
	};

	void draw()
	{
		for (int i{ 0 }; i < 2; ++i)
		{
			for (int j{ 0 }; j < 4; ++j)
			{
				for (int k{ 0 }; k < 2; ++k)
				{
					int index = m_slot[i][j][k];
					if (i == 0)
						m_sprite_orange[index].draw();
				}
			}
		}
	}
};

struct Board
{
	Board(int screenW, int screenH, float screen_aspect_ratio) :
		m_tileDim(49, 49 * screen_aspect_ratio),
		m_orange_draw_start(0.22f * screenW + 2 * m_tileDim.x, screenW - 2 * m_tileDim.y),
		m_banane_draw_start(0.22f * screenW + 2 * m_tileDim.x, screenW - 1.5f * m_tileDim.y),
		m_orange(std::make_unique<Sprite>(0, m_orange_draw_start, m_tileDim, screenW, screenH)),
		m_banane(std::make_unique < Sprite>(1, m_banane_draw_start, glm::vec2(m_tileDim.x, m_tileDim.y * 1.5f), screenW, screenH)),
		m_floor1(std::make_unique < Sprite>(2, m_orange_draw_start, m_tileDim, screenW, screenH)),
		m_floor2(std::make_unique < Sprite>(3, m_orange_draw_start - glm::vec2(0.0f, 7 * m_tileDim.y), m_tileDim, screenW, screenH))
	{}
	// -1 : destroyed
	//  0 : orange
	//  1 : banane
	//  2 : empty
	int m_data[8][8] = {
		{2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2}
	};
	const glm::vec2 m_tileDim;
	const glm::vec2 m_orange_draw_start;
	const glm::vec2 m_banane_draw_start;
	std::unique_ptr<Sprite> m_orange;
	std::unique_ptr<Sprite> m_banane;
	std::unique_ptr<Sprite> m_floor1;
	std::unique_ptr<Sprite> m_floor2;
	void draw()
	{

	}
};

struct Cursor
{
	int m_focus; // 0 = pseudo, 1 = chat, 2 = not writing
	int m_pos;
};

enum class WRITE_ACTION
{
	ERASE,
	CURSOR_LEFT,
	CURSOR_RIGHT,
	CHARACTER,
	NOTHING
};

class Writer
{
	public:
		Writer()
		{
			m_cursor.m_focus = 2;
			m_cursor.m_pos = 0;
			m_deltaWrite = 0.0f;
			m_lastWriteAction = WRITE_ACTION::NOTHING;
			m_lastCharacter = '~';
		}

		void write(std::bitset<52>& writeInput, float delta);
		
		std::array<std::string, 2> m_textInput;
		const int m_textInputMaxCapacity[2] = { 20, 30 };
		Cursor m_cursor;
		float m_deltaWrite;
		WRITE_ACTION m_lastWriteAction;
		char m_lastCharacter;
	
	private:
		void write_aux(WRITE_ACTION writeAction, char c, float delta);
};

class Game
{
	public:

		Game(int clientWidth, int clientHeight);
		void createUI(int screenW, int screenH);
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
		int getCursorFocus();
		Writer& get_writer();
		void updateUI(const std::bitset<10> & inputs, int screenW, int screenH, float delta);
		void swap_gender_features(Avatar::GENDER from, Avatar::GENDER to);

	private:

		int activeScene;
		int activeVehicle;
		std::vector<Scene> scenes;
		std::vector<WorldPhysics> worldPhysics;
		std::map<std::string, std::shared_ptr<Object>> loadedAssets;
		Graphics graphics;
		std::shared_ptr<Character> character;
		NetworkClient m_client;
    
    private:

		void drawUI(float& delta, double& elapsedTime, int width, int height, DRAWING_MODE mode = DRAWING_MODE::SOLID);
        std::unique_ptr<Text> textRenderer;
		std::unique_ptr<Mouse> m_mouse;

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

		Avatar m_avatar;
		Writer m_writer;
		Cards m_cards;
		Board m_board;
		UI m_ui;
		int m_fruit; // 0 => orange, 1 => banane, -1 => undefined
		std::string m_pseudo_orange;
		std::string m_pseudo_banane;
};

#endif
