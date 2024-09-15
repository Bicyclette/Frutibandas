#include "game.hpp"

Game::Game(int clientWidth, int clientHeight) :
	activeScene(0),
	graphics(clientWidth, clientHeight),
	m_bandas(graphics)
{
	// scene
	glm::vec3 camPos;
	glm::vec3 camTarget;
	glm::vec3 camDir;
	glm::vec3 camRight;
	glm::vec3 camUp;

	scenes.emplace_back("Frutibandas", 0);

	camPos = glm::vec3(0.0f, 0.0f, -1.0f);
	camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	camDir = glm::normalize(camTarget - camPos);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camRight = glm::normalize(glm::cross(camDir, camUp));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[0].addCamera(CAM_TYPE::REGULAR, glm::ivec2(clientWidth, clientHeight), camPos, camTarget, camUp, 45.0f, 0.1f, 100.0f);
	scenes[0].setActiveCamera(0);

	// music
	scenes[0].addSoundSource(glm::vec3(0.0f));
	scenes[0].getSoundSource(0).set_looping(false);
	scenes[0].getSoundSource(0).set_volume(m_bandas.music.volume);
	scenes[0].addAudioFile("assets/sound/intro.wav");
	scenes[0].addAudioFile("assets/sound/loop.wav");
}

Game::~Game() {}

void Game::draw(float& delta, double& elapsedTime, int width, int height, DRAWING_MODE mode, bool debug, bool debugPhysics)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(activeScene < scenes.size())
	{
        // DRAW USER INTERFACE
        drawUI(delta, elapsedTime, width, height, mode);

		// COMPOSITING
		compositingPass();
	}
	else
	{
		std::cerr << "Error: wrong scene index supplied for draw command.\n";
	}
}

void Game::drawUI(float& delta, double& elapsedTime, int width, int height, DRAWING_MODE mode)
{
	if (m_bandas.m_ui.get_active_page() == 0)
	{
		m_bandas.draw_home_page(delta);
	}
	else if (m_bandas.m_ui.get_active_page() == 1)
	{
		m_bandas.draw_game_page(delta);
	}
}

void Game::sound_manager()
{
	if (m_bandas.m_ui.get_active_page() == 1)
	{
		if (!scenes[0].getSoundSource(0).is_playing() && !m_bandas.music.intro_done)
		{
			m_bandas.music.intro_done = true;
			scenes[0].playSound(0, 0);
		}
		else if (!scenes[0].getSoundSource(0).is_playing() && m_bandas.music.intro_done)
		{
			scenes[0].getSoundSource(0).set_looping(true);
			scenes[0].playSound(0, 1);
		}
		scenes[0].getSoundSource(0).set_volume(m_bandas.music.volume);
	}
	else if(m_bandas.m_ui.get_active_page() == 0)
	{
		if (scenes[0].getSoundSource(0).is_playing())
		{
			scenes[0].stopSound(0, 0);
			scenes[0].stopSound(0, 1);
			scenes[0].getSoundSource(0).set_looping(false);
			m_bandas.music.intro_done = false;
		}
	}
}

void Game::resizeScreen(int clientWidth, int clientHeight)
{
	#pragma omp parallel for
	for(int i = 0; i < scenes.size(); ++i)
	{
		scenes[i].updateCameraPerspective(glm::ivec2(clientWidth, clientHeight));
	}

	graphics.resizeScreen(clientWidth, clientHeight);
}

void Game::updateSceneActiveCameraView(int index, const std::bitset<10> inputs, std::array<int, 3> mouse, float delta)
{
	if(index < scenes.size())
	{
		CAM_TYPE type = scenes[index].getActiveCamera().getType();
		if (type == CAM_TYPE::REGULAR)
		{
			scenes[index].getActiveCamera().updateViewMatrix(inputs, mouse, delta);
		}
	}
}

Graphics& Game::getGraphics()
{
	return graphics;
}

std::vector<Scene> & Game::getScenes()
{
	return scenes;
}

int Game::getActiveScene()
{
	return activeScene;
}

void Game::setActiveScene(int index)
{
	activeScene = index;
}

void Game::compositingPass()
{
	{ sceneCompositing(); uiCompositing(); }

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader& s{graphics.end};
	s.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics.compositeFBO[0]->getAttachments()[0].id);
	s.setInt("scene", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, graphics.compositeFBO[1]->getAttachments()[0].id);
	s.setInt("ui", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, graphics.compositeFBO[1]->getAttachments()[1].id);
	s.setInt("ui_mask", 2);

	graphics.getQuadMesh()->draw(s);
}

void Game::sceneCompositing()
{
	graphics.compositeFBO[0]->bind();
	glClear(GL_COLOR_BUFFER_BIT);

	Shader& s{graphics.sceneCompositing};
	s.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics.getNormalFBO(0)->getAttachments()[0].id);
	s.setInt("scene", 0);
	s.setInt("tone_mapping", static_cast<int>(graphics.get_scene_tone_mapping()));
	graphics.getQuadMesh()->draw(s);
}

void Game::uiCompositing()
{
	graphics.compositeFBO[1]->bind();
	glClear(GL_COLOR_BUFFER_BIT);

	Shader& s{ graphics.uiCompositing };
	s.use();

	if (m_bandas.m_ui.get_active_page() == 1) {
		s.setInt("page", 1);
	}
	else {
		s.setInt("page", 0);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics.userInterfaceFBO->getAttachments()[0].id);
	s.setInt("ui", 0);
	
	s.setInt("tone_mapping", static_cast<int>(graphics.get_ui_tone_mapping()));
	graphics.getQuadMesh()->draw(s);
}
