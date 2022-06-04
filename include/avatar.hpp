#ifndef AVATAR_HPP
#define AVATAR_HPP

#include <GL/glew.h>
#include <array>
#include "shader_light.hpp"

// eye colors
#define JAUNE	glm::vec3(53 / 360.0f, 0.95f, 1.0f)
#define MARRON	glm::vec3(25 / 360.0f, 0.95f, 0.4f)
#define ROUGE	glm::vec3(360 / 360.0f, 1.0f, 1.0f)
#define VIOLET	glm::vec3(275 / 360.0f, 1.0f, 1.0f)
#define ROSE	glm::vec3(322 / 360.0f, 1.0f, 1.0f)
#define BLEU	glm::vec3(216 / 360.0f, 1.0f, 1.0f)
#define VERT	glm::vec3(100 / 360.0f, 1.0f, 1.0f)
#define BLACK	glm::vec3(0.0f, 0.0f, 1.0f)

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
#define H_ORANGE glm::vec3(0.78f, 0.31f, 0.15f)
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
#define H_BLACK glm::vec3(0.135f, 0.053f, 0.008f)

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
		GRANDE,
		MACHOIRE_SOURIRE,
		SOURIRE,
		TETINE
	};
	enum class HAIR
	{
		MIXTE,
		HERISSON,
		BULBE,
		ARRIERE,
		DECOIFFE,
		ELVIS,
		EPI1,
		EPI2,
		EPI3,
		MECHE_AVANT,
		STEADY,
		CRETE,
		KASPA,
		SEVENTIES,
		AU_BOL,
		BOULES,
		COUETTES,
		FRANGE,
		MI_LONG,
		PONYTAIL,
		RAIE,
		MEDUSE,
		NATTES
	};
	enum class EYES
	{
		EGYPTE,
		INQUIETE,
		MANGA,
		AMANDE,
		COLERE,
		GROS,
		FATIGUE,
		PSYCHO
	};
	enum class SAC
	{
		NONE,
		LVL1,
		LVL2,
		LVL3
	};
	enum class FACE
	{
		NORMAL,
		TACHES
	};

	struct Frutibouille
	{
		Frutibouille(Avatar::FACE face, Avatar::HAIR hair, Avatar::EYES eyes, Avatar::MOUTH mouth, Avatar::SAC sac, int skin_id, int hair_id, int eyes_id)
		{
			m_face = face;
			m_hair = hair;
			m_eyes = eyes;
			m_mouth = mouth;
			m_sac = sac;
			m_skin_color_id = skin_id;
			m_hair_color_id = hair_id;
			m_eyes_color_id = eyes_id;
		}

		Avatar::FACE m_face;
		Avatar::HAIR m_hair;
		Avatar::EYES m_eyes;
		Avatar::MOUTH m_mouth;
		Avatar::SAC m_sac;
		int m_skin_color_id;
		int m_hair_color_id;
		int m_eyes_color_id;
	};

	Avatar() :
		m_gender(Avatar::GENDER::MALE),
		m_homme(Avatar::FACE::TACHES, Avatar::HAIR::BULBE, Avatar::EYES::MANGA, Avatar::MOUTH::MACHOIRE_SOURIRE, Avatar::SAC::NONE, 4, 3, 0),
		m_femme(Avatar::FACE::TACHES, Avatar::HAIR::COUETTES, Avatar::EYES::MANGA, Avatar::MOUTH::SOURIRE, Avatar::SAC::NONE, 4, 3, 0),
		m_eyes_color{JAUNE,MARRON,ROUGE,VIOLET,ROSE,BLEU,VERT},
		m_skin_color{
			PALE1,PALE2,PALE3,
			MARRON_CLAIR1,MARRON_CLAIR2,MARRON_CLAIR3,
			MARRON1,MARRON2,MARRON3,MARRON4,
			H_ROUGE,H_ORANGE,
			H_JAUNE1,H_JAUNE2,
			H_VERT1,H_VERT2,
			H_BLEU1,H_BLEU2,H_BLEU3,
			H_POURPRE,H_VIOLET,H_ROSE,H_ROSE_BONBON},
		m_hair_color{H_ROUGE,H_ORANGE,H_BLACK,H_JAUNE1,H_JAUNE2,H_VERT1,H_VERT2,H_BLEU1,H_BLEU2,H_BLEU3,H_POURPRE,H_VIOLET,H_ROSE,H_ROSE_BONBON},
		m_shaderHSL("shaders/avatar/HSL/vertex.glsl", "shaders/avatar/HSL/fragment.glsl"),
		m_shaderRGB("shaders/avatar/RGB/vertex.glsl", "shaders/avatar/RGB/fragment.glsl"),
		m_shader("shaders/avatar/simple/vertex.glsl", "shaders/avatar/simple/fragment.glsl"),
		m_projection(glm::ortho(0.0f, 512.0f, 0.0f, 512.0f)),
		m_tex_homme{
			createTexture("assets/avatar/sprites/homme/cheveux_arriere.tga", TEXTURE_TYPE::DIFFUSE, true),					// 0
			createTexture("assets/avatar/sprites/homme/cheveux_bulbe.tga", TEXTURE_TYPE::DIFFUSE, true),					// 1
			createTexture("assets/avatar/sprites/homme/cheveux_decoiffe.tga", TEXTURE_TYPE::DIFFUSE, true),					// 2
			createTexture("assets/avatar/sprites/homme/cheveux_elvis.tga", TEXTURE_TYPE::DIFFUSE, true),					// 3
			createTexture("assets/avatar/sprites/homme/cheveux_epi1.tga", TEXTURE_TYPE::DIFFUSE, true),						// 4
			createTexture("assets/avatar/sprites/homme/cheveux_epi2.tga", TEXTURE_TYPE::DIFFUSE, true),						// 5
			createTexture("assets/avatar/sprites/homme/cheveux_epi3.tga", TEXTURE_TYPE::DIFFUSE, true),						// 6
			createTexture("assets/avatar/sprites/homme/cheveux_herisson.tga", TEXTURE_TYPE::DIFFUSE, true),					// 7
			createTexture("assets/avatar/sprites/homme/cheveux_meche_avant.tga", TEXTURE_TYPE::DIFFUSE, true),				// 8
			createTexture("assets/avatar/sprites/homme/cheveux_steady.tga", TEXTURE_TYPE::DIFFUSE, true),					// 9
			createTexture("assets/avatar/sprites/homme/crete.tga", TEXTURE_TYPE::DIFFUSE, true),							// 10
			createTexture("assets/avatar/sprites/homme/kaspa_back.tga", TEXTURE_TYPE::DIFFUSE, true),						// 11
			createTexture("assets/avatar/sprites/homme/kaspa_front.tga", TEXTURE_TYPE::DIFFUSE, true),						// 12
			createTexture("assets/avatar/sprites/homme/yeux_amande.tga", TEXTURE_TYPE::DIFFUSE, true),						// 13
			createTexture("assets/avatar/sprites/homme/yeux_colere.tga", TEXTURE_TYPE::DIFFUSE, true),						// 14
			createTexture("assets/avatar/sprites/homme/yeux_gros.tga", TEXTURE_TYPE::DIFFUSE, true),						// 15
			createTexture("assets/avatar/sprites/homme/yeux_manga.tga", TEXTURE_TYPE::DIFFUSE, true),						// 16
			createTexture("assets/avatar/sprites/homme/yeux_amande_mask.tga", TEXTURE_TYPE::DIFFUSE, true),					// 17
			createTexture("assets/avatar/sprites/homme/yeux_gros_mask.tga", TEXTURE_TYPE::DIFFUSE, true),					// 18
			createTexture("assets/avatar/sprites/homme/yeux_manga_mask.tga", TEXTURE_TYPE::DIFFUSE, true)					// 19
		},
		m_tex_femme{
			createTexture("assets/avatar/sprites/femme/cheveux_70_back.tga", TEXTURE_TYPE::DIFFUSE, true),					// 0
			createTexture("assets/avatar/sprites/femme/cheveux_70_front.tga", TEXTURE_TYPE::DIFFUSE, true),					// 1
			createTexture("assets/avatar/sprites/femme/cheveux_au_bol_back.tga", TEXTURE_TYPE::DIFFUSE, true),				// 2
			createTexture("assets/avatar/sprites/femme/cheveux_au_bol_front.tga", TEXTURE_TYPE::DIFFUSE, true),				// 3
			createTexture("assets/avatar/sprites/femme/cheveux_boule_back.tga", TEXTURE_TYPE::DIFFUSE, true),				// 4
			createTexture("assets/avatar/sprites/femme/cheveux_boule_front.tga", TEXTURE_TYPE::DIFFUSE, true),				// 5
			createTexture("assets/avatar/sprites/femme/cheveux_couettes_back.tga", TEXTURE_TYPE::DIFFUSE, true),			// 6
			createTexture("assets/avatar/sprites/femme/cheveux_couettes_front.tga", TEXTURE_TYPE::DIFFUSE, true),			// 7
			createTexture("assets/avatar/sprites/femme/cheveux_decoiffe_back.tga", TEXTURE_TYPE::DIFFUSE, true),			// 8
			createTexture("assets/avatar/sprites/femme/cheveux_decoiffe_front.tga", TEXTURE_TYPE::DIFFUSE, true),			// 9
			createTexture("assets/avatar/sprites/femme/cheveux_frange_back.tga", TEXTURE_TYPE::DIFFUSE, true),				// 10
			createTexture("assets/avatar/sprites/femme/cheveux_frange_front.tga", TEXTURE_TYPE::DIFFUSE, true),				// 11
			createTexture("assets/avatar/sprites/femme/cheveux_mi_long_back.tga", TEXTURE_TYPE::DIFFUSE, true),				// 12
			createTexture("assets/avatar/sprites/femme/cheveux_mi_long_front.tga", TEXTURE_TYPE::DIFFUSE, true),			// 13
			createTexture("assets/avatar/sprites/femme/cheveux_ponytail_back.tga", TEXTURE_TYPE::DIFFUSE, true),			// 14
			createTexture("assets/avatar/sprites/femme/cheveux_ponytail_front.tga", TEXTURE_TYPE::DIFFUSE, true),			// 15
			createTexture("assets/avatar/sprites/femme/cheveux_raie_back.tga", TEXTURE_TYPE::DIFFUSE, true),				// 16
			createTexture("assets/avatar/sprites/femme/cheveux_raie_front.tga", TEXTURE_TYPE::DIFFUSE, true),				// 17
			createTexture("assets/avatar/sprites/femme/meduse_back.tga", TEXTURE_TYPE::DIFFUSE, true),						// 18
			createTexture("assets/avatar/sprites/femme/meduse_front.tga", TEXTURE_TYPE::DIFFUSE, true),						// 19
			createTexture("assets/avatar/sprites/femme/nattes_back.tga", TEXTURE_TYPE::DIFFUSE, true),						// 20
			createTexture("assets/avatar/sprites/femme/nattes_front.tga", TEXTURE_TYPE::DIFFUSE, true),						// 21
			createTexture("assets/avatar/sprites/femme/yeux_colere.tga", TEXTURE_TYPE::DIFFUSE, true),						// 22
			createTexture("assets/avatar/sprites/femme/yeux_egypte.tga", TEXTURE_TYPE::DIFFUSE, true),						// 23
			createTexture("assets/avatar/sprites/femme/yeux_manga.tga", TEXTURE_TYPE::DIFFUSE, true),						// 24
			createTexture("assets/avatar/sprites/femme/yeux_inquiet.tga", TEXTURE_TYPE::DIFFUSE, true),						// 25
			createTexture("assets/avatar/sprites/femme/yeux_manga_mask.tga", TEXTURE_TYPE::DIFFUSE, true),					// 26
			createTexture("assets/avatar/sprites/femme/yeux_egypte_mask.tga", TEXTURE_TYPE::DIFFUSE, true)					// 27
		},
		m_tex_mixte{
			createTexture("assets/avatar/sprites/mixte/bouche_grande.tga", TEXTURE_TYPE::DIFFUSE, true),					// 0
			createTexture("assets/avatar/sprites/mixte/bouche_moyenne.tga", TEXTURE_TYPE::DIFFUSE, true),					// 1
			createTexture("assets/avatar/sprites/mixte/bouche_petite.tga", TEXTURE_TYPE::DIFFUSE, true),					// 2
			createTexture("assets/avatar/sprites/mixte/cheveux_mixte_back.tga", TEXTURE_TYPE::DIFFUSE, true),				// 3
			createTexture("assets/avatar/sprites/mixte/cheveux_mixte_front.tga", TEXTURE_TYPE::DIFFUSE, true),				// 4
			createTexture("assets/avatar/sprites/mixte/sac1.tga", TEXTURE_TYPE::DIFFUSE, true),								// 5
			createTexture("assets/avatar/sprites/mixte/sac2.tga", TEXTURE_TYPE::DIFFUSE, true),								// 6
			createTexture("assets/avatar/sprites/mixte/sac3.tga", TEXTURE_TYPE::DIFFUSE, true),								// 7
			createTexture("assets/avatar/sprites/mixte/sourire.tga", TEXTURE_TYPE::DIFFUSE, true),							// 8
			createTexture("assets/avatar/sprites/mixte/sourire_dents.tga", TEXTURE_TYPE::DIFFUSE, true),					// 9
			createTexture("assets/avatar/sprites/mixte/sourire_machoire.tga", TEXTURE_TYPE::DIFFUSE, true),					// 10
			createTexture("assets/avatar/sprites/mixte/sourire_machoire_dents.tga", TEXTURE_TYPE::DIFFUSE, true),			// 11
			createTexture("assets/avatar/sprites/mixte/tetine.tga", TEXTURE_TYPE::DIFFUSE, true),							// 12
			createTexture("assets/avatar/sprites/mixte/visage1.tga", TEXTURE_TYPE::DIFFUSE, true),							// 13
			createTexture("assets/avatar/sprites/mixte/visage2.tga", TEXTURE_TYPE::DIFFUSE, true),							// 14
			createTexture("assets/avatar/sprites/mixte/yeux_1.tga", TEXTURE_TYPE::DIFFUSE, true),							// 15
			createTexture("assets/avatar/sprites/mixte/yeux_fatigue.tga", TEXTURE_TYPE::DIFFUSE, true),						// 16
			createTexture("assets/avatar/sprites/mixte/sac2_back.tga", TEXTURE_TYPE::DIFFUSE, true),						// 17
			createTexture("assets/avatar/sprites/mixte/sac3_back.tga", TEXTURE_TYPE::DIFFUSE, true),						// 18
			createTexture("assets/avatar/sprites/mixte/yeux_1_mask.tga", TEXTURE_TYPE::DIFFUSE, true),						// 19
			createTexture("assets/avatar/sprites/mixte/yeux_fatigue_mask.tga", TEXTURE_TYPE::DIFFUSE, true)					// 20
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

	void draw(bool mirrorX = false)
	{
		glBindVertexArray(m_vao);
		glActiveTexture(GL_TEXTURE0);

		m_shaderHSL.use();
		m_shaderHSL.setBool("mirrorX", mirrorX);
		m_shaderHSL.setMatrix("proj", m_projection);
		m_shaderHSL.setInt("image", 0);
		m_shaderHSL.setInt("mask", 1);

		m_shaderRGB.use();
		m_shaderRGB.setBool("mirrorX", mirrorX);
		m_shaderRGB.setMatrix("proj", m_projection);
		m_shaderRGB.setInt("image", 0);

		m_shader.use();
		m_shader.setBool("mirrorX", mirrorX);
		m_shader.setMatrix("proj", m_projection);
		m_shader.setInt("image", 0);

		if (m_gender == GENDER::MALE)
		{
			draw_male();
		}
		else
		{
			draw_female();
		}

		glBindVertexArray(0);
	}

	void draw_male()
	{
		if (m_homme.m_sac == SAC::NONE)
		{
			// draw back hair
			m_shaderRGB.use();
			if (m_homme.m_hair == HAIR::MIXTE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[3].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::KASPA)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[11].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		else if (m_homme.m_sac == SAC::LVL2)
		{
			m_shader.use();
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[17].id);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (m_homme.m_sac == SAC::LVL3)
		{
			m_shader.use();
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[18].id);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// draw face
		m_shaderRGB.use();
		if (m_homme.m_face == FACE::NORMAL) {
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[13].id);
			m_shaderRGB.setVec3f("color", m_skin_color[m_homme.m_skin_color_id]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (m_homme.m_face == FACE::TACHES) {
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[14].id);
			m_shaderRGB.setVec3f("color", m_skin_color[m_homme.m_skin_color_id]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		if (m_homme.m_sac == SAC::NONE || m_homme.m_sac == SAC::LVL3)
		{
			// draw eyes
			m_shaderHSL.use();
			if (m_homme.m_eyes == EYES::PSYCHO) {
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[15].id);
				m_shaderHSL.setVec3f("HSL", m_eyes_color[m_homme.m_eyes_color_id]);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[19].id);
				glActiveTexture(GL_TEXTURE0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_eyes == EYES::FATIGUE) {
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[16].id);
				m_shaderHSL.setVec3f("HSL", m_eyes_color[m_homme.m_eyes_color_id]);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[20].id);
				glActiveTexture(GL_TEXTURE0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_eyes == EYES::MANGA) {
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[16].id);
				m_shaderHSL.setVec3f("HSL", m_eyes_color[m_homme.m_eyes_color_id]);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[19].id);
				glActiveTexture(GL_TEXTURE0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_eyes == EYES::COLERE) {
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[14].id);
				m_shaderHSL.setVec3f("HSL", BLACK);
				glActiveTexture(GL_TEXTURE0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_eyes == EYES::AMANDE) {
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[13].id);
				m_shaderHSL.setVec3f("HSL", m_eyes_color[m_homme.m_eyes_color_id]);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[17].id);
				glActiveTexture(GL_TEXTURE0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_eyes == EYES::GROS) {
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[15].id);
				m_shaderHSL.setVec3f("HSL", m_eyes_color[m_homme.m_eyes_color_id]);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[18].id);
				glActiveTexture(GL_TEXTURE0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		
		if (m_homme.m_sac == SAC::NONE || m_homme.m_sac == SAC::LVL3 || m_homme.m_sac == SAC::LVL2)
		{
			// draw mouth
			if (m_homme.m_mouth == MOUTH::PETITE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[2].id);
				m_shaderRGB.setVec3f("color", m_skin_color[m_homme.m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_mouth == MOUTH::MOYENNE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[1].id);
				m_shaderRGB.setVec3f("color", m_skin_color[m_homme.m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_mouth == MOUTH::GRANDE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[0].id);
				m_shaderRGB.setVec3f("color", m_skin_color[m_homme.m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_mouth == MOUTH::SOURIRE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[8].id);
				m_shaderRGB.setVec3f("color", m_skin_color[m_homme.m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				m_shader.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[9].id);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_mouth == MOUTH::MACHOIRE_SOURIRE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[10].id);
				m_shaderRGB.setVec3f("color", m_skin_color[m_homme.m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				m_shader.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[11].id);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_mouth == MOUTH::TETINE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[12].id);
				m_shaderRGB.setVec3f("color", m_skin_color[17]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}

		m_shaderRGB.use();
		if (m_homme.m_sac == SAC::NONE)
		{
			// draw front hair
			if (m_homme.m_hair == HAIR::MIXTE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[4].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::KASPA)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[12].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::ARRIERE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[0].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::BULBE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[1].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::CRETE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[10].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::DECOIFFE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[2].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::ELVIS)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[3].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::EPI1)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[4].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::EPI2)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[5].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::EPI3)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[6].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::HERISSON)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[7].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::MECHE_AVANT)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[8].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::STEADY)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[9].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_homme.m_hair == HAIR::ARRIERE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_homme[0].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_homme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}

		// draw burlap sack
		m_shader.use();
		if (m_homme.m_sac == SAC::LVL1)
		{
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[5].id);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (m_homme.m_sac == SAC::LVL2)
		{
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[6].id);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (m_homme.m_sac == SAC::LVL3)
		{
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[7].id);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	void draw_female()
	{
		if (m_femme.m_sac == SAC::NONE)
		{
			// draw back hair
			m_shaderRGB.use();
			if (m_femme.m_hair == HAIR::MIXTE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[3].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::AU_BOL)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[2].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::BOULES)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[4].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::COUETTES)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[6].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::DECOIFFE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[8].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::FRANGE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[10].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::MEDUSE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[18].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::MI_LONG)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[12].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::NATTES)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[20].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::PONYTAIL)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[14].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::RAIE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[16].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::SEVENTIES)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[0].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		else if (m_femme.m_sac == SAC::LVL2)
		{
			m_shader.use();
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[17].id);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (m_femme.m_sac == SAC::LVL3)
		{
			m_shader.use();
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[18].id);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// draw face
		m_shaderRGB.use();
		if (m_femme.m_face == FACE::NORMAL) {
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[13].id);
			m_shaderRGB.setVec3f("color", m_skin_color[m_femme.m_skin_color_id]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (m_femme.m_face == FACE::TACHES) {
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[14].id);
			m_shaderRGB.setVec3f("color", m_skin_color[m_femme.m_skin_color_id]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		if (m_femme.m_sac == SAC::NONE || m_femme.m_sac == SAC::LVL3)
		{
			// draw eyes
			m_shaderHSL.use();
			if (m_femme.m_eyes == EYES::PSYCHO) {
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[15].id);
				m_shaderHSL.setVec3f("HSL", m_eyes_color[m_femme.m_eyes_color_id]);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[19].id);
				glActiveTexture(GL_TEXTURE0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_eyes == EYES::FATIGUE) {
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[16].id);
				m_shaderHSL.setVec3f("HSL", m_eyes_color[m_femme.m_eyes_color_id]);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[20].id);
				glActiveTexture(GL_TEXTURE0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_eyes == EYES::MANGA) {
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[24].id);
				m_shaderHSL.setVec3f("HSL", m_eyes_color[m_femme.m_eyes_color_id]);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[26].id);
				glActiveTexture(GL_TEXTURE0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_eyes == EYES::COLERE) {
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[22].id);
				m_shaderHSL.setVec3f("HSL", BLACK);
				glActiveTexture(GL_TEXTURE0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_eyes == EYES::EGYPTE) {
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[23].id);
				m_shaderHSL.setVec3f("HSL", m_eyes_color[m_femme.m_eyes_color_id]);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[27].id);
				glActiveTexture(GL_TEXTURE0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_eyes == EYES::INQUIETE) {
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[25].id);
				glActiveTexture(GL_TEXTURE0);
				m_shaderHSL.setVec3f("HSL", BLACK);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}

		if (m_femme.m_sac == SAC::NONE || m_femme.m_sac == SAC::LVL3 || m_femme.m_sac == SAC::LVL2)
		{
			// draw mouth
			if (m_femme.m_mouth == MOUTH::PETITE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[2].id);
				m_shaderRGB.setVec3f("color", m_skin_color[m_femme.m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_mouth == MOUTH::MOYENNE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[1].id);
				m_shaderRGB.setVec3f("color", m_skin_color[m_femme.m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_mouth == MOUTH::GRANDE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[0].id);
				m_shaderRGB.setVec3f("color", m_skin_color[m_femme.m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_mouth == MOUTH::SOURIRE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[8].id);
				m_shaderRGB.setVec3f("color", m_skin_color[m_femme.m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				m_shader.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[9].id);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_mouth == MOUTH::MACHOIRE_SOURIRE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[10].id);
				m_shaderRGB.setVec3f("color", m_skin_color[m_femme.m_skin_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				m_shader.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[11].id);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_mouth == MOUTH::TETINE) {
				m_shaderRGB.use();
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[12].id);
				m_shaderRGB.setVec3f("color", m_skin_color[21]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}

		m_shaderRGB.use();
		if (m_femme.m_sac == SAC::NONE)
		{
			// draw front hair
			if (m_femme.m_hair == HAIR::MIXTE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_mixte[4].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::AU_BOL)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[3].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::BOULES)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[5].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::COUETTES)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[7].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::DECOIFFE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[9].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::FRANGE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[11].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::MEDUSE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[19].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::MI_LONG)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[13].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::NATTES)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[21].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::PONYTAIL)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[15].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::RAIE)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[17].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (m_femme.m_hair == HAIR::SEVENTIES)
			{
				glBindTexture(GL_TEXTURE_2D, m_tex_femme[1].id);
				m_shaderRGB.setVec3f("color", m_hair_color[m_femme.m_hair_color_id]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}

		// draw burlap sack
		m_shader.use();
		if (m_femme.m_sac == SAC::LVL1)
		{
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[5].id);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (m_femme.m_sac == SAC::LVL2)
		{
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[6].id);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (m_femme.m_sac == SAC::LVL3)
		{
			glBindTexture(GL_TEXTURE_2D, m_tex_mixte[7].id);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	void cleanup()
	{
		for (auto& tex : m_tex_homme)
		{
			if (tex.id != -1) {
				glDeleteTextures(1, &tex.id);
			}
		}
		for (auto& tex : m_tex_femme)
		{
			if (tex.id != -1) {
				glDeleteTextures(1, &tex.id);
			}
		}
		for (auto& tex : m_tex_mixte)
		{
			if (tex.id != -1) {
				glDeleteTextures(1, &tex.id);
			}
		}
	}

	std::vector<glm::vec3> m_eyes_color;
	std::vector<glm::vec3> m_skin_color;
	std::vector<glm::vec3> m_hair_color;
	std::vector<Texture> m_tex_homme;
	std::vector<Texture> m_tex_femme;
	std::vector<Texture> m_tex_mixte;
	Shader m_shaderRGB;
	Shader m_shaderHSL;
	Shader m_shader;
	glm::mat4 m_projection;
	GLuint m_vao;
	GLuint m_vbo;
	GENDER m_gender; // 0 = male, 1 = female
	Frutibouille m_homme;
	Frutibouille m_femme;
};

#endif