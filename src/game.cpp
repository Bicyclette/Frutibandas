#include "game.hpp"

Game::Game(int clientWidth, int clientHeight) :
	activeScene(0),
	activeVehicle(-1),
	graphics(clientWidth, clientHeight),
    textRenderer(std::make_unique<Text>(clientWidth, clientHeight))
{
    // load some fonts and set an active font
    textRenderer->load_police("assets/fonts/FreeMonoBold.ttf", 24);
    textRenderer->use_police(0);
	
	// scene objects
	std::vector<std::shared_ptr<Object>> scene_objects;

	glm::vec3 camPos;
	glm::vec3 camTarget;
	glm::vec3 camDir;
	glm::vec3 camRight;
	glm::vec3 camUp;

	// create client scene
	scenes.emplace_back("client", 0);

	camPos = glm::vec3(0.0f, 0.0f, -1.0f);
	camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	camDir = glm::normalize(camTarget - camPos);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camRight = glm::normalize(glm::cross(camDir, camUp));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[0].addCamera(CAM_TYPE::REGULAR, glm::ivec2(clientWidth, clientHeight), camPos, camTarget, camUp, 45.0f, 0.1f, 100.0f);

	scenes[0].setActiveCamera(0);

	// create frutibandas scene
	scenes.emplace_back("frutibandas", 1);

	camPos = glm::vec3(0.0f, 0.0f, -1.0f);
	camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	camDir = glm::normalize(camTarget - camPos);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camRight = glm::normalize(glm::cross(camDir, camUp));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[1].addCamera(CAM_TYPE::REGULAR, glm::ivec2(clientWidth, clientHeight), camPos, camTarget, camUp, 45.0f, 0.1f, 100.0f);
	scenes[1].setActiveCamera(0);

	// set sprite groups
	float screenAR = static_cast<float>(clientWidth) / static_cast<float>(clientHeight);
	sprite_group.emplace_back(0); // interface
	sprite_group.emplace_back(1); // plateau
	sprite_group.emplace_back(2); // oranges
	sprite_group.emplace_back(3); // bananes
	sprite_group.emplace_back(4); // chat

	// interface
	glm::vec2 avatar_size(0.12f, 0.12f * screenAR);
	glm::vec2 left_avatar_pos(0.02f, 0.97f);
	glm::vec2 right_avatar_pos(0.98f - avatar_size.x, 0.97f);

	glm::vec2 slot_size(0.08f, 0.16f);
	glm::vec4 slot_color(0.67f, 0.69f, 0.25f, 1.0f);
	float start_slot = 0.73f;
	float marginV = 0.01f;
	float marginH = (0.22f - slot_size.x * 2.0f) / 3.0f;

	std::unique_ptr<Sprite> bkg(std::make_unique<Sprite>(glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f), -0.25f, clientWidth, clientHeight));
	bkg->set_background_color(glm::vec4(0.21f, 0.30f, 0.025f, 1.0f));
	std::unique_ptr<Sprite> leftPanel(std::make_unique<Sprite>(glm::vec2(0.0f, 1.0f), glm::vec2(0.22f, 1.0f), 0.5f, clientWidth, clientHeight));
	leftPanel->set_background_color(glm::vec4(0.737f, 0.806f, 0.396f, 1.0f));
	std::unique_ptr<Sprite> rightPanel(std::make_unique<Sprite>(glm::vec2(0.78f, 1.0f), glm::vec2(0.22f, 1.0f), 0.5f, clientWidth, clientHeight));
	rightPanel->set_background_color(glm::vec4(0.737f, 0.806f, 0.396f, 1.0f));
	
	std::unique_ptr<Sprite> left_avatar(std::make_unique<Sprite>(left_avatar_pos, avatar_size, 0.55f, clientWidth, clientHeight));
	std::unique_ptr<Sprite> right_avatar(std::make_unique<Sprite>(right_avatar_pos, avatar_size, 0.55f, clientWidth, clientHeight));

	float shiftX = 0.78f;
	float shiftY = 0.0f;
	std::vector<std::unique_ptr<Sprite>> left_cards;
	std::vector<std::unique_ptr<Sprite>> right_cards;
	for (int i{ 0 }; i < 4; ++i)
	{
		shiftY = (slot_size.y + marginV) * i;
		left_cards.push_back(std::make_unique<Sprite>(glm::vec2(marginH, start_slot - shiftY), slot_size, 0.55f, clientWidth, clientHeight));
		left_cards.push_back(std::make_unique<Sprite>(glm::vec2(slot_size.x + 2 * marginH, start_slot - shiftY), slot_size, 0.55f, clientWidth, clientHeight));
		right_cards.push_back(std::make_unique<Sprite>(glm::vec2(marginH + shiftX, start_slot - shiftY), slot_size, 0.55f, clientWidth, clientHeight));
		right_cards.push_back(std::make_unique<Sprite>(glm::vec2(slot_size.x + 2 * marginH + shiftX, start_slot - shiftY), slot_size, 0.55f, clientWidth, clientHeight));
	}

	std::unique_ptr<Sprite> rOrange(std::make_unique<Sprite>(glm::vec2(0.15f, 0.97f), glm::vec2(0.03f, 0.03f * screenAR), 0.8f, clientWidth, clientHeight));
	rOrange->set_background_img("assets/orange.tga");
	std::unique_ptr<Sprite> rBanane(std::make_unique<Sprite>(glm::vec2(0.79f, 0.97f + 0.03f * screenAR * 0.5f), glm::vec2(0.03f, 0.03f * screenAR * 1.5f), 0.8f, clientWidth, clientHeight));
	rBanane->set_background_img("assets/banane.tga");

	std::unique_ptr<Sprite> left(std::make_unique<Sprite>(glm::vec2(0.225f, 0.65f), glm::vec2(0.08f, 0.08f * screenAR), 0.75f, clientWidth, clientHeight));
	left->set_background_img("assets/arrow_left.tga");
	std::unique_ptr<Sprite> right(std::make_unique<Sprite>(glm::vec2(0.695f, 0.65f), glm::vec2(0.08f, 0.08f * screenAR), 0.75f, clientWidth, clientHeight));
	right->set_background_img("assets/arrow_right.tga");
	std::unique_ptr<Sprite> up(std::make_unique<Sprite>(glm::vec2(0.46f, 0.985f), glm::vec2(0.08f, 0.08f * screenAR), 0.75f, clientWidth, clientHeight));
	up->set_background_img("assets/arrow_up.tga");
	std::unique_ptr<Sprite> down(std::make_unique<Sprite>(glm::vec2(0.46f, 0.335f), glm::vec2(0.08f, 0.08f * screenAR), 0.75f, clientWidth, clientHeight));
	down->set_background_img("assets/arrow_down.tga");

	sprite_group[0].m_sprite.emplace_back(std::move(bkg));
	sprite_group[0].m_sprite.emplace_back(std::move(leftPanel));
	sprite_group[0].m_sprite.emplace_back(std::move(rightPanel));
	sprite_group[0].m_sprite.emplace_back(std::move(left_avatar));
	sprite_group[0].m_sprite.emplace_back(std::move(right_avatar));
	for (int i = 0; i < left_cards.size(); ++i)
	{
		sprite_group[0].m_sprite.emplace_back(std::move(left_cards[i]));
		sprite_group[0].m_sprite.emplace_back(std::move(right_cards[i]));
	}
	sprite_group[0].m_sprite.emplace_back(std::move(rOrange));
	sprite_group[0].m_sprite.emplace_back(std::move(rBanane));
	sprite_group[0].m_sprite.emplace_back(std::move(left));
	sprite_group[0].m_sprite.emplace_back(std::move(right));
	sprite_group[0].m_sprite.emplace_back(std::move(up));
	sprite_group[0].m_sprite.emplace_back(std::move(down));

	// plateau
	float tileDim = (0.56f / 12.0f);
	glm::vec2 top_left_start(0.22f + 2 * tileDim, 1.0f - 2 * tileDim * screenAR);
	for (int i{ 0 }; i < 8; ++i)
	{
		for (int j{ 0 }; j < 8; ++j)
		{
			std::unique_ptr<Sprite> tile(std::make_unique<Sprite>(top_left_start + glm::vec2(j * tileDim, -i * tileDim * screenAR), glm::vec2(tileDim, tileDim * screenAR), 0.55f, clientWidth, clientHeight));
			if (i == 7)
				tile->set_background_img("assets/board_bottom.tga");
			else
				tile->set_background_img("assets/board.tga");
			sprite_group[1].m_sprite.emplace_back(std::move(tile));
		}
	}

	// oranges et bananes
	glm::vec2 orange_start(0.22f + 2 * tileDim, 1.0f - 2 * tileDim * screenAR);
	glm::vec2 banane_start(0.22f + 2 * tileDim, 1.0f - (1.5 * tileDim * screenAR));

	for (int i{ 0 }; i < 8; ++i)
	{
		for (int j{ 0 }; j < 8; ++j)
		{
			int board_value = board[i][j];
			if (board_value == 0)
			{
				std::unique_ptr<Sprite> orange(std::make_unique<Sprite>(orange_start + glm::vec2(j,-i) * glm::vec2(tileDim, tileDim * screenAR), glm::vec2(tileDim, tileDim * screenAR), 0.6f, clientWidth, clientHeight));
				orange->set_background_img("assets/orange.tga");
				sprite_group[2].m_sprite.emplace_back(std::move(orange));
			}
			else if (board_value == 1)
			{
				std::unique_ptr<Sprite> banane(std::make_unique<Sprite>(banane_start + glm::vec2(j,-i) * glm::vec2(tileDim, tileDim * screenAR), glm::vec2(tileDim, tileDim * screenAR * 1.5f), 0.65f, clientWidth, clientHeight));
				banane->set_background_img("assets/banane.tga");
				sprite_group[3].m_sprite.emplace_back(std::move(banane));
			}
		}
	}

	// chat
	std::unique_ptr<Sprite> chat(std::make_unique<Sprite>(glm::vec2(0.23, 0.23f), glm::vec2(0.54f, 0.22f), 0.5f, clientWidth, clientHeight));
	chat->set_background_color(glm::vec4(0.737f, 0.806f, 0.396f, 1.0f));
	sprite_group[4].m_sprite.emplace_back(std::move(chat));
}

void Game::draw(float& delta, double& elapsedTime, int width, int height, DRAWING_MODE mode, bool debug, bool debugPhysics)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update physics
	if(!worldPhysics.empty())
	{
		if(debugPhysics)
		{
			worldPhysics[activeScene].stepSimulation(
					scenes[activeScene].getActiveCamera().getViewMatrix(),
					scenes[activeScene].getActiveCamera().getProjectionMatrix()
					);
			return;
		}
		worldPhysics[activeScene].stepSimulation();
	}

	// update character's current animation
	if(character && character->sceneID == scenes[activeScene].getId())
	{
		character->get()->getAnimator()->updateAnimation(delta);
	}

	// get shader
	Shader s = graphics.getPBRShader();

	if(activeScene < scenes.size())
	{
	    s.use();

		s.setInt("shadowOn", 0);
        if(graphics.shadowsOn())
        {
			s.setInt("shadowOn", 1);
			// SHADOW PASS : directional & spot light sources
			directionalShadowPass(activeScene, delta, mode);
			// SHADOW PASS : point light sources
			omnidirectionalShadowPass(activeScene, delta, mode);
        }

        // FILL G-BUFFER
        GBufferPass(activeScene, width, height, delta);

		// SSAO PASS
		if(graphics.ssaoOn())
			ssaoPass(activeScene, width, height, delta);

		// COLOR PASS : multisampling
		colorMultisamplePass(activeScene, width, height, delta, mode, debug);

        // BLOOM PASS
		if(graphics.bloomOn())
			bloomPass(width, height, graphics.getNormalFBO(1), 0, graphics.getBloomTexture(0));

		// VOLUMETRICS PASS
		if(graphics.volumetricLightingOn() && graphics.shadowsOn())
			volumetricsPass(activeScene, width, height, delta, elapsedTime);
		
        // MOTION BLUR PASS
		if(graphics.motionBlurFX)
			motionBlurPass(activeScene, width, height);

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
    graphics.userInterfaceFBO->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

	for (SpriteGroup & sg : sprite_group)
	{
		for(auto& s : sg.m_sprite)
			s->draw();
	}

	bloomPass(width, height, graphics.userInterfaceFBO, 1, graphics.getBloomTexture(1));
}


void Game::resizeScreen(int clientWidth, int clientHeight)
{
	#pragma omp parallel for
	for(int i{0}; i < scenes.size(); ++i)
	{
		scenes[i].updateCameraPerspective(glm::ivec2(clientWidth, clientHeight));
	}

	graphics.resizeScreen(clientWidth, clientHeight);
    textRenderer->resize_screen(clientWidth, clientHeight);
	for (SpriteGroup& sg : sprite_group)
	{
		for (auto& s : sg.m_sprite)
		{
			s->resize_screen(clientWidth, clientHeight);
		}
	}
}

void Game::updateSceneActiveCameraView(int index, const std::bitset<16> & inputs, std::array<int, 3> & mouse, float delta)
{
	if(index < scenes.size())
	{
		CAM_TYPE type = scenes[index].getActiveCamera().getType();
		if(type == CAM_TYPE::REGULAR)
			scenes[index].getActiveCamera().updateViewMatrix(inputs, mouse, delta);
		else if(type == CAM_TYPE::THIRD_PERSON)
			scenes[index].getActiveCamera().updateViewMatrix(character->getPosition(), character->getDirection(), inputs, mouse, delta);
		else if(type == CAM_TYPE::VEHICLE)
		{
			std::shared_ptr<Vehicle> & vehicle{scenes[index].getVehicles()[activeVehicle]};
			btRaycastVehicle * raycastVehicle = vehicle->vehicle;
			btVector3 position = raycastVehicle->getChassisWorldTransform().getOrigin();
			glm::vec3 pos(position.x(), position.y(), position.z());
			btVector3 direction = raycastVehicle->getForwardVector();
			glm::vec3 dir(direction.x(), direction.y(), direction.z());
			float steerAngle = vehicle->data.steering;
			glm::vec3 up = vehicle->up;
			scenes[index].getActiveCamera().updateViewMatrix(pos, -dir, up, -steerAngle, vehicle->data.steeringIncrement, inputs, mouse, delta);
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

int Game::getActiveVehicle()
{
	return activeVehicle;
}

void Game::setActiveVehicle(int index)
{
	activeVehicle = index;
}

void Game::setCharacterScene(int index)
{
	if(character)
	{
		character->sceneID = index;
	}
}

int Game::getCharacterScene()
{
	if(character)
	{
		return character->sceneID;
	}
	return -1;
}

void Game::setCharacter(std::string filePath, glm::mat4 aModel, std::string aName, int index, glm::ivec2 scrDim)
{
	glm::vec3 camPos;
	glm::vec3 camTarget;
	glm::vec3 camDir;
	glm::vec3 camRight;
	glm::vec3 camUp;

	character = std::make_shared<Character>(filePath, aModel, aName);
	setCharacterScene(index);

	camTarget = character->getPosition();
	camDir = glm::normalize(character->getDirection());
	camPos = camTarget - (camDir * 7.0f) + glm::vec3(0.0f, 3.0f, 0.0f);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camRight = glm::normalize(glm::cross(camDir, camUp));
	camUp = glm::normalize(glm::cross(camRight, camDir));
	scenes[character->sceneID].addCamera(CAM_TYPE::THIRD_PERSON, scrDim, camPos, camTarget, camUp, 45.0f, 0.1f, 100.0f);
	scenes[character->sceneID].setActiveCamera(scenes[character->sceneID].getCameras().size() - 1);

	for(int i{0}; i < scenes.size(); ++i)
	{
		scenes[i].setCharacter(character);
		worldPhysics[i].setKinematicCharacter(character->get(), character->getModel());
	}
}

void Game::removeCharacter()
{
	for(int i{0}; i < scenes.size(); ++i)
	{
		scenes[i].removeCharacter();
		worldPhysics[i].removeKinematicCharacter();
	}
	character.reset();
}

void Game::characterDoActionWalk(Character::DIRECTION d, float delta)
{
	if(character)
	{
		character->walk();
		worldPhysics[activeScene].characterDoActionWalk(character, d, delta);
	}
}

void Game::characterDoActionRun(Character::DIRECTION d, float delta)
{
	if(character)
	{
		character->run();
		worldPhysics[activeScene].characterDoActionRun(character, d, delta);
	}
}

void Game::characterDoActionJump(bool forward, float delta)
{
	if(character)
	{
		character->jump();
		worldPhysics[activeScene].characterDoActionJump(character, forward, delta);
	}
}

void Game::characterDoActionIdle()
{
	if(character)
	{
		character->idle();
		worldPhysics[activeScene].characterDoActionIdle(character);
	}
}

void Game::vehicleDrive(bool forward)
{
	worldPhysics[activeScene].vehicleDrive(scenes[activeScene].getVehicles()[activeVehicle], forward);
}

void Game::vehicleDriveReset()
{
	worldPhysics[activeScene].vehicleDriveReset(scenes[activeScene].getVehicles()[activeVehicle]);
}

void Game::vehicleSteering(VEHICLE_STEERING dir)
{
	worldPhysics[activeScene].vehicleSteering(scenes[activeScene].getVehicles()[activeVehicle], dir);
}

void Game::vehicleSteeringReset()
{
	worldPhysics[activeScene].vehicleSteeringReset(scenes[activeScene].getVehicles()[activeVehicle]);
}

void Game::vehicleSetWheelTransform()
{
	worldPhysics[activeScene].setVehicleWheelTransform(scenes[activeScene].getVehicles()[activeVehicle]);
}

void Game::vehicleUpdateUpVector()
{
	worldPhysics[activeScene].updateVehicleUpVector(scenes[activeScene].getVehicles()[activeVehicle]);
}

void Game::directionalShadowPass(int index, float delta, DRAWING_MODE mode)
{
	graphics.getShadowMappingShader().use();
	graphics.getShadowMappingShader().setInt("computeWorldPos", 0);
	graphics.getShadowMappingShader().setInt("omniDepthRendering", 0);
	graphics.getShadowMappingShader().setInt("omnilightFragDepth", 0);

	// render directional depth maps
	int sLightsOffset{0};
	for(int i{0}; i < scenes[index].getDLights().size(); ++i, ++sLightsOffset)
	{
		glViewport(0, 0,
				static_cast<int>(scenes[index].getDLights()[i]->getShadowQuality()),
				static_cast<int>(scenes[index].getDLights()[i]->getShadowQuality()));
		graphics.setStdShadowQuality(scenes[index].getDLights()[i]->getShadowQuality(), i);
		graphics.getStdDepthFBO(i)->bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightPosition = scenes[index].getDLights()[i]->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes[index].getDLights()[i]->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

		graphics.getShadowMappingShader().setMatrix("view", lightView);
		graphics.getShadowMappingShader().setMatrix("proj", graphics.getOrthoProjection(scenes[index].getDLights()[i]->getOrthoDimension()));

		// draw scene
		scenes[index].draw(graphics.getShadowMappingShader(), graphics, DRAW_TYPE::DRAW_BOTH, delta, mode);
	}

	for(int i{0}; i < scenes[index].getSLights().size(); ++i)
	{
		glViewport(0, 0,
				static_cast<int>(scenes[index].getSLights()[i]->getShadowQuality()),
				static_cast<int>(scenes[index].getSLights()[i]->getShadowQuality()));
		graphics.setStdShadowQuality(scenes[index].getSLights()[i]->getShadowQuality(), i + scenes[index].getDLights().size());
		graphics.getStdDepthFBO(sLightsOffset + i)->bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightPosition = scenes[index].getSLights()[i]->getPosition();
		glm::vec3 lightDirection = scenes[index].getSLights()[i]->getDirection();
		glm::vec3 lightTarget = lightPosition + lightDirection;
		glm::vec3 up = (lightDirection == glm::vec3(0.0f, -1.0f, 0.0f)) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, up);
		float outerCutOff = scenes[index].getSLights()[i]->getOuterCutOff();

		glm::mat4 spotProj = glm::perspective(
					outerCutOff * 2.0f, 1.0f,
					scenes[index].getActiveCamera().getNearPlane(),
					scenes[index].getActiveCamera().getFarPlane()
					);

		graphics.getShadowMappingShader().setMatrix("proj", spotProj);
		graphics.getShadowMappingShader().setMatrix("view", lightView);

		// draw scene
		scenes[index].draw(graphics.getShadowMappingShader(), graphics, DRAW_TYPE::DRAW_BOTH, delta, mode);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::omnidirectionalShadowPass(int index, float delta, DRAWING_MODE mode)
{
	graphics.getShadowMappingShader().use();
	graphics.getShadowMappingShader().setInt("computeWorldPos", 1);
	graphics.getShadowMappingShader().setInt("omniDepthRendering", 1);
	graphics.getShadowMappingShader().setInt("omnilightFragDepth", 1);

	// render omnidirectional depth maps
	std::vector<glm::mat4> omnilightViews;
	for(int i{0}; i < scenes[index].getPLights().size(); ++i)
	{
		glViewport(0, 0,
				static_cast<int>(scenes[index].getPLights()[i]->getShadowQuality()),
				static_cast<int>(scenes[index].getPLights()[i]->getShadowQuality()));
		graphics.setOmniShadowQuality(scenes[index].getPLights()[i]->getShadowQuality(), i);
		graphics.getOmniDepthFBO(i)->bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		// (proj * view)
		glm::vec3 lightPosition = scenes[index].getPLights()[i]->getPosition();
		omnilightViews.push_back(graphics.getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		omnilightViews.push_back(graphics.getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		omnilightViews.push_back(graphics.getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		omnilightViews.push_back(graphics.getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		omnilightViews.push_back(graphics.getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		omnilightViews.push_back(graphics.getOmniPerspProjection() * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		graphics.getShadowMappingShader().setVec3f("lightPosition", lightPosition);
		for(int j{0}; j < 6; ++j)
			graphics.getShadowMappingShader().setMatrix("omnilightViews[" + std::to_string(j) + "]", omnilightViews[j]);
		omnilightViews.clear();

		// draw scene
		scenes[index].draw(graphics.getShadowMappingShader(), graphics, DRAW_TYPE::DRAW_BOTH, delta, mode);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::colorMultisamplePass(int index, int width, int height, float delta, DRAWING_MODE mode, bool debug)
{
	// render to multisample framebuffer
	glViewport(0, 0, width, height);
	graphics.getMultisampleFBO()->bind();

	// get shader
	Shader s = graphics.getPBRShader();

	// draw scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	s.use();
	s.setInt("pointLightCount", scenes[index].getPLights().size());
	s.setVec3f("cam.viewPos", scenes[index].getActiveCamera().getPosition());
	s.setMatrix("view", scenes[index].getActiveCamera().getViewMatrix());
	s.setMatrix("proj", scenes[index].getActiveCamera().getProjectionMatrix());
	s.setLighting(scenes[index].getPLights(), scenes[index].getDLights(), scenes[index].getSLights());
	s.setInt("hasSSAO", graphics.ssaoOn() ? 1 : 0);
	glActiveTexture(GL_TEXTURE0 + 14);
	glBindTexture(GL_TEXTURE_2D, graphics.getAOFBO(1)->getAttachments()[0].id);
	s.setInt("ssao", 14);
	s.setVec2f("viewport", glm::vec2(width, height));
	
    // set shadow maps (point first, dir second and spot last)
	int nbPLights = scenes[index].getPLights().size();
	int nbDLights = scenes[index].getDLights().size();
	int nbSLights = scenes[index].getSLights().size();

	int textureOffset{5};
	
    // "you have to uniform all elements in samplerCube array. Otherwise, there will be a"
	// "black screen, or your clear color. Also, following draw calls may cause invalid "
	// "operation. Better to uniform all unused sampler types with some random texture index."
	for(int i{nbPLights}; i < 10; ++i)
	{
		s.setInt("omniDepthMap[" + std::to_string(i) + "]", textureOffset);
	}

    if(graphics.shadowsOn())
    {
	    for(int i{0}; i < nbPLights; ++i)
	    {
	    	glActiveTexture(GL_TEXTURE0 + textureOffset);
	    	glBindTexture(GL_TEXTURE_CUBE_MAP, graphics.getOmniDepthFBO(i)->getAttachments()[0].id);
	    	s.setInt("omniDepthMap[" + std::to_string(i) + "]", textureOffset);
	    	s.setMatrix("light[" + std::to_string(i) + "].lightSpaceMatrix", glm::mat4(1.0f));
	    	textureOffset++;
	    }

	    int depthMapIndex{0};
	    for(int i{0}; i < nbDLights; ++i)
	    {
		    glm::vec3 lightPosition = scenes[index].getDLights()[i]->getPosition();
		    glm::vec3 lightTarget = lightPosition + scenes[index].getDLights()[i]->getDirection();
		    glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

		    glActiveTexture(GL_TEXTURE0 + textureOffset);
		    glBindTexture(GL_TEXTURE_2D, graphics.getStdDepthFBO(depthMapIndex)->getAttachments()[0].id);
		    s.setInt("depthMap[" + std::to_string(depthMapIndex) + "]", textureOffset);
		    s.setMatrix("light[" + std::to_string(i + nbPLights) + "].lightSpaceMatrix", graphics.getOrthoProjection(scenes[index].getDLights()[i]->getOrthoDimension()) * lightView);
		    depthMapIndex++;
		    textureOffset++;
	    }

	    for(int i{0}; i < nbSLights; ++i)
	    {
		    float outerCutOff = scenes[index].getSLights()[i]->getOuterCutOff();
		    glm::vec3 lightPosition = scenes[index].getSLights()[i]->getPosition();
		    glm::vec3 lightTarget = lightPosition + scenes[index].getSLights()[i]->getDirection();
		    glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));
		    glm::mat4 spotProj = glm::perspective(
                    outerCutOff * 2.0f, 1.0f,
					scenes[index].getActiveCamera().getNearPlane(),
					scenes[index].getActiveCamera().getFarPlane()
					);

		    glActiveTexture(GL_TEXTURE0 + textureOffset);
		    glBindTexture(GL_TEXTURE_2D, graphics.getStdDepthFBO(depthMapIndex)->getAttachments()[0].id);
		    s.setInt("depthMap[" + std::to_string(depthMapIndex) + "]", textureOffset);
		    s.setMatrix("light[" + std::to_string(i + nbPLights + nbDLights) + "].lightSpaceMatrix", spotProj * lightView);
		    depthMapIndex++;
		    textureOffset++;
	    }
    }

	scenes[index].draw(s, graphics, DRAW_TYPE::DRAW_OPAQUE, delta, mode, debug);
	scenes[index].draw(s, graphics, DRAW_TYPE::DRAW_TRANSPARENT, delta, mode, debug);

    // blit to normal framebuffer (resolve multisampling)
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	graphics.getMultisampleFBO()->blitFramebuffer(graphics.getNormalFBO(0), width, height);
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	graphics.getMultisampleFBO()->blitFramebuffer(graphics.getNormalFBO(1), width, height);
}

void Game::bloomPass(int width, int height, std::unique_ptr<Framebuffer>& in, int attachmentIndex, GLuint out)
{
	bool firstIteration{true};
	Shader & downSampling = graphics.getDownSamplingShader();
	Shader & upSampling = graphics.getUpSamplingShader();
	Shader & gaussianBlur = graphics.getGaussianBlurShader();
	Shader & tentBlur = graphics.getTentBlurShader();

	// downsampling and blurring
	for(int i{0}; i < 6; ++i)
	{
		downSampling.use();
		int factor = std::pow(2, i+1);
		glViewport(0, 0, width / factor, height / factor);
		std::unique_ptr<Framebuffer> & fbo = graphics.getDownSamplingFBO(i);
		fbo->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		downSampling.setInt("image", 0);
		glActiveTexture(GL_TEXTURE0);
		if(firstIteration)
		{
			firstIteration = false;
			glBindTexture(GL_TEXTURE_2D, in->getAttachments()[attachmentIndex].id);
		}
		else
			glBindTexture(GL_TEXTURE_2D, graphics.getPingPongFBO(i*2-1)->getAttachments()[0].id);
		graphics.getQuadMesh()->draw(downSampling);
		
        // apply horizontal gaussian blur
		gaussianBlur.use();
		std::unique_ptr<Framebuffer> & ping = graphics.getPingPongFBO(i*2);
		ping->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		gaussianBlur.setInt("image", 0);
		gaussianBlur.setInt("blurSize", graphics.getBloomSize());
		gaussianBlur.setFloat("sigma", graphics.getBloomSigma());
		gaussianBlur.setInt("direction", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, graphics.getDownSamplingFBO(i)->getAttachments()[0].id);
		graphics.getQuadMesh()->draw(gaussianBlur);

		// apply vertical gaussian blur
		std::unique_ptr<Framebuffer> & pong = graphics.getPingPongFBO(i*2+1);
		pong->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		gaussianBlur.setInt("image", 0);
		gaussianBlur.setInt("blurSize", graphics.getBloomSize());
		gaussianBlur.setFloat("sigma", graphics.getBloomSigma());
		gaussianBlur.setInt("direction", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, graphics.getPingPongFBO(i*2)->getAttachments()[0].id);
		graphics.getQuadMesh()->draw(gaussianBlur);
    }

	// upsampling
	firstIteration = true;
	for(int i{0}; i < 6; ++i)
	{
		upSampling.use();
        upSampling.setInt("merge_to_current_FBO", 0);
		int factor = std::pow(2, 5-i);
		glViewport(0, 0, width / factor, height / factor);
		std::unique_ptr<Framebuffer> & mergeFBO = graphics.getUpSamplingFBO(i*2);
		mergeFBO->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		upSampling.setInt("low_res", 0);
		upSampling.setInt("high_res", 1);
		glActiveTexture(GL_TEXTURE0);
		if(firstIteration)
		{
			firstIteration = false;
			glBindTexture(GL_TEXTURE_2D, graphics.getPingPongFBO((5-i)*2+1)->getAttachments()[0].id);
		}
		else
			glBindTexture(GL_TEXTURE_2D, graphics.getUpSamplingFBO((i-1)*2+1)->getAttachments()[0].id);
		glActiveTexture(GL_TEXTURE1);
		if(4-i == -1)
			glBindTexture(GL_TEXTURE_2D, in->getAttachments()[attachmentIndex].id);
		else
			glBindTexture(GL_TEXTURE_2D, graphics.getPingPongFBO((4-i)*2+1)->getAttachments()[0].id);
		graphics.getQuadMesh()->draw(upSampling);

		// apply tent filter
		tentBlur.use();
		std::unique_ptr<Framebuffer> & tentFBO = graphics.getUpSamplingFBO(i*2+1);
		tentFBO->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		tentBlur.setInt("image", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, graphics.getUpSamplingFBO(i*2)->getAttachments()[0].id);
		graphics.getQuadMesh()->draw(tentBlur);
	}
	glCopyImageSubData(graphics.getUpSamplingFBO(11)->getAttachments()[0].id, GL_TEXTURE_2D, 0, 0, 0, 0, out, GL_TEXTURE_2D, 0, 0, 0, 0, width, height, 1);
}

void Game::GBufferPass(int index, int width, int height, float delta)
{
	graphics.getGBufferFBO()->bind();
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render position, normal, and depth data
	graphics.getGBufferShader().use();
	graphics.getGBufferShader().setMatrix("view", scenes[index].getActiveCamera().getViewMatrix());
	graphics.getGBufferShader().setMatrix("proj", scenes[index].getActiveCamera().getProjectionMatrix());
	scenes[index].draw(graphics.getGBufferShader(), graphics, DRAW_TYPE::DRAW_BOTH, delta);
}

void Game::ssaoPass(int index, int width, int height, float delta)
{
	graphics.getAOFBO(0)->bind();
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

    Shader & AOShader{graphics.getAOShader()};
	AOShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics.getGBufferFBO()->getAttachments()[0].id); // position (view space)
	AOShader.setInt("positionBuffer", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, graphics.getGBufferFBO()->getAttachments()[1].id); // normal
	AOShader.setInt("normalBuffer", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, graphics.getAONoiseTexture()); // noise texture
	AOShader.setInt("noiseTexture", 2);
    AOShader.setInt("kernelSize", graphics.getAOSampleCount());
	std::vector<glm::vec3> & aoKernel{graphics.getAOKernel()};
	for(int i{0}; i < graphics.getAOSampleCount(); ++i)
		AOShader.setVec3f("samples[" + std::to_string(i) + "]", aoKernel[i]);
	AOShader.setFloat("radius", graphics.getAORadius());
	AOShader.setFloat("bias", 0.05f);
	AOShader.setMatrix("projection", scenes[index].getActiveCamera().getProjectionMatrix());
	AOShader.setFloat("screenWidth", static_cast<float>(width));
	AOShader.setFloat("screenHeight", static_cast<float>(height));
	graphics.getQuadMesh()->draw(AOShader);

	graphics.getAOFBO(1)->bind();
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	graphics.getAOBlurShader().use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics.getAOFBO(0)->getAttachments()[0].id); // raw AO
	graphics.getAOBlurShader().setInt("aoInput", 0);
	graphics.getQuadMesh()->draw(graphics.getAOBlurShader());

	// reset clear color
	glClearColor(LIGHT_GREY[0], LIGHT_GREY[1], LIGHT_GREY[2], LIGHT_GREY[3]);
}

void Game::volumetricsPass(int index, int width, int height, float delta, double elapsedTime)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Downsample GBuffer frag world pos map (subsample by 2)
	graphics.getVolumetricsFBO(0)->bind();
	glViewport(0, 0, width/2, height/2);
	glClear(GL_COLOR_BUFFER_BIT);

    Shader VLDownSample = graphics.getVolumetricDownSamplingShader();
    VLDownSample.use();
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics.getGBufferFBO()->getAttachments()[3].id); // frag world position
    VLDownSample.setInt("fragWorldPos", 0);
	
    graphics.getQuadMesh()->draw(VLDownSample);

    // Render volumetric lighting
	graphics.getVolumetricsFBO(1)->bind();
	glClear(GL_COLOR_BUFFER_BIT);

	Shader s = graphics.getVolumetricLightingShader();
	
	// set shader data
	s.use();
	s.setInt("pointLightCount", scenes[index].getPLights().size());
	s.setVec3f("cam.viewPos", scenes[index].getActiveCamera().getPosition());
	s.setFloat("cam.near_plane", scenes[index].getActiveCamera().getNearPlane());
	s.setFloat("cam.far_plane", scenes[index].getActiveCamera().getFarPlane());
	glm::mat4 inv_viewProj = glm::inverse(scenes[index].getActiveCamera().getProjectionMatrix() * scenes[index].getActiveCamera().getViewMatrix());
	s.setMatrix("cam.inv_viewProj", inv_viewProj);
	glActiveTexture(GL_TEXTURE0 + 10);
	glBindTexture(GL_TEXTURE_2D, graphics.getGBufferFBO()->getAttachments()[2].id); // depth of each fragment
	s.setInt("cam.depthMap", 10);
	glActiveTexture(GL_TEXTURE0 + 11);
	glBindTexture(GL_TEXTURE_2D, graphics.getVolumetricsFBO(0)->getAttachments()[0].id); // world position of each fragment
	s.setInt("worldPosMap", 11);
	s.setInt("N", 50);
	s.setFloat("time", elapsedTime);

	s.setLighting(scenes[index].getPLights(), scenes[index].getDLights(), scenes[index].getSLights());

	// set shadow maps (point first, dir second and spot last)
	int nbPLights = scenes[index].getPLights().size();
	int nbDLights = scenes[index].getDLights().size();
	int nbSLights = scenes[index].getSLights().size();

	for(int i{0}; i < nbPLights; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, graphics.getOmniDepthFBO(i)->getAttachments()[0].id);
		s.setInt("omniDepthMap[" + std::to_string(i) + "]", i);
		s.setMatrix("light[" + std::to_string(i) + "].lightSpaceMatrix", glm::mat4(1.0f));
	}
	// "you have to uniform all elements in samplerCube array. Otherwise, there will be a"
	// "black screen, or your clear color. Also, following draw calls may cause invalid "
	// "operation. Better to uniform all unused sampler types with some random texture index."
	for(int i{nbPLights}; i < 10; ++i)
	{
		s.setInt("omniDepthMap[" + std::to_string(i) + "]", nbPLights);
	}

	for(int i{0}; i < nbDLights; ++i)
	{
		glm::vec3 lightPosition = scenes[index].getDLights()[i]->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes[index].getDLights()[i]->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));

		glActiveTexture(GL_TEXTURE0 + nbPLights + i);
		glBindTexture(GL_TEXTURE_2D, graphics.getStdDepthFBO(i)->getAttachments()[0].id);
		s.setInt("depthMap[" + std::to_string(i) + "]", nbPLights + i);
		s.setMatrix("light[" + std::to_string(i + nbPLights) + "].lightSpaceMatrix", graphics.getOrthoProjection(scenes[index].getDLights()[i]->getOrthoDimension()) * lightView);
	}

	for(int i{0}; i < nbSLights; ++i)
	{
		float outerCutOff = scenes[index].getSLights()[i]->getOuterCutOff();
		glm::vec3 lightPosition = scenes[index].getSLights()[i]->getPosition();
		glm::vec3 lightTarget = lightPosition + scenes[index].getSLights()[i]->getDirection();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 spotProj = glm::perspective(
					outerCutOff * 2.0f, 1.0f,
					scenes[index].getActiveCamera().getNearPlane(),
					scenes[index].getActiveCamera().getFarPlane()
					);

		glActiveTexture(GL_TEXTURE0 + nbPLights + nbDLights + i);
		glBindTexture(GL_TEXTURE_2D, graphics.getStdDepthFBO(nbDLights + i)->getAttachments()[0].id);
		s.setInt("depthMap[" + std::to_string(nbDLights + i) + "]", nbPLights + nbDLights + i);
		s.setMatrix("light[" + std::to_string(i + nbPLights + nbDLights) + "].lightSpaceMatrix", spotProj * lightView);
	}
	
	graphics.getQuadMesh()->draw(s);

    // Bilateral blur
	graphics.getVolumetricsFBO(2)->bind();
	glClear(GL_COLOR_BUFFER_BIT);

    Shader & bilateralBlur = graphics.getBilateralBlurShader();
    bilateralBlur.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, graphics.getVolumetricsFBO(1)->getAttachments()[0].id);
    bilateralBlur.setInt("image", 0);
    bilateralBlur.setInt("kernelSize", 5);
    bilateralBlur.setFloat("sigma", 1.5f);
    bilateralBlur.setInt("direction", 0); // horizontal
    graphics.getQuadMesh()->draw(bilateralBlur);
	
    graphics.getVolumetricsFBO(1)->bind();
	glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, graphics.getVolumetricsFBO(2)->getAttachments()[0].id);
    bilateralBlur.setInt("direction", 1); // vertical
    graphics.getQuadMesh()->draw(bilateralBlur);
		
    // Upsample result to screen resolution
	graphics.getVolumetricsFBO(3)->bind();
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	Shader & VLUpSample = graphics.getUpSamplingShader();
    VLUpSample.use();
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics.getVolumetricsFBO(1)->getAttachments()[0].id);
    VLUpSample.setInt("low_res", 0);
    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, graphics.getVolumetricsFBO(1)->getAttachments()[0].id);
    VLUpSample.setInt("high_res", 1);
    VLUpSample.setInt("merge_to_current_FBO", 1);
	
    graphics.getQuadMesh()->draw(VLUpSample);

    // reset clear color
	glClearColor(LIGHT_GREY[0], LIGHT_GREY[1], LIGHT_GREY[2], LIGHT_GREY[3]);
}

void Game::motionBlurPass(int index, int width, int height)
{
    Camera& cam{scenes[index].getActiveCamera()};
    glm::mat4 view = cam.getViewMatrix();
    glm::mat4 prev_view = cam.getPreviousViewMatrix();
    glm::mat4 proj = cam.getProjectionMatrix();
    Shader & shader = graphics.motionBlur;
    
    graphics.motionBlurFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    shader.setMatrix("curr_MVP", proj * view);
    shader.setMatrix("prev_MVP", proj * prev_view);
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics.getGBufferFBO()->getAttachments()[3].id); // frag world position
    shader.setInt("worldPos", 0);
    graphics.quad->draw(shader);
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
	if (graphics.bloomOn())
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, graphics.getBloomTexture(0));
		s.setInt("bloom", 1);
		s.setInt("bloomEffect", 1);
	}
	else
	{
		s.setInt("bloomEffect", 0);
	}
	if (graphics.volumetricLightingOn())
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, graphics.getVolumetricsFBO(3)->getAttachments()[0].id);
		s.setInt("volumetrics", 2);
		s.setInt("volumetricsOn", 1);
	}
	else
	{
		s.setInt("volumetricsOn", 0);
	}
	if (graphics.motionBlurFX)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, graphics.motionBlurFBO->getAttachments()[0].id);
		s.setInt("motionBlur", 3);
		s.setInt("motionBlurOn", 1);
		s.setInt("motionBlurStrength", graphics.motionBlurStrength);
	}
	else
	{
		s.setInt("motionBlurOn", 0);
	}

	s.setInt("tone_mapping", static_cast<int>(graphics.get_scene_tone_mapping()));
	graphics.getQuadMesh()->draw(s);
}

void Game::uiCompositing()
{
	graphics.compositeFBO[1]->bind();
	glClear(GL_COLOR_BUFFER_BIT);

	Shader& s{ graphics.uiCompositing };
	s.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics.userInterfaceFBO->getAttachments()[0].id);
	s.setInt("ui", 0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, graphics.getBloomTexture(1));
	s.setInt("uiBloom", 1);
	
	s.setInt("tone_mapping", static_cast<int>(graphics.get_ui_tone_mapping()));
	graphics.getQuadMesh()->draw(s);
}