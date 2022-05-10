#include "game.hpp"

void Writer::write(char* c, std::bitset<10>& userInputs, float delta, int boundX, glm::vec3 cursor_shape)
{
	std::string character(c);
	if (character.size() > 1) {
		return;
	}

	WRITE_ACTION writeAction;
	if (userInputs.test(6))
		writeAction = WRITE_ACTION::ERASE;
	else if (userInputs.test(8))
		writeAction = WRITE_ACTION::CURSOR_LEFT;
	else if (userInputs.test(7))
		writeAction = WRITE_ACTION::CURSOR_RIGHT;
	else if (character == "")
		writeAction = WRITE_ACTION::NOTHING;
	else
		writeAction = WRITE_ACTION::CHARACTER;

	write_aux(writeAction, character, delta, boundX, cursor_shape);
}

void Writer::write_aux(WRITE_ACTION writeAction, std::string& character, float delta, int boundX, glm::vec3 cursor_shape)
{
	if (writeAction == WRITE_ACTION::NOTHING)
	{
		m_lastCharacter = "";
		m_deltaWrite = 0.0f;
		m_lastWriteAction = writeAction;
		return;
	}
	if (m_lastWriteAction == writeAction || (writeAction == WRITE_ACTION::CHARACTER && character == m_lastCharacter))
	{
		m_deltaWrite += delta;
		if (m_deltaWrite < 1.0f)
			return;
		else
		{
			if (m_deltaWrite < 1.05f)
				return;
			else
				m_deltaWrite = 1.0f;
		}
	}
	if (writeAction == WRITE_ACTION::CHARACTER)
	{
		if (character != m_lastCharacter)
		{
			m_lastCharacter = character;
			m_deltaWrite = 0.0f;
		}
		if (boundX > (cursor_shape.x + cursor_shape.z))
		{
			m_textInput[m_cursor.m_focus].insert(m_cursor.m_pos, character.data(), character.size());
			m_textSectionsWidth[m_cursor.m_focus].insert(m_textSectionsWidth[m_cursor.m_focus].begin() + m_cursor.m_pos, character.size());
			m_cursor.m_pos++;
		}
	}
	else if (writeAction == WRITE_ACTION::ERASE)
	{
		if (m_textInput[m_cursor.m_focus].size() > 0 && m_cursor.m_pos > 0)
		{
			int sectionWidth = m_textSectionsWidth[m_cursor.m_focus][m_cursor.m_pos - 1];
			m_textSectionsWidth[m_cursor.m_focus].erase(m_textSectionsWidth[m_cursor.m_focus].begin() + m_cursor.m_pos - 1);
			m_textInput[m_cursor.m_focus].erase(m_cursor.m_pos - sectionWidth, sectionWidth);
			m_cursor.m_pos -= sectionWidth;
		}
	}
	else if (writeAction == WRITE_ACTION::CURSOR_LEFT)
	{
		m_cursor.m_pos = max(0, m_cursor.m_pos - 1);
	}
	else if (writeAction == WRITE_ACTION::CURSOR_RIGHT)
	{
		if (m_cursor.m_pos < m_textInput[m_cursor.m_focus].size())
			m_cursor.m_pos = min(m_textInput[m_cursor.m_focus].size(), m_cursor.m_pos + 1);
	}

	m_lastWriteAction = writeAction;
}

Game::Game(int clientWidth, int clientHeight) :
	activeScene(0),
	activeVehicle(-1),
	graphics(clientWidth, clientHeight),
    textRenderer(std::make_unique<Text>(clientWidth, clientHeight)),
	m_cards(clientWidth, clientHeight),
	m_board(clientWidth, clientHeight, graphics.aspect_ratio),
	m_fruit(-1),
	m_writer(clientWidth, clientHeight)
{
	// create mouse
	int mouse_pos[2];
	SDL_GetMouseState(&mouse_pos[0], &mouse_pos[1]);
	int mouse_size[2] = { 25,25 };
	m_mouse = std::make_unique<Mouse>(mouse_pos, mouse_size, "assets/mouse/normal.tga", "assets/mouse/hover.tga", clientWidth, clientHeight);
	m_mouse->activate();

	// load some fonts and set an active font
    textRenderer->load_police("assets/fonts/ebrima.ttf", 20);
    textRenderer->use_police(0);

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

	// init
	createUI(clientWidth, clientHeight);
}

void Game::createUI(int width, int height)
{
	// home page
	m_ui.add_page();

	Page& home_page = m_ui.get_page(0);
	home_page.add_layer(0);
	home_page.add_layer(1); // emphase sur le nom de la partie du visage en cours d'édition
	home_page.add_layer(2); // menu déroulant "partie du visage"
	home_page.add_layer(3); // options visage
	home_page.add_layer(4); // options cheveux (homme)
	home_page.add_layer(5); // options cheveux (femme)
	home_page.add_layer(6); // options yeux (homme)
	home_page.add_layer(7); // options yeux (femme)
	home_page.add_layer(8); // options bouche
	home_page.add_layer(9); // options sexe
	home_page.add_layer(10); // avatar
	home_page.add_layer(11); // choix de couleur
	home_page.add_layer(12); // user inputs (pseudo, connexion, chercher un adversaire)
	
	Layer& h_layer0 = home_page.get_layer(0);
	h_layer0.add_sprite(0, glm::vec2(0.0f), glm::vec2(width, height), width, height);
	h_layer0.get_sprite(0)->set_background_img("assets/home.tga");
	h_layer0.get_sprite(0)->use_background_img();

	Layer& h_layer1 = home_page.get_layer(1);
	h_layer1.set_visibility(false);
	h_layer1.add_sprite(1, glm::vec2(400, 728-(140+48)), glm::vec2(400, 48), width, height);
	h_layer1.get_sprite(1)->set_background_img("assets/avatar/face_part_highlight.tga");
	h_layer1.get_sprite(1)->use_background_img();

	Layer& h_layer2 = home_page.get_layer(2);
	h_layer2.add_sprite(2, glm::vec2(600, 728 - (140 + 48 - 12)), glm::vec2(150, 24), width, height);
	h_layer2.get_sprite(2)->set_background_img("assets/avatar/visage.tga");
	h_layer2.get_sprite(2)->set_background_img_selected("assets/avatar/visage_hover.tga");
	h_layer2.get_sprite(2)->use_background_img();
	h_layer2.get_sprite(2)->select();
	// >>>>> highlight
	h_layer1.get_sprite(1)->set_pos(glm::vec2(400, 728 - 140));
	h_layer1.set_visibility(true);
	// <<<<< highlight
	h_layer2.add_sprite(3, glm::vec2(600, 728 - (140 + 48*2 - 12)), glm::vec2(150, 24), width, height);
	h_layer2.get_sprite(3)->set_background_img("assets/avatar/cheveux.tga");
	h_layer2.get_sprite(3)->set_background_img_selected("assets/avatar/cheveux_hover.tga");
	h_layer2.get_sprite(3)->use_background_img();
	h_layer2.add_sprite(4, glm::vec2(600, 728 - (140 + 48*3 - 12)), glm::vec2(150, 24), width, height);
	h_layer2.get_sprite(4)->set_background_img("assets/avatar/yeux.tga");
	h_layer2.get_sprite(4)->set_background_img_selected("assets/avatar/yeux_hover.tga");
	h_layer2.get_sprite(4)->use_background_img();
	h_layer2.add_sprite(5, glm::vec2(600, 728 - (140 + 48*4 - 12)), glm::vec2(150, 24), width, height);
	h_layer2.get_sprite(5)->set_background_img("assets/avatar/bouche.tga");
	h_layer2.get_sprite(5)->set_background_img_selected("assets/avatar/bouche_hover.tga");
	h_layer2.get_sprite(5)->use_background_img();
	h_layer2.add_sprite(6, glm::vec2(600, 728 - (140 + 48*5 - 12)), glm::vec2(150, 24), width, height);
	h_layer2.get_sprite(6)->set_background_img("assets/avatar/sexe.tga");
	h_layer2.get_sprite(6)->set_background_img_selected("assets/avatar/sexe_hover.tga");
	h_layer2.get_sprite(6)->use_background_img();

	Layer& h_layer3 = home_page.get_layer(3);
	h_layer3.set_visibility(false);
	h_layer3.add_sprite(7, glm::vec2(610, 728 - (140 + 48 + 12)), glm::vec2(130, 24), width, height);
	h_layer3.get_sprite(7)->set_background_img("assets/avatar/visage_normal.tga");
	h_layer3.get_sprite(7)->set_background_img_selected("assets/avatar/visage_normal_hover.tga");
	h_layer3.get_sprite(7)->use_background_img();

	Layer& h_layer4 = home_page.get_layer(4);
	h_layer4.set_visibility(false);
	h_layer4.add_sprite(8, glm::vec2(610, 728 - (140 + 48 * 2 + 12)), glm::vec2(130, 24), width, height);
	h_layer4.get_sprite(8)->set_background_img("assets/avatar/cheveux_herisson.tga");
	h_layer4.get_sprite(8)->set_background_img_selected("assets/avatar/cheveux_herisson_hover.tga");
	h_layer4.get_sprite(8)->use_background_img();
	h_layer4.add_sprite(9, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 24)), glm::vec2(130, 24), width, height);
	h_layer4.get_sprite(9)->set_background_img("assets/avatar/cheveux_decoiffe.tga");
	h_layer4.get_sprite(9)->set_background_img_selected("assets/avatar/cheveux_decoiffe_hover.tga");
	h_layer4.get_sprite(9)->use_background_img();
	h_layer4.add_sprite(10, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 48)), glm::vec2(130, 24), width, height);
	h_layer4.get_sprite(10)->set_background_img("assets/avatar/cheveux_meche_avant.tga");
	h_layer4.get_sprite(10)->set_background_img_selected("assets/avatar/cheveux_meche_avant_hover.tga");
	h_layer4.get_sprite(10)->use_background_img();
	h_layer4.add_sprite(11, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 72)), glm::vec2(130, 24), width, height);
	h_layer4.get_sprite(11)->set_background_img("assets/avatar/cheveux_mixte.tga");
	h_layer4.get_sprite(11)->set_background_img_selected("assets/avatar/cheveux_mixte_hover.tga");
	h_layer4.get_sprite(11)->use_background_img();
	h_layer4.add_sprite(12, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 96)), glm::vec2(130, 24), width, height);
	h_layer4.get_sprite(12)->set_background_img("assets/avatar/cheveux_arriere.tga");
	h_layer4.get_sprite(12)->set_background_img_selected("assets/avatar/cheveux_arriere_hover.tga");
	h_layer4.get_sprite(12)->use_background_img();

	Layer& h_layer5 = home_page.get_layer(5);
	h_layer5.set_visibility(false);
	h_layer5.add_sprite(13, glm::vec2(610, 728 - (140 + 48 * 2 + 12)), glm::vec2(130, 24), width, height);
	h_layer5.get_sprite(13)->set_background_img("assets/avatar/cheveux_mixte.tga");
	h_layer5.get_sprite(13)->set_background_img_selected("assets/avatar/cheveux_mixte_hover.tga");
	h_layer5.get_sprite(13)->use_background_img();
	h_layer5.add_sprite(14, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 24)), glm::vec2(130, 24), width, height);
	h_layer5.get_sprite(14)->set_background_img("assets/avatar/cheveux_mi_long.tga");
	h_layer5.get_sprite(14)->set_background_img_selected("assets/avatar/cheveux_mi_long_hover.tga");
	h_layer5.get_sprite(14)->use_background_img();
	h_layer5.add_sprite(15, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 48)), glm::vec2(130, 24), width, height);
	h_layer5.get_sprite(15)->set_background_img("assets/avatar/cheveux_frange.tga");
	h_layer5.get_sprite(15)->set_background_img_selected("assets/avatar/cheveux_frange_hover.tga");
	h_layer5.get_sprite(15)->use_background_img();
	h_layer5.add_sprite(16, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 72)), glm::vec2(130, 24), width, height);
	h_layer5.get_sprite(16)->set_background_img("assets/avatar/cheveux_au_bol.tga");
	h_layer5.get_sprite(16)->set_background_img_selected("assets/avatar/cheveux_au_bol_hover.tga");
	h_layer5.get_sprite(16)->use_background_img();
	h_layer5.add_sprite(17, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 96)), glm::vec2(130, 24), width, height);
	h_layer5.get_sprite(17)->set_background_img("assets/avatar/queue_cheval.tga");
	h_layer5.get_sprite(17)->set_background_img_selected("assets/avatar/queue_cheval_hover.tga");
	h_layer5.get_sprite(17)->use_background_img();

	Layer& h_layer6 = home_page.get_layer(6);
	h_layer6.set_visibility(false);
	h_layer6.add_sprite(18, glm::vec2(610, 728 - (140 + 48 * 3 + 12)), glm::vec2(130, 24), width, height);
	h_layer6.get_sprite(18)->set_background_img("assets/avatar/yeux_manga.tga");
	h_layer6.get_sprite(18)->set_background_img_selected("assets/avatar/yeux_manga_hover.tga");
	h_layer6.get_sprite(18)->use_background_img();
	h_layer6.add_sprite(19, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 24)), glm::vec2(130, 24), width, height);
	h_layer6.get_sprite(19)->set_background_img("assets/avatar/yeux_amande.tga");
	h_layer6.get_sprite(19)->set_background_img_selected("assets/avatar/yeux_amande_hover.tga");
	h_layer6.get_sprite(19)->use_background_img();

	Layer& h_layer7 = home_page.get_layer(7);
	h_layer7.set_visibility(false);
	h_layer7.add_sprite(20, glm::vec2(610, 728 - (140 + 48 * 3 + 12)), glm::vec2(130, 24), width, height);
	h_layer7.get_sprite(20)->set_background_img("assets/avatar/yeux_manga.tga");
	h_layer7.get_sprite(20)->set_background_img_selected("assets/avatar/yeux_manga_hover.tga");
	h_layer7.get_sprite(20)->use_background_img();
	h_layer7.add_sprite(21, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 24)), glm::vec2(130, 24), width, height);
	h_layer7.get_sprite(21)->set_background_img("assets/avatar/yeux_egypte.tga");
	h_layer7.get_sprite(21)->set_background_img_selected("assets/avatar/yeux_egypte_hover.tga");
	h_layer7.get_sprite(21)->use_background_img();
	h_layer7.add_sprite(22, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 48)), glm::vec2(130, 24), width, height);
	h_layer7.get_sprite(22)->set_background_img("assets/avatar/yeux_mascara.tga");
	h_layer7.get_sprite(22)->set_background_img_selected("assets/avatar/yeux_mascara_hover.tga");
	h_layer7.get_sprite(22)->use_background_img();

	Layer& h_layer8 = home_page.get_layer(8);
	h_layer8.set_visibility(false);
	h_layer8.add_sprite(23, glm::vec2(610, 728 - (140 + 48 * 4 + 12)), glm::vec2(130, 24), width, height);
	h_layer8.get_sprite(23)->set_background_img("assets/avatar/bouche_petite.tga");
	h_layer8.get_sprite(23)->set_background_img_selected("assets/avatar/bouche_petite_hover.tga");
	h_layer8.get_sprite(23)->use_background_img();
	h_layer8.add_sprite(24, glm::vec2(610, 728 - (140 + 48 * 4 + 12 + 24)), glm::vec2(130, 24), width, height);
	h_layer8.get_sprite(24)->set_background_img("assets/avatar/bouche_moyenne.tga");
	h_layer8.get_sprite(24)->set_background_img_selected("assets/avatar/bouche_moyenne_hover.tga");
	h_layer8.get_sprite(24)->use_background_img();
	h_layer8.add_sprite(25, glm::vec2(610, 728 - (140 + 48 * 4 + 12 + 48)), glm::vec2(130, 24), width, height);
	h_layer8.get_sprite(25)->set_background_img("assets/avatar/bouche_grande.tga");
	h_layer8.get_sprite(25)->set_background_img_selected("assets/avatar/bouche_grande_hover.tga");
	h_layer8.get_sprite(25)->use_background_img();

	Layer& h_layer9 = home_page.get_layer(9);
	h_layer9.set_visibility(false);
	h_layer9.add_sprite(26, glm::vec2(610, 728 - (140 + 48 * 5 + 12)), glm::vec2(130, 24), width, height);
	h_layer9.get_sprite(26)->set_background_img("assets/avatar/sexe_homme.tga");
	h_layer9.get_sprite(26)->set_background_img_selected("assets/avatar/sexe_homme_hover.tga");
	h_layer9.get_sprite(26)->use_background_img();
	h_layer9.get_sprite(26)->select();
	h_layer9.add_sprite(27, glm::vec2(610, 728 - (140 + 48 * 5 + 12 + 24)), glm::vec2(130, 24), width, height);
	h_layer9.get_sprite(27)->set_background_img("assets/avatar/sexe_femme.tga");
	h_layer9.get_sprite(27)->set_background_img_selected("assets/avatar/sexe_femme_hover.tga");
	h_layer9.get_sprite(27)->use_background_img();

	Layer& h_layer10 = home_page.get_layer(10);
	h_layer10.add_sprite(28, glm::vec2(250, 728 - (140 + 48 * 5 + 12 + 24)), glm::vec2(300, 300), width, height);
	h_layer10.get_sprite(28)->set_background_img_gl(graphics.avatarFBO->getAttachments()[0].id);
	h_layer10.get_sprite(28)->use_background_img_gl();

	Layer& h_layer11 = home_page.get_layer(11);
	h_layer11.add_sprite(29, glm::vec2(225, 728 - 285), glm::vec2(50, 50), width, height);
	h_layer11.get_sprite(29)->set_background_img("assets/avatar/couleur_left.tga");
	h_layer11.get_sprite(29)->use_background_img();
	h_layer11.add_sprite(30, glm::vec2(525, 728 - 285), glm::vec2(50, 50), width, height);
	h_layer11.get_sprite(30)->set_background_img("assets/avatar/couleur_right.tga");
	h_layer11.get_sprite(30)->use_background_img();
	h_layer11.add_sprite(31, glm::vec2(1050 - 60, 728 - 65), glm::vec2(50, 50), width, height);
	h_layer11.get_sprite(31)->set_background_img("assets/off.tga");
	h_layer11.get_sprite(31)->use_background_img();

	Layer& h_layer12 = home_page.get_layer(12);
	h_layer12.add_sprite(32, glm::vec2(525 - 75, 728 - (140 + 48 * 7 - 12)), glm::vec2(150, 30), width, height);
	h_layer12.get_sprite(32)->set_background_img("assets/pseudo.tga");
	h_layer12.get_sprite(32)->set_background_img_selected("assets/pseudo_hover.tga");
	h_layer12.get_sprite(32)->use_background_img();
	h_layer12.add_sprite(33, glm::vec2(525 - 75, 728 - (140 + 48 * 8 - 12)), glm::vec2(150, 30), width, height);
	h_layer12.get_sprite(33)->set_background_img("assets/connexion.tga");
	h_layer12.get_sprite(33)->set_background_img_selected("assets/connexion_hover.tga");
	h_layer12.get_sprite(33)->use_background_img();

	// game page
	m_ui.add_page();
	
	Page& game_page = m_ui.get_page(1);
	game_page.add_layer(0);

	Layer& g_layer0 = game_page.get_layer(0);
	g_layer0.add_sprite(0, glm::vec2(0.0f), glm::vec2(width, height), width, height);
	g_layer0.get_sprite(0)->set_background_img("assets/game.tga");
	g_layer0.get_sprite(0)->use_background_img();

	// active page
	m_ui.set_active_page(0);
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
		/*
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
		*/
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
	// draw avatar
	graphics.avatarFBO->bind();
	glViewport(0, 0, 512, 512);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_avatar.draw();

	// draw UI
    graphics.userInterfaceFBO->bind();
	glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	m_ui.get_page(m_ui.get_active_page()).draw();

	// draw text
	if (m_ui.get_active_page() == 0) {
		textRenderer->print(m_writer.m_textInput[0], 525 - 72, 272, 1, glm::vec3(0));
		// draw cursor
		if (m_writer.m_cursor.m_focus == 0)
		{
			glm::vec3 cursor_shape = textRenderer->get_cursor_shape(m_writer.m_textInput[0], 525 - 72, 272, 1, m_writer.m_cursor.m_pos);
			m_writer.m_cursor.draw(cursor_shape, delta);
		}
	}
	else if (m_ui.get_active_page() == 1) {
		textRenderer->print(m_writer.m_textInput[1], 0, 0, 1, glm::vec3(0));
	}

	// mouse
	if (m_mouse && m_mouse->is_active())
	{
		m_mouse->update_position();
		m_mouse->draw();
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
}

void Game::updateSceneActiveCameraView(int index, const std::bitset<10> & inputs, std::array<int, 3> & mouse, float delta)
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

int Game::getCursorFocus()
{
	return m_writer.m_cursor.m_focus;
}

Writer& Game::get_writer()
{
	return m_writer;
}

void Game::updateUI(std::bitset<10>& inputs, char* text_input, int screenW, int screenH, float delta)
{
	int* mouse_pos = m_mouse->get_position();
	int* mouse_size = m_mouse->get_size();
	m_mouse->use_normal();
	std::shared_ptr<Sprite> hovered = m_ui.get_hovered_sprite(mouse_pos[0], mouse_pos[1]);

	if (!hovered)
		return;
	if (m_ui.get_active_page() == 0) // home
	{
		Page& home_page{m_ui.get_page(0)};
		int sprite_id{ hovered->get_id() };

		if (sprite_id >= 2 && sprite_id <= 6) // hovered a face feature
		{
			// change sprite texture to selected/hover texture
			home_page.get_layer(2).get_sprite(sprite_id)->use_background_img_selected();
			for (int i{ 2 }; i < 7; ++i)
				if (i != sprite_id)
					home_page.get_layer(2).get_sprite(i)->use_background_img();
		}
		else
		{
			// reset to normal texture
			for (int i{ 2 }; i < 7; ++i)
				home_page.get_layer(2).get_sprite(i)->use_background_img();
		}
		if (sprite_id >= 7 && sprite_id <= 27) // hovered a face feature option
		{
			// change sprite texture to selected/hover texture
			Layer& layer{home_page.get_layer(hovered->get_layer_id())};
			layer.get_sprite(sprite_id)->use_background_img_selected();
			for (int i{ 7 }; i < 29; ++i)
			{
				if (i == sprite_id)
					continue;
				if (i == 7) // layer 3
					home_page.get_layer(3).get_sprite(i)->use_background_img();
				else if (i >= 8 && i <= 12) // layer 4
					home_page.get_layer(4).get_sprite(i)->use_background_img();
				else if (i >= 13 && i <= 17) // layer 5
					home_page.get_layer(5).get_sprite(i)->use_background_img();
				else if (i >= 18 && i <= 19) // layer 6
					home_page.get_layer(6).get_sprite(i)->use_background_img();
				else if (i >= 20 && i <= 22) // layer 7
					home_page.get_layer(7).get_sprite(i)->use_background_img();
				else if (i >= 23 && i <= 25) // layer 8
					home_page.get_layer(8).get_sprite(i)->use_background_img();
				else if (i == 26 || i == 27) // layer 9
					home_page.get_layer(9).get_sprite(i)->use_background_img();
			}
		}
		else
		{
			// reset to normal texture
			for (int i{ 7 }; i < 29; ++i)
			{
				if (i == 7) // layer 3
					home_page.get_layer(3).get_sprite(i)->use_background_img();
				else if (i >= 8 && i <= 12) // layer 4
					home_page.get_layer(4).get_sprite(i)->use_background_img();
				else if (i >= 13 && i <= 17) // layer 5
					home_page.get_layer(5).get_sprite(i)->use_background_img();
				else if (i >= 18 && i <= 19) // layer 6
					home_page.get_layer(6).get_sprite(i)->use_background_img();
				else if (i >= 20 && i <= 22) // layer 7
					home_page.get_layer(7).get_sprite(i)->use_background_img();
				else if (i >= 23 && i <= 25) // layer 8
					home_page.get_layer(8).get_sprite(i)->use_background_img();
				else if (i == 26 || i == 27) // layer 9
					home_page.get_layer(9).get_sprite(i)->use_background_img();
			}
		}
		if (sprite_id == 29 || sprite_id == 30) // hovered a color picker arrow
		{
			// bloom
			home_page.get_layer(11).get_sprite(sprite_id)->set_bloom_strength(1.5f);
		}
		else
		{
			// reset bloom
			home_page.get_layer(11).get_sprite(29)->set_bloom_strength(1.0f);
			home_page.get_layer(11).get_sprite(30)->set_bloom_strength(1.0f);
		}
		if (sprite_id == 31) // hovered close game button
		{
			// bloom
			home_page.get_layer(11).get_sprite(sprite_id)->set_bloom_strength(100'000.0f);
		}
		else
		{
			// reset bloom
			home_page.get_layer(11).get_sprite(31)->set_bloom_strength(1.0f);
		}
		if (sprite_id == 33) // hovered connexion button
		{
			home_page.get_layer(12).get_sprite(sprite_id)->use_background_img_selected();
		}
		else
		{
			home_page.get_layer(12).get_sprite(33)->use_background_img();
		}
		if (sprite_id >= 2 && sprite_id <= 6 && inputs.test(2) && inputs.test(9)) // clicked on a face feature
		{
			// highlight
			home_page.get_layer(1).get_sprite(1)->set_pos(glm::vec2(400, 728 - (140 + 48 * (sprite_id - 2))));
			home_page.get_layer(1).set_visibility(true);

			// select
			hovered->select();
			for (int i{ 2 }; i <= 6; ++i)
			{
				if (sprite_id != i)
				{
					home_page.get_layer(2).get_sprite(i)->unselect();
				}
			}

			// stop focus pseudo input
			home_page.get_layer(12).get_sprite(32)->use_background_img();
			m_writer.m_cursor.m_focus = 2; // 0 = pseudo, 1 = chat, 2 = not writting

			// show options
			if (sprite_id == 2)
			{
				if(home_page.get_layer(3).m_visible)
					home_page.get_layer(3).set_visibility(false);
				else
					home_page.get_layer(3).set_visibility(true);
				for(int i{4}; i < 10; ++i)
					home_page.get_layer(i).set_visibility(false);
			}
			else if (sprite_id == 3)
			{
				if (home_page.get_layer(9).get_sprite(26)->is_selected())
				{
					if(home_page.get_layer(4).m_visible)
						home_page.get_layer(4).set_visibility(false);
					else
						home_page.get_layer(4).set_visibility(true);
					for (int i{ 3 }; i < 10; ++i)
						if (i != 4)
							home_page.get_layer(i).set_visibility(false);
				}
				else if (home_page.get_layer(9).get_sprite(27)->is_selected())
				{
					if(home_page.get_layer(5).m_visible)
						home_page.get_layer(5).set_visibility(true);
					else
						home_page.get_layer(5).set_visibility(true);
					for (int i{ 3 }; i < 10; ++i)
						if (i != 5)
							home_page.get_layer(i).set_visibility(false);
				}
			}
			else if (sprite_id == 4)
			{
				if (home_page.get_layer(9).get_sprite(26)->is_selected())
				{
					if (home_page.get_layer(6).m_visible)
						home_page.get_layer(6).set_visibility(false);
					else
						home_page.get_layer(6).set_visibility(true);
					for (int i{ 3 }; i < 10; ++i)
						if (i != 6)
							home_page.get_layer(i).set_visibility(false);
				}
				else if (home_page.get_layer(9).get_sprite(27)->is_selected())
				{
					if (home_page.get_layer(7).m_visible)
						home_page.get_layer(7).set_visibility(true);
					else
						home_page.get_layer(7).set_visibility(true);
					for (int i{ 3 }; i < 10; ++i)
						if (i != 7)
							home_page.get_layer(i).set_visibility(false);
				}
			}
			else if (sprite_id == 5)
			{
				if(home_page.get_layer(8).m_visible)
					home_page.get_layer(8).set_visibility(false);
				else
					home_page.get_layer(8).set_visibility(true);
				for (int i{ 3 }; i < 10; ++i)
					if (i != 8)
						home_page.get_layer(i).set_visibility(false);
			}
			else if (sprite_id == 6)
			{
				if(home_page.get_layer(9).m_visible)
					home_page.get_layer(9).set_visibility(false);
				else
					home_page.get_layer(9).set_visibility(true);
				for (int i{ 3 }; i < 9; ++i)
						home_page.get_layer(i).set_visibility(false);
			}
		}
		else if (sprite_id >= 7 && sprite_id <= 27 && inputs.test(2) && inputs.test(9)) // clicked on a face feature option
		{
			// select option
			hovered->select();
			if (sprite_id >= 8 && sprite_id <= 12)
			{
				for (int i{ 8 }; i <= 12; ++i)
				{
					if (i == sprite_id)
						continue;
					home_page.get_layer(4).get_sprite(i)->unselect();
				}
			}
			else if (sprite_id >= 13 && sprite_id <= 17)
			{
				for (int i{ 13 }; i <= 17; ++i)
				{
					if (i == sprite_id)
						continue;
					home_page.get_layer(5).get_sprite(i)->unselect();
				}
			}
			else if (sprite_id >= 18 && sprite_id <= 19)
			{
				for (int i{ 18 }; i <= 19; ++i)
				{
					if (i == sprite_id)
						continue;
					home_page.get_layer(6).get_sprite(i)->unselect();
				}
			}
			else if (sprite_id >= 20 && sprite_id <= 22)
			{
				for (int i{ 20 }; i <= 22; ++i)
				{
					if (i == sprite_id)
						continue;
					home_page.get_layer(7).get_sprite(i)->unselect();
				}
			}
			else if (sprite_id >= 23 && sprite_id <= 25)
			{
				for (int i{ 23 }; i <= 25; ++i)
				{
					if (i == sprite_id)
						continue;
					home_page.get_layer(8).get_sprite(i)->unselect();
				}
			}
			else if (sprite_id >= 26 && sprite_id <= 27)
			{
				for (int i{ 26 }; i <= 27; ++i)
				{
					if (i == sprite_id)
						continue;
					home_page.get_layer(9).get_sprite(i)->unselect();
				}
			}

			// hide layer
			home_page.get_layer(hovered->get_layer_id()).set_visibility(false);

			// set gender
			if (sprite_id == 26)
			{
				m_avatar.m_gender = Avatar::GENDER::MALE;
				// unselect all female features and select corresponding male ones
				swap_gender_features(Avatar::GENDER::FEMALE, Avatar::GENDER::MALE);
			}
			else if (sprite_id == 27)
			{
				m_avatar.m_gender = Avatar::GENDER::FEMALE;
				// unselect all male features and select corresponding female ones
				swap_gender_features(Avatar::GENDER::MALE, Avatar::GENDER::FEMALE);
			}

			// set mouth
			else if (sprite_id == 25)
				m_avatar.m_mouth = Avatar::MOUTH::GRANDE;
			else if (sprite_id == 24)
				m_avatar.m_mouth = Avatar::MOUTH::MOYENNE;
			else if (sprite_id == 23)
				m_avatar.m_mouth = Avatar::MOUTH::PETITE;
			else
			{
				if (m_avatar.m_gender == Avatar::GENDER::MALE)
				{
					// set male eyes
					if (sprite_id == 18)
						m_avatar.m_eyes = Avatar::EYES::MANGA;
					else if (sprite_id == 19)
						m_avatar.m_eyes = Avatar::EYES::AMANDE;

					// set male hair
					else if (sprite_id == 8)
						m_avatar.m_hair = Avatar::HAIR::HERISSON;
					else if (sprite_id == 9)
						m_avatar.m_hair = Avatar::HAIR::DECOIFFE;
					else if (sprite_id == 10)
						m_avatar.m_hair = Avatar::HAIR::MECHE_AVANT;
					else if (sprite_id == 11)
						m_avatar.m_hair = Avatar::HAIR::MIXTE;
					else if (sprite_id == 12)
						m_avatar.m_hair = Avatar::HAIR::ARRIERE;
				}
				else
				{
					// set female eyes
					if (sprite_id == 20)
						m_avatar.m_eyes = Avatar::EYES::MANGA;
					else if (sprite_id == 21)
						m_avatar.m_eyes = Avatar::EYES::EGYPTE;
					else if (sprite_id == 22)
						m_avatar.m_eyes = Avatar::EYES::MASCARA;

					// set female hair
					else if (sprite_id == 13)
						m_avatar.m_hair = Avatar::HAIR::MIXTE;
					else if (sprite_id == 14)
						m_avatar.m_hair = Avatar::HAIR::MI_LONG;
					else if (sprite_id == 15)
						m_avatar.m_hair = Avatar::HAIR::FRANGE;
					else if (sprite_id == 16)
						m_avatar.m_hair = Avatar::HAIR::AU_BOL;
					else if (sprite_id == 17)
						m_avatar.m_hair = Avatar::HAIR::PONYTAIL;
				}
			}
		}
		else if ((sprite_id == 29 || sprite_id == 30) && inputs.test(2) && inputs.test(9)) // color picker
		{
			if (home_page.get_layer(2).get_sprite(2)->is_selected()) // change skin color
			{
				if (sprite_id == 29)
				{
					m_avatar.m_skin_color_id--;
					if (m_avatar.m_skin_color_id == -1)
					{
						m_avatar.m_skin_color_id = m_avatar.m_color.size() - 1;
					}
				}
				else
				{
					m_avatar.m_skin_color_id++;
					if (m_avatar.m_skin_color_id >= m_avatar.m_color.size())
					{
						m_avatar.m_skin_color_id = 0;
					}
				}
			}
			else if (home_page.get_layer(2).get_sprite(3)->is_selected()) // change hair color
			{
				if (sprite_id == 29)
				{
					m_avatar.m_hair_color_id--;
					if (m_avatar.m_hair_color_id == -1)
					{
						m_avatar.m_hair_color_id = m_avatar.m_color.size() - 2;
					}
				}
				else
				{
					m_avatar.m_hair_color_id++;
					if (m_avatar.m_hair_color_id >= m_avatar.m_color.size() - 1)
					{
						m_avatar.m_hair_color_id = 0;
					}
				}
			}
			else if (home_page.get_layer(2).get_sprite(4)->is_selected()) // change eyes color
			{
				if (sprite_id == 29)
				{
					m_avatar.m_eyes_color_id--;
					if (m_avatar.m_eyes_color_id == -1)
					{
						m_avatar.m_eyes_color_id = m_avatar.m_color.size() - 2;
					}
				}
				else
				{
					m_avatar.m_eyes_color_id++;
					if (m_avatar.m_eyes_color_id >= m_avatar.m_color.size() - 1)
					{
						m_avatar.m_eyes_color_id = 0;
					}
				}
			}

			// stop focus pseudo input
			home_page.get_layer(12).get_sprite(32)->use_background_img();
			m_writer.m_cursor.m_focus = 2; // 0 = pseudo, 1 = chat, 2 = not writting

			// hide face feature options
			home_page.get_layer(3).set_visibility(false);
			home_page.get_layer(4).set_visibility(false);
			home_page.get_layer(5).set_visibility(false);
			home_page.get_layer(6).set_visibility(false);
			home_page.get_layer(7).set_visibility(false);
			home_page.get_layer(8).set_visibility(false);
			home_page.get_layer(9).set_visibility(false);
		}
		else if (sprite_id == 31 && inputs.test(2) && inputs.test(9)) // clicked on quit game
		{
			SDL_Event event;
			event.type = SDL_QUIT;
			SDL_PushEvent(&event);
		}
		else if (sprite_id == 32 && inputs.test(2) && inputs.test(9)) // clicked on pseudo
		{
			home_page.get_layer(12).get_sprite(sprite_id)->use_background_img_selected();
			m_writer.m_cursor.m_focus = 0; // 0 = pseudo, 1 = chat, 2 = not writting
		}
		else if (inputs.test(2) && inputs.test(9))
		{
			// stop focus pseudo input
			home_page.get_layer(12).get_sprite(32)->use_background_img();
			m_writer.m_cursor.m_focus = 2; // 0 = pseudo, 1 = chat, 2 = not writting
			
			// hide face feature options
			home_page.get_layer(3).set_visibility(false);
			home_page.get_layer(4).set_visibility(false);
			home_page.get_layer(5).set_visibility(false);
			home_page.get_layer(6).set_visibility(false);
			home_page.get_layer(7).set_visibility(false);
			home_page.get_layer(8).set_visibility(false);
			home_page.get_layer(9).set_visibility(false);
		}
		if (m_writer.m_cursor.m_focus == 0)
		{
			int boundX = home_page.get_layer(12).get_sprite(32)->get_position().x + home_page.get_layer(12).get_sprite(32)->get_size().x;
			glm::vec3 cursor_shape = textRenderer->get_cursor_shape(m_writer.m_textInput[0], 525 - 72, 272, 1, m_writer.m_cursor.m_pos);
			m_writer.write(text_input, inputs, delta, boundX, cursor_shape);
		}
	}
	else // game
	{

	}
}

void Game::swap_gender_features(Avatar::GENDER from, Avatar::GENDER to)
{
	// swap hair
	if (from == Avatar::GENDER::MALE)
	{
		if (m_avatar.m_hair == Avatar::HAIR::ARRIERE)
			m_avatar.m_hair = Avatar::HAIR::PONYTAIL;
		else if (m_avatar.m_hair == Avatar::HAIR::DECOIFFE)
			m_avatar.m_hair = Avatar::HAIR::AU_BOL;
		else if (m_avatar.m_hair == Avatar::HAIR::HERISSON)
			m_avatar.m_hair = Avatar::HAIR::MI_LONG;
		else if (m_avatar.m_hair == Avatar::HAIR::MECHE_AVANT)
			m_avatar.m_hair = Avatar::HAIR::FRANGE;
	}
	else
	{
		if (m_avatar.m_hair == Avatar::HAIR::FRANGE)
			m_avatar.m_hair = Avatar::HAIR::MECHE_AVANT;
		else if (m_avatar.m_hair == Avatar::HAIR::MI_LONG)
			m_avatar.m_hair = Avatar::HAIR::HERISSON;
		else if (m_avatar.m_hair == Avatar::HAIR::PONYTAIL)
			m_avatar.m_hair = Avatar::HAIR::ARRIERE;
		else if (m_avatar.m_hair == Avatar::HAIR::AU_BOL)
			m_avatar.m_hair = Avatar::HAIR::DECOIFFE;
	}
	// swap eyes
	if (from == Avatar::GENDER::MALE)
	{
		if (m_avatar.m_eyes == Avatar::EYES::AMANDE)
			m_avatar.m_eyes = Avatar::EYES::EGYPTE;
	}
	else
	{
		if (m_avatar.m_eyes == Avatar::EYES::EGYPTE)
			m_avatar.m_eyes = Avatar::EYES::AMANDE;
		else if (m_avatar.m_eyes == Avatar::EYES::MASCARA)
			m_avatar.m_eyes = Avatar::EYES::AMANDE;
	}
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