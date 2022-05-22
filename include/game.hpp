#ifndef GAME_HPP
#define GAME_HPP

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <cmath>
#include <thread>
#include <mutex>
#include <queue>
#include <map>
#include <iterator>
#include <sstream>
#include "scene.hpp"
#include "graphics.hpp"
#include "color.hpp"
#include "character.hpp"
#include "user_interface.hpp"
#include "network_client.hpp"
#include "mouse.hpp"
#include "helpers.hpp"

enum class MOVE
{
	UNDEFINED,
	UP,
	DOWN,
	RIGHT,
	LEFT
};

// eye colors
#define JAUNE	55 / 360.0f
#define MARRON	40 / 360.0f
#define ROUGE	360 / 360.0f
#define VIOLET	275 / 360.0f
#define ROSE	322 / 360.0f
#define BLEU	216 / 360.0f
#define VERT	100 / 360.0f

// skin colors
#define PALE1 glm::vec3(0.988f, 0.863f, 0.847f)
#define PALE2 glm::vec3(0.984f, 0.784f, 0.745f)
#define PALE3 glm::vec3(0.98f, 0.706f, 0.643f)
#define MARRON_CLAIR1 glm::vec3(0.902f, 0.608f, 0.314f)
#define MARRON_CLAIR2 glm::vec3(0.843f, 0.49f, 0.235f)
#define MARRON_CLAIR3 glm::vec3(0.784f, 0.392f, 0.157f)
#define MARRON1 glm::vec3(0.627f, 0.392f, 0.176f)
#define MARRON2 glm::vec3(0.541f, 0.341f, 0.145f)
#define MARRON3 glm::vec3(0.424f, 0.267f, 0.118f)
#define MARRON4 glm::vec3(0.294f, 0.188f, 0.078f)

// hair colors
#define H_ROUGE glm::vec3(0.946f, 0.046f, 0.05f)
#define H_ORANGE glm::vec3(1.0f, 0.546f, 0.0f)
#define H_JAUNE1 glm::vec3(1.0f, 0.851f, 0.0f)
#define H_JAUNE2 glm::vec3(0.901f, 0.704f, 0.011f)
#define H_VERT1 glm::vec3(0.097f, 0.932f, 0.023f)
#define H_VERT2 glm::vec3(0.004f, 0.586f, 0.004f)
#define H_BLEU1 glm::vec3(0.219f, 0.917f, 1.0f)
#define H_BLEU2 glm::vec3(0.0f, 0.339f, 1.0f)
#define H_BLEU3 glm::vec3(0.317f, 0.152f, 0.925f)
#define H_POURPRE glm::vec3(0.423f, 0.050f, 0.431f)
#define H_VIOLET glm::vec3(1.0f, 0.0f, 0.838f)
#define H_ROSE glm::vec3(0.964f, 0.447f, 0.88f)
#define H_ROSE_BONBON glm::vec3(1.0f, 0.078f, 0.529f)

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
		GROS,
		EGYPTE,
		MASCARA
	};

	Avatar() :
		m_gender(Avatar::GENDER::MALE),
		m_hair(Avatar::HAIR::HERISSON),
		m_eyes(Avatar::EYES::MANGA),
		m_mouth(Avatar::MOUTH::MOYENNE),
		m_eyes_color{
			JAUNE,
			MARRON,
			ROUGE,
			VIOLET,
			ROSE,
			BLEU,
			VERT
		},
		m_skin_color{
			PALE1,
			PALE2,
			PALE3,
			MARRON_CLAIR1,
			MARRON_CLAIR2,
			MARRON_CLAIR3,
			MARRON1,
			MARRON2,
			MARRON3,
			MARRON4,
			H_ROUGE,
			H_ORANGE,
			H_JAUNE1,
			H_JAUNE2,
			H_VERT1,
			H_VERT2,
			H_BLEU1,
			H_BLEU2,
			H_BLEU3,
			H_POURPRE,
			H_VIOLET,
			H_ROSE,
			H_ROSE_BONBON
		},
		m_hair_color{
		H_ROUGE,
		H_ORANGE,
		H_JAUNE1,
		H_JAUNE2,
		H_VERT1,
		H_VERT2,
		H_BLEU1,
		H_BLEU2,
		H_BLEU3,
		H_POURPRE,
		H_VIOLET,
		H_ROSE,
		H_ROSE_BONBON
		},
		m_skin_color_id(3),
		m_hair_color_id(3),
		m_eyes_color_id(0),
		m_shaderHSL("shaders/avatar/HSL/vertex.glsl", "shaders/avatar/HSL/fragment.glsl"),
		m_shaderRGB("shaders/avatar/RGB/vertex.glsl", "shaders/avatar/RGB/fragment.glsl"),
		m_projection(glm::ortho(0.0f, 512.0f, 0.0f, 512.0f)),
		m_tex{
			createTexture("assets/avatar/sprites/bouche_petite.tga", TEXTURE_TYPE::DIFFUSE, true),					// 0
			createTexture("assets/avatar/sprites/bouche_moyenne.tga", TEXTURE_TYPE::DIFFUSE, true),					// 1
			createTexture("assets/avatar/sprites/bouche_grande.tga", TEXTURE_TYPE::DIFFUSE, true),					// 2
			createTexture("assets/avatar/sprites/yeux_manga.tga", TEXTURE_TYPE::DIFFUSE, true),						// 3
			createTexture("assets/avatar/sprites/yeux_amande.tga", TEXTURE_TYPE::DIFFUSE, true),					// 4
			createTexture("assets/avatar/sprites/yeux_gros.tga", TEXTURE_TYPE::DIFFUSE, true),						// 5
			createTexture("assets/avatar/sprites/yeux_egypte.tga", TEXTURE_TYPE::DIFFUSE, true),					// 6
			createTexture("assets/avatar/sprites/yeux_mascara.tga", TEXTURE_TYPE::DIFFUSE, true),					// 7
			createTexture("assets/avatar/sprites/cheveux_mixte_front.tga", TEXTURE_TYPE::DIFFUSE, true),			// 8
			createTexture("assets/avatar/sprites/cheveux_mixte_back.tga", TEXTURE_TYPE::DIFFUSE, true),				// 9
			createTexture("assets/avatar/sprites/cheveux_herisson.tga", TEXTURE_TYPE::DIFFUSE, true),				// 10
			createTexture("assets/avatar/sprites/cheveux_arriere.tga", TEXTURE_TYPE::DIFFUSE, true),				// 11
			createTexture("assets/avatar/sprites/cheveux_meche_avant.tga", TEXTURE_TYPE::DIFFUSE, true),			// 12
			createTexture("assets/avatar/sprites/cheveux_decoiffe.tga", TEXTURE_TYPE::DIFFUSE, true),				// 13
			createTexture("assets/avatar/sprites/cheveux_mi_long_front.tga", TEXTURE_TYPE::DIFFUSE, true),			// 14
			createTexture("assets/avatar/sprites/cheveux_mi_long_back.tga", TEXTURE_TYPE::DIFFUSE, true),			// 15
			createTexture("assets/avatar/sprites/cheveux_frange_front.tga", TEXTURE_TYPE::DIFFUSE, true),			// 16
			createTexture("assets/avatar/sprites/cheveux_frange_back.tga", TEXTURE_TYPE::DIFFUSE, true),			// 17
			createTexture("assets/avatar/sprites/cheveux_ponytail_front.tga", TEXTURE_TYPE::DIFFUSE, true),			// 18
			createTexture("assets/avatar/sprites/cheveux_ponytail_back.tga", TEXTURE_TYPE::DIFFUSE, true),			// 19
			createTexture("assets/avatar/sprites/cheveux_au_bol_front.tga", TEXTURE_TYPE::DIFFUSE, true),			// 20
			createTexture("assets/avatar/sprites/cheveux_au_bol_back.tga", TEXTURE_TYPE::DIFFUSE, true)				// 21
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

	void draw(bool mirrorX)
	{
		glBindVertexArray(m_vao);
		glActiveTexture(GL_TEXTURE0);
		
		m_shaderHSL.use();
		m_shaderHSL.setBool("mirrorX", mirrorX);
		m_shaderHSL.setMatrix("proj", m_projection);
		m_shaderHSL.setInt("image", 0);
		
		m_shaderRGB.use();
		m_shaderRGB.setBool("mirrorX", mirrorX);
		m_shaderRGB.setMatrix("proj", m_projection);
		m_shaderRGB.setInt("image", 0);

		// draw back hair
		m_shaderRGB.use();
		if (m_gender == GENDER::MALE) {
			if (m_hair == HAIR::MIXTE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[9].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		else {
			if (m_hair == HAIR::MIXTE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[9].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_hair == HAIR::AU_BOL) {
				glBindTexture(GL_TEXTURE_2D, m_tex[21].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_hair == HAIR::FRANGE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[17].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_hair == HAIR::PONYTAIL) {
				glBindTexture(GL_TEXTURE_2D, m_tex[19].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_hair == HAIR::MI_LONG) {
				glBindTexture(GL_TEXTURE_2D, m_tex[15].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}

		// draw face and mouth
		m_shaderRGB.use();
		if (m_mouth == MOUTH::PETITE) {
			glBindTexture(GL_TEXTURE_2D, m_tex[0].id);
			m_shaderRGB.setVec3f("color", m_skin_color[m_skin_color_id]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (m_mouth == MOUTH::MOYENNE) {
			glBindTexture(GL_TEXTURE_2D, m_tex[1].id);
			m_shaderRGB.setVec3f("color", m_skin_color[m_skin_color_id]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (m_mouth == MOUTH::GRANDE) {
			glBindTexture(GL_TEXTURE_2D, m_tex[2].id);
			m_shaderRGB.setVec3f("color", m_skin_color[m_skin_color_id]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// draw eyes
		m_shaderHSL.use();
		if (m_gender == GENDER::MALE) {
			if (m_eyes == EYES::MANGA) {
				glBindTexture(GL_TEXTURE_2D, m_tex[3].id);
			}
			else if (m_eyes == EYES::AMANDE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[4].id);
			}
			else if (m_eyes == EYES::GROS) {
				glBindTexture(GL_TEXTURE_2D, m_tex[5].id);
			}
		}
		else if (m_gender == GENDER::FEMALE) {
			if (m_eyes == EYES::MASCARA) {
				glBindTexture(GL_TEXTURE_2D, m_tex[7].id);
			}
			else if (m_eyes == EYES::EGYPTE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[6].id);
			}
		}
		m_shaderHSL.setFloat("teinte", m_eyes_color[m_eyes_color_id]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// draw hair
		m_shaderRGB.use();
		if (m_gender == GENDER::MALE) {
			if (m_hair == HAIR::MIXTE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[8].id);
			}
			else if (m_hair == HAIR::ARRIERE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[11].id);
			}
			else if (m_hair == HAIR::DECOIFFE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[13].id);
			}
			else if (m_hair == HAIR::MECHE_AVANT) {
				glBindTexture(GL_TEXTURE_2D, m_tex[12].id);
			}
			else if (m_hair == HAIR::HERISSON) {
				glBindTexture(GL_TEXTURE_2D, m_tex[10].id);
			}
		}
		else {
			if (m_hair == HAIR::MIXTE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[8].id);
			}
			else if (m_hair == HAIR::AU_BOL) {
				glBindTexture(GL_TEXTURE_2D, m_tex[20].id);
			}
			else if (m_hair == HAIR::FRANGE) {
				glBindTexture(GL_TEXTURE_2D, m_tex[16].id);
			}
			else if (m_hair == HAIR::PONYTAIL) {
				glBindTexture(GL_TEXTURE_2D, m_tex[18].id);
			}
			else if (m_hair == HAIR::MI_LONG) {
				glBindTexture(GL_TEXTURE_2D, m_tex[14].id);
			}
		}
		m_shaderRGB.setVec3f("color", m_hair_color[m_hair_color_id]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
	}

	void cleanup()
	{
		for (auto& tex : m_tex)
		{
			if (tex.id != -1) {
				glDeleteTextures(1, &tex.id);
			}
		}
	}

	GENDER m_gender; // 0 = male, 1 = female
	HAIR m_hair;
	EYES m_eyes;
	MOUTH m_mouth;
	std::array<float, 7> m_eyes_color;
	std::array<glm::vec3, 23> m_skin_color;
	std::array<glm::vec3, 13> m_hair_color;
	int m_skin_color_id;
	int m_hair_color_id;
	int m_eyes_color_id;
	Shader m_shaderRGB;
	Shader m_shaderHSL;
	glm::mat4 m_projection;
	std::array<Texture, 22> m_tex;
	GLuint m_vao;
	GLuint m_vbo;
};

struct Cards
{
	const std::array<Texture, 12> m_description = {
		createTexture("assets/cartes/enclume_desc.tga", TEXTURE_TYPE::DIFFUSE, true),			// 0
		createTexture("assets/cartes/celerite_desc.tga", TEXTURE_TYPE::DIFFUSE, true),			// 1
		createTexture("assets/cartes/confiscation_desc.tga", TEXTURE_TYPE::DIFFUSE, true),		// 2
		createTexture("assets/cartes/renfort_desc.tga", TEXTURE_TYPE::DIFFUSE, true),			// 3
		createTexture("assets/cartes/desordre_desc.tga", TEXTURE_TYPE::DIFFUSE, true),			// 4
		createTexture("assets/cartes/petrification_desc.tga", TEXTURE_TYPE::DIFFUSE, true),		// 5
		createTexture("assets/cartes/vachette_desc.tga", TEXTURE_TYPE::DIFFUSE, true),			// 6
		createTexture("assets/cartes/conversion_desc.tga", TEXTURE_TYPE::DIFFUSE, true),		// 7
		createTexture("assets/cartes/charge_desc.tga", TEXTURE_TYPE::DIFFUSE, true),			// 8
		createTexture("assets/cartes/entracte_desc.tga", TEXTURE_TYPE::DIFFUSE, true),			// 9
		createTexture("assets/cartes/solo_desc.tga", TEXTURE_TYPE::DIFFUSE, true),				// 10
		createTexture("assets/cartes/piege_desc.tga", TEXTURE_TYPE::DIFFUSE, true)				// 11
	};

	const std::array<Texture, 13> m_tex = {
		createTexture("assets/cartes/enclume.tga", TEXTURE_TYPE::DIFFUSE, true),		// 0
		createTexture("assets/cartes/celerite.tga", TEXTURE_TYPE::DIFFUSE, true),		// 1
		createTexture("assets/cartes/confiscation.tga", TEXTURE_TYPE::DIFFUSE, true),	// 2
		createTexture("assets/cartes/renfort.tga", TEXTURE_TYPE::DIFFUSE, true),		// 3
		createTexture("assets/cartes/desordre.tga", TEXTURE_TYPE::DIFFUSE, true),		// 4
		createTexture("assets/cartes/petrification.tga", TEXTURE_TYPE::DIFFUSE, true),	// 5
		createTexture("assets/cartes/vachette.tga", TEXTURE_TYPE::DIFFUSE, true),		// 6
		createTexture("assets/cartes/conversion.tga", TEXTURE_TYPE::DIFFUSE, true),		// 7
		createTexture("assets/cartes/charge.tga", TEXTURE_TYPE::DIFFUSE, true),			// 8
		createTexture("assets/cartes/entracte.tga", TEXTURE_TYPE::DIFFUSE, true),		// 9
		createTexture("assets/cartes/solo.tga", TEXTURE_TYPE::DIFFUSE, true),			// 10
		createTexture("assets/cartes/piege.tga", TEXTURE_TYPE::DIFFUSE, true),			// 11
		createTexture("assets/cartes/verso.tga", TEXTURE_TYPE::DIFFUSE, true)			// 12
	};

	// -1 if no card on the slot
	// positive numbers are the index to fetch data in the arrays "m_tex" and "description"
	int m_slot[16] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	std::array<std::unique_ptr<Sprite>, 16> m_sprite;

	Cards()
	{
		m_sprite[0] = std::make_unique<Sprite>(100, glm::vec2(19, 728 - 197 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[0]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[1] = std::make_unique<Sprite>(101, glm::vec2(125, 728 - 197 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[1]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[2] = std::make_unique<Sprite>(102, glm::vec2(19, 728 - 320 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[2]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[3] = std::make_unique<Sprite>(103, glm::vec2(125, 728 - 320 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[3]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[4] = std::make_unique<Sprite>(104, glm::vec2(19, 728 - 444 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[4]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[5] = std::make_unique<Sprite>(105, glm::vec2(125, 728 - 444 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[5]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[6] = std::make_unique<Sprite>(106, glm::vec2(19, 728 - 568 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[6]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[7] = std::make_unique<Sprite>(107, glm::vec2(125, 728 - 568 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[7]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		
		m_sprite[8] = std::make_unique<Sprite>(200, glm::vec2(839, 728 - 197 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[8]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[9] = std::make_unique<Sprite>(201, glm::vec2(944, 728 - 197 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[9]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[10] = std::make_unique<Sprite>(202, glm::vec2(839, 728 - 320 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[10]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[11] = std::make_unique<Sprite>(203, glm::vec2(944, 728 - 320 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[11]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[12] = std::make_unique<Sprite>(204, glm::vec2(839, 728 - 444 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[12]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[13] = std::make_unique<Sprite>(205, glm::vec2(944, 728 - 444 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[13]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[14] = std::make_unique<Sprite>(206, glm::vec2(839, 728 - 568 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[14]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
		m_sprite[15] = std::make_unique<Sprite>(207, glm::vec2(944, 728 - 568 - 117), glm::vec2(84, 117), 1050, 728);
		m_sprite[15]->set_background_color(glm::vec4(0.835f, 0.843f, 0.533f, 1.0f));
	}

	bool hovered_card(int mouseX, int mouseY, int& card_id)
	{
		bool hovered{ false };
		for (auto& sprite : m_sprite)
		{
			if (!sprite->is_selectable())
				continue;
			glm::vec2 pos{ sprite->get_position() };
			glm::vec2 size{ sprite->get_size() };
			if (mouseX >= pos.x && mouseX <= (pos.x + size.x) && mouseY >= pos.y && mouseY <= (pos.y + size.y)) {
				card_id = sprite->get_id();
				hovered = true;
			}
		}
		return hovered;
	}

	void draw()
	{
		for (int i{ 0 }; i < m_sprite.size(); ++i){
			int slot = m_slot[i];
			if (slot != -1)
			{
				m_sprite[i]->set_background_img_gl(m_tex[slot].id);
				m_sprite[i]->use_background_img_gl();
			}
			else
			{
				m_sprite[i]->use_background_color();
			}
			m_sprite[i]->draw();
		}
	}

	void cleanup()
	{
		for (auto& tex : m_tex)
		{
			if (tex.id != -1) {
				glDeleteTextures(1, &tex.id);
			}
		}
		for (auto& tex : m_description)
		{
			if (tex.id != -1) {
				glDeleteTextures(1, &tex.id);
			}
		}
	}
};

struct Fruit
{
	// >>>>>>>>>> methods
	Fruit() : m_type(-1), m_petrified(false), m_animationTimer(0.0f) {}
	// >>>>>>>>>> properties
	int m_type;	// -1 = none, 0 = orange, 1 = banane
	bool m_petrified;
	float m_animationTimer;
};

struct Tile
{
	// >>>>>>>>>> methods
	Tile() : m_alive(true), m_dying(false), m_trap(false), m_animationTimer(0.0f) {}
	// >>>>>>>>>> properties
	bool m_alive;
	bool m_dying;
	bool m_trap;
	float m_animationTimer;
};

struct Board
{
	Board() :
		m_square(0, glm::vec2(0), glm::vec2(49), 1050, 728),
		m_rect(0, glm::vec2(0), glm::vec2(49, 55), 1050, 728),
		m_orange(0, glm::vec2(0), glm::vec2(245), 1050, 728),
		m_banane(0, glm::vec2(0), glm::vec2(245, 370), 1050, 728),
		boundLeft(0),
		boundRight(7),
		boundTop(0),
		boundBottom(7),
		m_dyingTimer(0.0f)
	{}

	void print()
	{
		for (int line{ 0 }; line < 8; ++line)
		{
			for (int col{ 0 }; col < 8; ++col)
			{
				int t = m_fruit[line][col].m_type;
				if (t > -1) {
					std::cout << ' ' << m_fruit[line][col].m_type << ' ';

				}
				else {
					std::cout << m_fruit[line][col].m_type << ' ';
				}
			}
			std::cout << std::endl;
		}
	}

	int orange_count()
	{
		int count{ 0 };
		for (int i{ 0 }; i < 8; ++i) {
			for (int j{ 0 }; j < 8; ++j) {
				count += (m_fruit[j][i].m_type == 0) ? 1 : 0;
			}
		}
		return count;
	}

	int banane_count()
	{
		int count{ 0 };
		for (int i{ 0 }; i < 8; ++i) {
			for (int j{ 0 }; j < 8; ++j) {
				count += (m_fruit[j][i].m_type == 1) ? 1 : 0;
			}
		}
		return count;
	}

	void draw_tiles(float delta = 0.0f)
	{
		if (m_dyingTimer < 0.0f)
		{
			m_dyingTimer += delta;
			if (m_dyingTimer >= 0.0f) {
				// set dying tiles to dead
				// reset animation timers to zero
				for (int i{ 0 }; i < 8; ++i) {
					for (int j{ 0 }; j < 8; ++j) {
						if (m_tile[i][j].m_dying) {
							m_tile[i][j].m_dying = false;
							m_tile[i][j].m_alive = false;
							m_tile[i][j].m_animationTimer = 0.0f;
						}
					}
				}
				m_dyingTimer = 0.0f;
			}
		}

		glm::vec2 start(525-(49*4), 645);
		for (int i{ 0 }; i <= 7; ++i) {
			for (int j{ 0 }; j <= 7; ++j) {
				if (!m_tile[j][i].m_alive) {
					continue;
				}
				glm::vec2 shift(49 * i, -49 * j);
				if (j == boundBottom) {	// bottom line
					if(m_tile[j][i].m_alive && !m_tile[j][i].m_dying) {
						m_rect.set_background_img_gl(m_tex[3].id);
						m_rect.use_background_img_gl();
						m_rect.set_pos(start + shift);
						m_rect.draw();
					}
					else if(m_tile[j][i].m_alive && m_tile[j][i].m_dying) {
						int index;
						if (m_tile[j][i].m_animationTimer <= 0.0f) {
							index = 2;
						}
						else if (m_tile[j][i].m_animationTimer >= 0.0f && m_tile[j][i].m_animationTimer <= 0.25f) {
							index = static_cast<int>(4.0f + (m_tile[j][i].m_animationTimer * 4.0f) * 5.0f);
						}
						else {
							index = 10;
						}
						m_tile[j][i].m_animationTimer += delta;
						m_square.set_background_img_gl(m_tex[index].id);
						m_square.use_background_img_gl();
						m_square.set_pos(start + shift);
						m_square.draw();
					}
				}
				else {					// elsewhere
					if(m_tile[j][i].m_alive && !m_tile[j][i].m_dying)
					{
						m_square.set_background_img_gl(m_tex[2].id);
					}
					else if (m_tile[j][i].m_alive && m_tile[j][i].m_dying) {
						int index;
						if (m_tile[j][i].m_animationTimer <= 0.0f) {
							index = 2;
						}
						else if (m_tile[j][i].m_animationTimer >= 0.0f && m_tile[j][i].m_animationTimer <= 0.25f) {
							index = static_cast<int>(4.0f + (m_tile[j][i].m_animationTimer * 4.0f) * 5.0f);
						}
						else {
							index = 10;
						}
						m_tile[j][i].m_animationTimer += delta;
						m_square.set_background_img_gl(m_tex[index].id);
					}
					m_square.use_background_img_gl();
					m_square.set_pos(start + shift);
					m_square.draw();
				}
			}
		}
	}

	void draw_fruits(MOVE move = MOVE::UNDEFINED, float delta = 0.0f)
	{
		//glm::vec2 start_orange(525 - (49 * 4), 645);
		//glm::vec2 start_banane(525 - (49 * 4), 645 + 24);
		glm::vec2 start_orange(525 - (49 * 3) - 245 * 0.5f - 24.0f, 645 + (245*0.5f) - 24.0f);
		glm::vec2 start_banane(525 - (49 * 3) - 245 * 0.5f - 24.0f, 645 + (24 + 370 * 0.5f) - 36.0f);
		for (int i{ 0 }; i < 8; ++i) {
			for (int j{ 0 }; j < 8; ++j) {
				glm::vec2 shift(49 * i, -49 * j);

				if (move == MOVE::UP) {
					m_fruit[j][i].m_animationTimer += delta;
					if (m_fruit[j][i].m_animationTimer >= 0 && m_fruit[j][i].m_animationTimer <= 0.25f) {
						shift += (m_fruit[j][i].m_animationTimer * 4.0f) * glm::vec2(0, 49);
					}
					else if (m_fruit[j][i].m_animationTimer >= 0.25f) {
						shift += glm::vec2(0, 49);
					}
				}
				else if (move == MOVE::DOWN) {
					m_fruit[j][i].m_animationTimer += delta;
					if (m_fruit[j][i].m_animationTimer >= 0 && m_fruit[j][i].m_animationTimer <= 0.25f) {
						shift += (m_fruit[j][i].m_animationTimer * 4.0f) * glm::vec2(0, -49);
					}
					else if (m_fruit[j][i].m_animationTimer >= 0.25f) {
						shift += glm::vec2(0, -49);
					}
				}
				else if (move == MOVE::RIGHT) {
					m_fruit[j][i].m_animationTimer += delta;
					if (m_fruit[j][i].m_animationTimer >= 0 && m_fruit[j][i].m_animationTimer <= 0.25f) {
						shift += (m_fruit[j][i].m_animationTimer * 4.0f) * glm::vec2(49, 0);
					}
					else if (m_fruit[j][i].m_animationTimer >= 0.25f) {
						shift += glm::vec2(49, 0);
					}
				}
				else if (move == MOVE::LEFT) {
					m_fruit[j][i].m_animationTimer += delta;
					if (m_fruit[j][i].m_animationTimer >= 0 && m_fruit[j][i].m_animationTimer <= 0.25f) {
						shift += (m_fruit[j][i].m_animationTimer * 4.0f) * glm::vec2(-49, 0);
					}
					else if (m_fruit[j][i].m_animationTimer >= 0.25f) {
						shift += glm::vec2(-49, 0);
					}
				}

				if (m_fruit[j][i].m_type == 0)
				{
					m_orange.set_background_img_gl(m_tex[0].id);
					m_orange.use_background_img_gl();
					m_orange.set_pos(start_orange + shift);
					m_orange.draw();
				}
				else if (m_fruit[j][i].m_type == 1)
				{
					m_banane.set_background_img_gl(m_tex[1].id);
					m_banane.use_background_img_gl();
					m_banane.set_pos(start_banane + shift);
					m_banane.draw();
				}
			}
		}
	}

	void update_up(int fruit, bool invTeam)
	{
		if (invTeam) {
			if (fruit == 0) {
				fruit = 1;
			}
			else {
				fruit = 0;
			}
		}
		int enemy = (fruit == 0) ? 1 : 0;
		for (int line{ boundTop }; line <= boundBottom; ++line)
		{
			for (int col{ boundLeft }; col <= boundRight; ++col)
			{
				if (m_fruit[line][col].m_type == enemy) {
					//check if there is a pusher down the column
					int l{ line };
					while (m_fruit[l][col].m_type == enemy && l <= boundBottom) {
						l++;
					}
					if (l == boundBottom + 1) {
						continue;
					}
					else if (m_fruit[l][col].m_type == fruit) {
						m_fruit[line][col].m_type = -1;
						if ((line - 1) >= boundTop) {
							m_fruit[line-1][col].m_type = enemy;
						}
					}
				}
				else if (m_fruit[line][col].m_type == fruit) {
					m_fruit[line][col].m_type = -1;
					if ((line - 1) >= boundTop) {
						m_fruit[line - 1][col].m_type = fruit;
					}
				}
				// reset animation timer
				m_fruit[line][col].m_animationTimer = 0.0f;
			}
		}
	}

	void update_down(int fruit, bool invTeam)
	{
		if (invTeam) {
			if (fruit == 0) {
				fruit = 1;
			}
			else {
				fruit = 0;
			}
		}
		int enemy = (fruit == 0) ? 1 : 0;
		for (int line{ boundBottom }; line >= boundTop; --line)
		{
			for (int col{ boundLeft }; col <= boundRight; ++col)
			{
				if (m_fruit[line][col].m_type == enemy) {
					//check if there is a pusher up the column
					int l{ line };
					while (m_fruit[l][col].m_type == enemy && l >= boundTop) {
						l--;
					}
					if (l == boundTop - 1) {
						continue;
					}
					else if (m_fruit[l][col].m_type == fruit) {
						m_fruit[line][col].m_type = -1;
						if ((line + 1) <= boundBottom) {
							m_fruit[line + 1][col].m_type = enemy;
						}
					}
				}
				else if (m_fruit[line][col].m_type == fruit) {
					m_fruit[line][col].m_type = -1;
					if ((line + 1) <= boundBottom) {
						m_fruit[line + 1][col].m_type = fruit;
					}
				}
				// reset animation timer
				m_fruit[line][col].m_animationTimer = 0.0f;
			}
		}
	}

	void update_right(int fruit, bool invTeam)
	{
		if (invTeam) {
			if (fruit == 0) {
				fruit = 1;
			}
			else {
				fruit = 0;
			}
		}
		int enemy = (fruit == 0) ? 1 : 0;
		for (int line{ boundTop }; line <= boundBottom; ++line)
		{
			for (int col{ boundRight }; col >= boundLeft; --col)
			{
				if (m_fruit[line][col].m_type == enemy) {
					//check if there is a pusher on the left side of the line
					int c{ col };
					while (m_fruit[line][c].m_type == enemy && c >= boundLeft) {
						c--;
					}
					if (c == boundLeft - 1) {
						continue;
					}
					else if (m_fruit[line][c].m_type == fruit) {
						m_fruit[line][col].m_type = -1;
						if ((col + 1) <= boundRight) {
							m_fruit[line][col + 1].m_type = enemy;
						}
					}
				}
				else if (m_fruit[line][col].m_type == fruit) {
					m_fruit[line][col].m_type = -1;
					if ((col + 1) <= boundRight) {
						m_fruit[line][col + 1].m_type = fruit;
					}
				}
				// reset animation timer
				m_fruit[line][col].m_animationTimer = 0.0f;
			}
		}
	}

	void update_left(int fruit, bool invTeam)
	{
		if (invTeam) {
			if (fruit == 0) {
				fruit = 1;
			}
			else {
				fruit = 0;
			}
		}
		int enemy = (fruit == 0) ? 1 : 0;
		for (int line{ boundTop }; line <= boundBottom; ++line)
		{
			for (int col{ boundLeft }; col <= boundRight; ++col)
			{
				if (m_fruit[line][col].m_type == enemy) {
					//check if there is a pusher on the right side of the line
					int c{ col };
					while (m_fruit[line][c].m_type == enemy && c <= boundRight) {
						c++;
					}
					if (c == boundRight + 1) {
						continue;
					}
					else if (m_fruit[line][c].m_type == fruit) {
						m_fruit[line][col].m_type = -1;
						if ((col - 1) >= boundLeft) {
							m_fruit[line][col - 1].m_type = enemy;
						}
					}
				}
				else if (m_fruit[line][col].m_type == fruit) {
					m_fruit[line][col].m_type = -1;
					if ((col - 1) >= boundLeft) {
						m_fruit[line][col - 1].m_type = fruit;
					}
				}
				// reset animation timer
				m_fruit[line][col].m_animationTimer = 0.0f;
			}
		}
	}

	void update_boundaries()
	{
		int min_x{ 8 };
		int max_x{ -1 };
		int min_y{ 8 };
		int max_y{ -1 };
		for (int line{ boundTop }; line <= boundBottom; ++line)
		{
			for (int col{ boundLeft }; col <= boundRight; ++col)
			{
				int t{ m_fruit[line][col].m_type };
				if (t != -1)
				{
					if (col < min_x) {
						min_x = col;
					}
					if (col > max_x) {
						max_x = col;
					}
					if (line > max_y) {
						max_y = line;
					}
					if (line < min_y) {
						min_y = line;
					}
				}
			}
		}

		// set dying tiles
		int diff_left = min_x - boundLeft;
		int diff_right = boundRight - max_x;
		int diff_top = min_y - boundTop;
		int diff_bottom = boundBottom - max_y;

		float delay{ 0.0f };
		if (diff_bottom > 0) {
			for (int line{ boundBottom }; line > max_y; --line)
			{
				int start, end;
				if (diff_left > 0) {
					start = boundLeft;
				}
				else {
					start = min_x;
				}
				if (diff_right > 0) {
					end = boundRight;
				}
				else {
					end = max_x;
				}
				for (int col{ start }; col <= end; ++col)
				{
					m_tile[line][col].m_dying = true;
					delay -= (col - start) * 0.125f;
					m_tile[line][col].m_animationTimer += delay;
				}
			}
		}
		if (diff_right > 0) {
			for (int col{ boundRight }; col > max_x; --col)
			{
				int start, end;
				if (diff_bottom > 0) {
					start = boundBottom;
				}
				else {
					start = max_y;
				}
				if (diff_top > 0) {
					end = boundTop;
				}
				else {
					end = min_y;
				}
				for (int line{ start }; line >= end; --line)
				{
					m_tile[line][col].m_dying = true;
					if (delay < 0.0f) {
						delay -= 0.125f;
					}
					else {
						delay -= (start - line) * 0.125f;
					}
					m_tile[line][col].m_animationTimer += delay;
				}
			}
		}
		if (diff_top > 0) {
			for (int line{ boundTop }; line < min_y; ++line)
			{
				int start, end;
				if (diff_left > 0) {
					start = boundLeft;
				}
				else {
					start = min_x;
				}
				if (diff_right > 0) {
					end = boundRight;
				}
				else {
					end = max_x;
				}
				for (int col{ end }; col >= start; --col)
				{
					m_tile[line][col].m_dying = true;
					if (delay < 0.0f) {
						delay -= 0.125f;
					}
					else {
						delay -= (end - col) * 0.125f;
					}
					m_tile[line][col].m_animationTimer += delay;
				}
			}
		}
		if (diff_left > 0) {
			for (int col{ boundLeft }; col < min_x; ++col)
			{
				int start, end;
				if (diff_bottom > 0) {
					end = boundBottom;
				}
				else {
					end = max_y;
				}
				if (diff_top > 0) {
					start = boundTop;
				}
				else {
					start = min_y;
				}
				for (int line{ start }; line <= end; ++line)
				{
					m_tile[line][col].m_dying = true;
					if (delay < 0.0f) {
						delay -= 0.125f;
					}
					else {
						delay -= (line - start) * 0.125f;
					}
					m_tile[line][col].m_animationTimer += delay;
				}
			}
		}
		if (delay < 0.0f) {
			m_dyingTimer = delay - 0.25f;
		}

		// set new boundaries
		boundLeft = min_x;
		boundRight = max_x;
		boundTop = min_y;
		boundBottom = max_y;
	}

	void cleanup()
	{
		for (auto& tex : m_tex)
		{
			if (tex.id != -1)
			{
				glDeleteTextures(1, &tex.id);
			}
		}
	}

	Tile m_tile[8][8];
	Fruit m_fruit[8][8];
	std::array<Texture, 11> m_tex = {
		createTexture("assets/orange_big.tga", TEXTURE_TYPE::DIFFUSE, true),
		createTexture("assets/banane_big.tga", TEXTURE_TYPE::DIFFUSE, true),
		createTexture("assets/board.tga", TEXTURE_TYPE::DIFFUSE, true),
		createTexture("assets/board_bottom.tga", TEXTURE_TYPE::DIFFUSE, true),
		createTexture("assets/board_dying1.tga", TEXTURE_TYPE::DIFFUSE, true), // 4
		createTexture("assets/board_dying2.tga", TEXTURE_TYPE::DIFFUSE, true), // 5
		createTexture("assets/board_dying3.tga", TEXTURE_TYPE::DIFFUSE, true), // 6
		createTexture("assets/board_dying4.tga", TEXTURE_TYPE::DIFFUSE, true), // 7
		createTexture("assets/board_dying5.tga", TEXTURE_TYPE::DIFFUSE, true), // 8
		createTexture("assets/board_dying6.tga", TEXTURE_TYPE::DIFFUSE, true), // 9
		createTexture("assets/board_dying7.tga", TEXTURE_TYPE::DIFFUSE, true)  // 10
	};
	Sprite m_square;
	Sprite m_rect;
	Sprite m_orange;
	Sprite m_banane;
	int boundLeft;
	int boundRight;
	int boundTop;
	int boundBottom;
	float m_dyingTimer;
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

	void cleanup()
	{
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_vbo);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vao);
	}

	int m_focus; // 0 = pseudo, 1 = chat, 2 = not writing
	size_t m_pos;
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
		
		std::array<std::string, 2> m_textInput = {"", ""};
		std::array<std::vector<int>, 2> m_textSectionsWidth;
		std::vector<std::string> m_chatLog;
		Cursor m_cursor;
		float m_deltaWrite;
		WRITE_ACTION m_lastWriteAction;
		std::string m_lastCharacter;
	
	private:
		void write_aux(WRITE_ACTION writeAction, std::string& character, float delta, int boundX, glm::vec3 cursor_shape);
};

struct Timer
{
	Timer() :
		m_shader("shaders/timer/vertex.glsl", "shaders/timer/geometry.glsl", "shaders/timer/fragment.glsl")
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		float data[2] = {0.0f, 0.0f};

		glBufferData(GL_ARRAY_BUFFER, 2, data, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void draw(float ratio, glm::vec3 color, float on_off, int fruit, bool flickering)
	{
		glBindVertexArray(m_vao);
		m_shader.use();
		m_shader.setFloat("ratio", ratio);
		m_shader.setBool("flickering", flickering);
		m_shader.setFloat("on_off", on_off);
		m_shader.setVec3f("color", color);
		m_shader.setBool("orange", (fruit == 0) ? true : false);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}

	void cleanup()
	{
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_vbo);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vao);
	}

	GLuint m_vao;
	GLuint m_vbo;
	Shader m_shader;
};

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
		int getCursorFocus();
		Writer& get_writer();
		MOVE& get_move();
		void set_turn(int turn);
		int getTeam();
		void set_winner(int winner);
		void updateUI(std::bitset<10> & inputs, char* text_input, int screenW, int screenH, float delta);

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

	public: // game behavior
		
		// UI
		void createUI(int screenW, int screenH);
		void swap_gender_features(Avatar::GENDER from, Avatar::GENDER to);
		void print_remaining_time();

		// fruits movement
		void set_animationTimer(bool inverseTeam);
		void set_animationTimer_move_up(bool inverseTeam);
		void set_animationTimer_move_down(bool inverseTeam);
		void set_animationTimer_move_right(bool inverseTeam);
		void set_animationTimer_move_left(bool inverseTeam);

	public:

		Avatar m_avatar;
		Avatar m_avatar_opponent;
		Writer m_writer;
		Cards m_cards;
		Board m_board;
		UI m_ui;
		int m_fruit; // 0 => orange, 1 => banane, -1 => undefined
		bool m_inverseTeam;
		int m_turn;
		float m_remaining_time;
		float m_remaining_time_enemy;
		float m_half_sec;
		int m_winner;
		std::string m_pseudo_orange;
		std::string m_pseudo_banane;
		MOVE m_move;
		float m_animationTimer;
		Sprite m_popup;
		Sprite m_back_home;
		std::array<Texture, 4> m_popup_tex;
		Timer m_timer;
};

inline std::queue<std::string> g_msg2server_queue;
inline std::queue<std::string> g_msg2client_queue;
inline std::mutex g_msg2server_mutex;
inline std::mutex g_msg2client_mutex;
inline bool g_connected{ false };
inline std::mutex g_connected_mutex;
inline bool g_try_connection{ false };
inline bool g_search_opponent{ false };
inline bool g_game_found{ false };
inline std::string g_game_init{""};
inline std::mutex g_game_init_mutex;
inline std::mutex g_fruit_move_mutex;
inline std::mutex g_turn_mutex;
inline std::mutex g_rte_mutex; // remaining time enemy

#endif
