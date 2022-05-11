#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <memory>
#include <utility>
#include <cmath>
#include <thread>
#include <mutex>
#include <queue>
#include <map>
#include <iterator>
#include "scene.hpp"
#include "graphics.hpp"
#include "color.hpp"
#include "character.hpp"
#include "user_interface.hpp"
#include "network_client.hpp"
#include "mouse.hpp"

#define JAUNE	55 / 360.0f
#define MARRON	40 / 360.0f
#define ROUGE	360 / 360.0f
#define VIOLET	275 / 360.0f
#define ROSE	322 / 360.0f
#define BLEU	216 / 360.0f
#define VERT	100 / 360.0f
#define SKIN	340 / 360.0f

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
		MIXTE,
		HERISSON,
		DECOIFFE,
		ARRIERE,
		MECHE_AVANT,
		MI_LONG,
		FRANGE,
		AU_BOL,
		PONYTAIL
	};
	enum class EYES
	{
		MANGA,
		AMANDE,
		EGYPTE,
		MASCARA
	};

	Avatar() :
		m_gender(Avatar::GENDER::MALE),
		m_hair(Avatar::HAIR::HERISSON),
		m_eyes(Avatar::EYES::MANGA),
		m_mouth(Avatar::MOUTH::PETITE),
		m_color{
			JAUNE,
			MARRON,
			ROUGE,
			VIOLET,
			ROSE,
			BLEU,
			VERT,
			SKIN
		},
		m_skin_color_id(7),
		m_hair_color_id(5),
		m_eyes_color_id(5),
		m_shader("shaders/avatar/vertex.glsl", "shaders/avatar/fragment.glsl"),
		m_projection(glm::ortho(0.0f, 512.0f, 0.0f, 512.0f)),
		m_tex{
			createTexture("assets/avatar/sprites/visage.tga", TEXTURE_TYPE::DIFFUSE, true),					// 0
			createTexture("assets/avatar/sprites/cheveux_decoiffe.tga", TEXTURE_TYPE::DIFFUSE, true),		// 1
			createTexture("assets/avatar/sprites/cheveux_arriere_homme.tga", TEXTURE_TYPE::DIFFUSE, true),	// 2
			createTexture("assets/avatar/sprites/cheveux_meche.tga", TEXTURE_TYPE::DIFFUSE, true),			// 3
			createTexture("assets/avatar/sprites/cheveux_herisson.tga", TEXTURE_TYPE::DIFFUSE, true),		// 4
			createTexture("assets/avatar/sprites/cheveux_mixte_front.tga", TEXTURE_TYPE::DIFFUSE, true),	// 5
			createTexture("assets/avatar/sprites/cheveux_mixte_back.tga", TEXTURE_TYPE::DIFFUSE, true),		// 6
			createTexture("assets/avatar/sprites/cheveux_ponytail_front.tga", TEXTURE_TYPE::DIFFUSE, true),	// 7
			createTexture("assets/avatar/sprites/cheveux_ponytail_back.tga", TEXTURE_TYPE::DIFFUSE, true),	// 8
			createTexture("assets/avatar/sprites/cheveux_frange_front.tga", TEXTURE_TYPE::DIFFUSE, true),	// 9
			createTexture("assets/avatar/sprites/cheveux_frange_back.tga", TEXTURE_TYPE::DIFFUSE, true),	// 10
			createTexture("assets/avatar/sprites/cheveux_au_bol_front.tga", TEXTURE_TYPE::DIFFUSE, true),	// 11
			createTexture("assets/avatar/sprites/cheveux_au_bol_back.tga", TEXTURE_TYPE::DIFFUSE, true),	// 12
			createTexture("assets/avatar/sprites/cheveux_mi_long.tga", TEXTURE_TYPE::DIFFUSE, true),		// 13
			createTexture("assets/avatar/sprites/bouche_petite.tga", TEXTURE_TYPE::DIFFUSE, true),			// 14
			createTexture("assets/avatar/sprites/bouche_moyenne.tga", TEXTURE_TYPE::DIFFUSE, true),			// 15
			createTexture("assets/avatar/sprites/bouche_grande.tga", TEXTURE_TYPE::DIFFUSE, true),			// 16
			createTexture("assets/avatar/sprites/yeux_manga_homme.tga", TEXTURE_TYPE::DIFFUSE, true),		// 17
			createTexture("assets/avatar/sprites/visage_light.tga", TEXTURE_TYPE::DIFFUSE, true),			// 18
			createTexture("assets/avatar/sprites/bouche_petite_light.tga", TEXTURE_TYPE::DIFFUSE, true),	// 19
			createTexture("assets/avatar/sprites/bouche_moyenne_light.tga", TEXTURE_TYPE::DIFFUSE, true),	// 20
			createTexture("assets/avatar/sprites/bouche_grande_light.tga", TEXTURE_TYPE::DIFFUSE, true),	// 21
			createTexture("assets/avatar/sprites/yeux_mascara.tga", TEXTURE_TYPE::DIFFUSE, true),			// 22
			createTexture("assets/avatar/sprites/yeux_egypte.tga", TEXTURE_TYPE::DIFFUSE, true),			// 23
			createTexture("assets/avatar/sprites/sourcils_femme.tga", TEXTURE_TYPE::DIFFUSE, true),			// 24
			createTexture("assets/avatar/sprites/sourcils_homme.tga", TEXTURE_TYPE::DIFFUSE, true),			// 25
			createTexture("assets/avatar/sprites/cheveux_mi_long_back.tga", TEXTURE_TYPE::DIFFUSE, true),	// 26
			createTexture("assets/avatar/sprites/yeux_amande.tga", TEXTURE_TYPE::DIFFUSE, true),			// 27
			createTexture("assets/avatar/sprites/sourcils_amande.tga", TEXTURE_TYPE::DIFFUSE, true)			// 28
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

		// draw back hair
		if (m_gender == GENDER::MALE) {
			if (m_hair == HAIR::MIXTE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[6].id);
				m_shader.setFloat("teinte", m_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		else {
			if (m_hair == HAIR::MIXTE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[6].id);
				m_shader.setFloat("teinte", m_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_hair == HAIR::AU_BOL) {
				glBindTexture(GL_TEXTURE_2D, m_tex[12].id);
				m_shader.setFloat("teinte", m_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_hair == HAIR::FRANGE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[10].id);
				m_shader.setFloat("teinte", m_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_hair == HAIR::PONYTAIL) {
				glBindTexture(GL_TEXTURE_2D, m_tex[8].id);
				m_shader.setFloat("teinte", m_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_hair == HAIR::MI_LONG) {
				glBindTexture(GL_TEXTURE_2D, m_tex[26].id);
				m_shader.setFloat("teinte", m_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}

		// draw face
		if (m_skin_color_id == 7) {
			glBindTexture(GL_TEXTURE_2D, m_tex[18].id);
			m_shader.setFloat("teinte", m_color[m_skin_color_id]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, m_tex[0].id);
			m_shader.setFloat("teinte", m_color[m_skin_color_id]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// draw eyebrows
		if (m_gender == GENDER::MALE){
			if (m_eyes == EYES::AMANDE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[28].id);
			}
			else if (m_eyes == EYES::MANGA) {
				glBindTexture(GL_TEXTURE_2D, m_tex[25].id);
			}
		}
		else if (m_gender == GENDER::FEMALE){
			glBindTexture(GL_TEXTURE_2D, m_tex[24].id);
		}
		m_shader.setFloat("teinte", m_color[m_skin_color_id]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// draw eyes
		if (m_gender == GENDER::MALE) {
			if (m_eyes == EYES::MANGA) {
				glBindTexture(GL_TEXTURE_2D, m_tex[17].id);
			}
			else if (m_eyes == EYES::AMANDE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[27].id);
			}
		}
		else if (m_gender == GENDER::FEMALE) {
			if (m_eyes == EYES::MANGA) {
				glBindTexture(GL_TEXTURE_2D, m_tex[17].id);
			}
			else if (m_eyes == EYES::MASCARA) {
				glBindTexture(GL_TEXTURE_2D, m_tex[22].id);
			}
			else if (m_eyes == EYES::EGYPTE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[23].id);
			}
		}
		m_shader.setFloat("teinte", m_color[m_eyes_color_id]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// draw mouth
		if (m_skin_color_id == 7) {
			if (m_mouth == MOUTH::PETITE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[19].id);
				m_shader.setFloat("teinte", m_color[m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_mouth == MOUTH::MOYENNE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[20].id);
				m_shader.setFloat("teinte", m_color[m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_mouth == MOUTH::GRANDE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[21].id);
				m_shader.setFloat("teinte", m_color[m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		else {
			if (m_mouth == MOUTH::PETITE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[14].id);
				m_shader.setFloat("teinte", m_color[m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_mouth == MOUTH::MOYENNE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[15].id);
				m_shader.setFloat("teinte", m_color[m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_mouth == MOUTH::GRANDE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[16].id);
				m_shader.setFloat("teinte", m_color[m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		
		// draw hair
		if (m_gender == GENDER::MALE) {
			if (m_hair == HAIR::MIXTE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[5].id);
			}
			else if (m_hair == HAIR::ARRIERE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[2].id);
			}
			else if (m_hair == HAIR::DECOIFFE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[1].id);
			}
			else if (m_hair == HAIR::MECHE_AVANT) {
				glBindTexture(GL_TEXTURE_2D, m_tex[3].id);
			}
			else if (m_hair == HAIR::HERISSON) {
				glBindTexture(GL_TEXTURE_2D, m_tex[4].id);
			}
		}
		else {
			if (m_hair == HAIR::MIXTE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[5].id);
			}
			else if (m_hair == HAIR::AU_BOL) {
				glBindTexture(GL_TEXTURE_2D, m_tex[11].id);
			}
			else if (m_hair == HAIR::FRANGE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[9].id);
			}
			else if (m_hair == HAIR::PONYTAIL) {
				glBindTexture(GL_TEXTURE_2D, m_tex[7].id);
			}
			else if (m_hair == HAIR::MI_LONG) {
				glBindTexture(GL_TEXTURE_2D, m_tex[13].id);
			}
		}
		m_shader.setFloat("teinte", m_color[m_hair_color_id]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
	}

	GENDER m_gender; // 0 = male, 1 = female
	HAIR m_hair;
	EYES m_eyes;
	MOUTH m_mouth;
	std::array<float, 8> m_color;
	int m_skin_color_id;
	int m_hair_color_id;
	int m_eyes_color_id;
	Shader m_shader;
	glm::mat4 m_projection;
	std::array<Texture, 29> m_tex;
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
	Cursor(int screenW, int screenH) :
		m_pos(0),
		m_focus(2),
		m_blink(0.0f),
		m_blink_ctrl(false),
		m_projection(glm::ortho(0.0f, static_cast<float>(screenW), 0.0f, static_cast<float>(screenH))),
		m_shader("shaders/cursor/vertex.glsl", "shaders/cursor/fragment.glsl")
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		float data[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	
	void draw(glm::vec3 cursor_shape, float delta)
	{
		// blink
		m_blink += delta;
		if (m_blink >= 1.0f) {
			m_blink = 0.0f;
			m_blink_ctrl = false;
		}
		else if (m_blink >= 0.5f) {
			m_blink_ctrl = true;
		}

		// create geometry data
		float data[4] = {
			cursor_shape.x, cursor_shape.y,
			cursor_shape.x, cursor_shape.y + cursor_shape.z
		};
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float), data);

		// draw
		glBindVertexArray(m_vao);
		m_shader.use();
		m_shader.setMatrix("proj", m_projection);
		m_shader.setBool("blink", m_blink_ctrl);
		glDrawArrays(GL_LINE_STRIP, 0, 2);
	}

	int m_focus; // 0 = pseudo, 1 = chat, 2 = not writing
	int m_pos;
	float m_blink;
	bool m_blink_ctrl;
	Shader m_shader;
	glm::mat4 m_projection;
	GLuint m_vao;
	GLuint m_vbo;
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
		Writer(int screenW, int screenH) :
			m_cursor(screenW, screenH)
		{
			m_deltaWrite = 0.0f;
			m_lastWriteAction = WRITE_ACTION::NOTHING;
			m_lastCharacter = "";
		}

		void write(char* c, std::bitset<10>& userInputs, float delta, int boundX, glm::vec3 cursor_shape);
		
		std::array<std::string, 2> m_textInput;
		std::array<std::vector<int>, 2> m_textSectionsWidth;
		Cursor m_cursor;
		float m_deltaWrite;
		WRITE_ACTION m_lastWriteAction;
		std::string m_lastCharacter;
	
	private:
		void write_aux(WRITE_ACTION writeAction, std::string& character, float delta, int boundX, glm::vec3 cursor_shape);
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
		void updateUI(std::bitset<10> & inputs, char* text_input, int screenW, int screenH, float delta);
		void swap_gender_features(Avatar::GENDER from, Avatar::GENDER to);

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

inline std::queue<std::string> g_msg2server_queue;
inline std::queue<std::string> g_msg2client_queue;
inline std::mutex g_msg2server_mutex;
inline std::mutex g_msg2client_mutex;
inline bool g_connected{ false };
inline std::mutex g_connected_mutex;
inline bool g_try_connection{ false };
inline bool g_search_opponent{ false };

#endif
