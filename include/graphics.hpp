#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <memory>
#include <utility>
#include <array>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "mesh.hpp"
#include "framebuffer.hpp"
#include "renderTexture.hpp"
#include "shader_light.hpp"
#include "helpers.hpp"

enum class TONE_MAPPING
{
	REINHARD = 0,
	ACES = 1,
	OFF = 2
};

class Graphics
{
	public:

		Graphics(int width, int height);
		void setNearPlane(float nearPlane);
		void setFarPlane(float farPlane);
		void set_scene_tone_mapping(TONE_MAPPING tone);
		void set_ui_tone_mapping(TONE_MAPPING tone);
		TONE_MAPPING get_scene_tone_mapping();
		TONE_MAPPING get_ui_tone_mapping();
		Shader & getPBRShader();
		Shader & getFinalShader();
		std::unique_ptr<Framebuffer> & getMultisampleFBO();
		std::unique_ptr<Framebuffer> & getNormalFBO(int index);
		std::unique_ptr<Mesh> & getQuadMesh();
		void resizeScreen(int width, int height);
	public:

		std::unique_ptr<Framebuffer> multisample; // color + depth + stencil
		std::array<std::unique_ptr<Framebuffer>, 2> normal; // only color, no multisampling
		std::unique_ptr<Framebuffer> userInterfaceFBO;
		std::unique_ptr<Framebuffer> avatarFBO;
		std::unique_ptr<Framebuffer> opponentAvatarFBO;
		std::array<std::unique_ptr<Framebuffer>, 2> compositeFBO;

		TONE_MAPPING scene_tone_mapping;
		TONE_MAPPING ui_tone_mapping;
		float aspect_ratio;
		float near;
		float far;
		Shader pbr;
		Shader sceneCompositing;
		Shader uiCompositing;
		Shader end;

		std::unique_ptr<Mesh> quad;
		Material quadMaterial;
};

#endif
