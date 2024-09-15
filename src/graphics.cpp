#include "graphics.hpp"

Graphics::Graphics(int width, int height) :
	scene_tone_mapping(TONE_MAPPING::ACES),
	ui_tone_mapping(TONE_MAPPING::OFF),
	aspect_ratio(static_cast<float>(width)/static_cast<float>(height)),
	near(0.1f),
	far(100.0f),
	multisample{std::make_unique<Framebuffer>(true, true, true)},
	normal{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
	},
	userInterfaceFBO{ std::make_unique<Framebuffer>(true, false, true) },
	avatarFBO{ std::make_unique<Framebuffer>(true, false, true) },
	opponentAvatarFBO{std::make_unique<Framebuffer>(true, false, true)},
	compositeFBO{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
	},
	pbr("shaders/PBR/vertex.glsl", "shaders/PBR/fragment.glsl", SHADER_TYPE::PBR),
	sceneCompositing("shaders/compositing/scene/vertex.glsl", "shaders/compositing/scene/fragment.glsl", SHADER_TYPE::COMPOSITING),
	uiCompositing("shaders/compositing/ui/vertex.glsl", "shaders/compositing/ui/fragment.glsl", SHADER_TYPE::COMPOSITING),
	end("shaders/final/vertex.glsl", "shaders/final/fragment.glsl", SHADER_TYPE::FINAL)
{
	// Generic Multisample FBO
	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::RENDER_BUFFER, ATTACHMENT_TARGET::DEPTH_STENCIL, width, height);

	for(int i{0}; i < 2; ++i)
		normal[i]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);

	// UI FBO
    userInterfaceFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
    userInterfaceFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);

	// AVATAR FBO
	avatarFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, 512, 512);
	avatarFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, 512, 512);
	
	// OPPONENT AVATAR FBO
	opponentAvatarFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, 512, 512);
	opponentAvatarFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, 512, 512);

	// COMPOSITING
	compositeFBO[0]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	compositeFBO[1]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	compositeFBO[1]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);

	// quad mesh for rendering final image
	glm::vec3 normal(0.0f, 0.0f, 1.0f);
	std::vector<Vertex> vertices{{
		Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), normal, glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(1.0f, -1.0f, 0.0f), normal, glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(1.0f, 1.0f, 0.0f), normal, glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), normal, glm::vec2(0.0f, 1.0f))
	}};

	std::vector<int> indices{{
		0, 1, 2,
		0, 2, 3
	}};

	quadMaterial.opacity = 1.0f;
	quad = std::make_unique<Mesh>(vertices, indices, quadMaterial, "final image", glm::vec3(0.0f));
}

void Graphics::setNearPlane(float nearPlane)
{
	near = nearPlane;
}

void Graphics::setFarPlane(float farPlane)
{
	far = farPlane;
}

void Graphics::set_scene_tone_mapping(TONE_MAPPING tone)
{
	scene_tone_mapping = tone;
}

void Graphics::set_ui_tone_mapping(TONE_MAPPING tone)
{
	ui_tone_mapping = tone;
}

TONE_MAPPING Graphics::get_scene_tone_mapping()
{
	return scene_tone_mapping;
}

TONE_MAPPING Graphics::get_ui_tone_mapping()
{
	return ui_tone_mapping;
}

Shader & Graphics::getPBRShader()
{
	return pbr;
}

Shader & Graphics::getFinalShader()
{
	return end;
}

std::unique_ptr<Framebuffer> & Graphics::getMultisampleFBO()
{
	return multisample;
}

std::unique_ptr<Framebuffer> & Graphics::getNormalFBO(int index)
{
	return normal[index];
}

std::unique_ptr<Mesh> & Graphics::getQuadMesh()
{
	return quad;
}

void Graphics::resizeScreen(int width, int height)
{
	aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
	multisample = std::make_unique<Framebuffer>(true, true, true);
	normal = std::array<std::unique_ptr<Framebuffer>, 2>{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
	};
	
	userInterfaceFBO = std::make_unique<Framebuffer>(true, false, true);
	avatarFBO = std::make_unique<Framebuffer>(true, false, true);
	opponentAvatarFBO = std::make_unique<Framebuffer>(true, false, true);
	compositeFBO = std::array<std::unique_ptr<Framebuffer>, 2>{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
	};

	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::RENDER_BUFFER, ATTACHMENT_TARGET::DEPTH_STENCIL, width, height);

	for(int i{0}; i < 2; ++i)
		normal[i]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	
    // UI FBO
    userInterfaceFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
    userInterfaceFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);

	// AVATAR FBO
	avatarFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, 512, 512);
	avatarFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, 512, 512);

	// OPPONENT AVATAR FBO
	opponentAvatarFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, 512, 512);
	opponentAvatarFBO->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, 512, 512);

	// COMPOSITING
	compositeFBO[0]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	compositeFBO[1]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	compositeFBO[1]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
}