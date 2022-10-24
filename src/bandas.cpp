#include "bandas.hpp"

Bandas::Bandas(Graphics& graphics) :
	m_graphics(graphics),
	m_text(c_screen_width, c_screen_height),
	m_mouse("assets/mouse/viseur.tga"),
	m_writer(c_screen_width, c_screen_height)
{
	// user interface
	createUI();

    // load some fonts and set an active font
    m_text.load_police("assets/fonts/ebrima.ttf", 20);
    m_text.load_police("assets/fonts/ebrima.ttf", 15);
    m_text.use_police(0);
}

Bandas::~Bandas()
{
	m_me.m_avatar.cleanup();
	m_me.m_chrono.cleanup();
	m_enemy.m_avatar.cleanup();
	m_enemy.m_chrono.cleanup();
}

void Bandas::createUI()
{
	create_home_page();
	create_game_page();
	m_ui.set_active_page(0);
}

void Bandas::create_home_page()
{
	m_ui.add_page();

	Page& home_page = m_ui.get_page(0);
	home_page.add_layer(0);
	home_page.add_layer(1); // emphase (élément graphique)
	home_page.add_layer(2); // menu déroulant "partie du visage"
	home_page.add_layer(3); // options visage
	home_page.add_layer(4); // options cheveux (homme)
	home_page.add_layer(5); // options cheveux (femme)
	home_page.add_layer(6); // options yeux (homme)
	home_page.add_layer(7); // options yeux (femme)
	home_page.add_layer(8); // options bouche
	home_page.add_layer(9); // options sac
	home_page.add_layer(10); // options sex
	home_page.add_layer(11); // avatar
	home_page.add_layer(12); // choix de couleur et fermer la fenêtre
	home_page.add_layer(13); // user inputs (pseudo, connexion)
	home_page.add_layer(14); // connection status
	home_page.add_layer(15); // chercher adversaire
	home_page.add_layer(16); // stop chercher adversaire
	home_page.add_layer(17); // pop up messages

	Layer& h_layer0 = home_page.get_layer(0);
	h_layer0.add_sprite(0, glm::vec2(0.0f), glm::vec2(c_screen_width, c_screen_height), c_screen_width, c_screen_height);
	h_layer0.get_sprite(0)->set_background_img("assets/home_page/home.tga");
	h_layer0.get_sprite(0)->use_background_img();

	Layer& h_layer1 = home_page.get_layer(1);
	h_layer1.add_sprite(1, glm::vec2(400, 728 - (140 + 48)), glm::vec2(400, 48), c_screen_width, c_screen_height);
	h_layer1.get_sprite(1)->set_background_img("assets/frutibouilleur/face_part_highlight.tga");
	h_layer1.get_sprite(1)->use_background_img();

	Layer& h_layer2 = home_page.get_layer(2);
	h_layer2.add_sprite(2, glm::vec2(600, 728 - (140 + 48 - 12)), glm::vec2(150, 24), c_screen_width, c_screen_height);
	h_layer2.get_sprite(2)->set_background_img("assets/frutibouilleur/face.tga");
	h_layer2.get_sprite(2)->set_background_img_selected("assets/frutibouilleur/face_hover.tga");
	h_layer2.get_sprite(2)->use_background_img();
	h_layer2.get_sprite(2)->select();
	// >>>>> highlight
	h_layer1.get_sprite(1)->set_pos(glm::vec2(400, 728 - 188));
	h_layer1.set_visibility(true);
	// <<<<< highlight
	h_layer2.add_sprite(3, glm::vec2(600, 728 - (140 + 48 * 2 - 12)), glm::vec2(150, 24), c_screen_width, c_screen_height);
	h_layer2.get_sprite(3)->set_background_img("assets/frutibouilleur/hair.tga");
	h_layer2.get_sprite(3)->set_background_img_selected("assets/frutibouilleur/hair_hover.tga");
	h_layer2.get_sprite(3)->use_background_img();
	h_layer2.add_sprite(4, glm::vec2(600, 728 - (140 + 48 * 3 - 12)), glm::vec2(150, 24), c_screen_width, c_screen_height);
	h_layer2.get_sprite(4)->set_background_img("assets/frutibouilleur/eyes.tga");
	h_layer2.get_sprite(4)->set_background_img_selected("assets/frutibouilleur/eyes_hover.tga");
	h_layer2.get_sprite(4)->use_background_img();
	h_layer2.add_sprite(5, glm::vec2(600, 728 - (140 + 48 * 4 - 12)), glm::vec2(150, 24), c_screen_width, c_screen_height);
	h_layer2.get_sprite(5)->set_background_img("assets/frutibouilleur/mouth.tga");
	h_layer2.get_sprite(5)->set_background_img_selected("assets/frutibouilleur/mouth_hover.tga");
	h_layer2.get_sprite(5)->use_background_img();
	h_layer2.add_sprite(6, glm::vec2(600, 728 - (140 + 48 * 5 - 12)), glm::vec2(150, 24), c_screen_width, c_screen_height);
	h_layer2.get_sprite(6)->set_background_img("assets/frutibouilleur/burlap_sack.tga");
	h_layer2.get_sprite(6)->set_background_img_selected("assets/frutibouilleur/burlap_sack_hover.tga");
	h_layer2.get_sprite(6)->use_background_img();

	Layer& h_layer3 = home_page.get_layer(3);
	h_layer3.set_visibility(false);
	h_layer3.add_sprite(7, glm::vec2(610, 728 - (140 + 48 + 12)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer3.get_sprite(7)->set_background_img("assets/frutibouilleur/visage1.tga");
	h_layer3.get_sprite(7)->set_background_img_selected("assets/frutibouilleur/visage1_hover.tga");
	h_layer3.get_sprite(7)->use_background_img();
	h_layer3.add_sprite(8, glm::vec2(610, 728 - (140 + 48 + 12 + 24)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer3.get_sprite(8)->set_background_img("assets/frutibouilleur/visage2.tga");
	h_layer3.get_sprite(8)->set_background_img_selected("assets/frutibouilleur/visage2_hover.tga");
	h_layer3.get_sprite(8)->use_background_img();

	Layer& h_layer4 = home_page.get_layer(4);
	h_layer4.set_visibility(false);
	h_layer4.add_sprite(9, glm::vec2(610, 728 - (140 + 48 * 2 + 12)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(9)->set_background_img("assets/frutibouilleur/bulb1.tga");
	h_layer4.get_sprite(9)->set_background_img_selected("assets/frutibouilleur/bulb1_hover.tga");
	h_layer4.get_sprite(9)->use_background_img();
	h_layer4.add_sprite(10, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 24)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(10)->set_background_img("assets/frutibouilleur/bulb2.tga");
	h_layer4.get_sprite(10)->set_background_img_selected("assets/frutibouilleur/bulb2_hover.tga");
	h_layer4.get_sprite(10)->use_background_img();
	h_layer4.add_sprite(11, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 48)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(11)->set_background_img("assets/frutibouilleur/decoiffe.tga");
	h_layer4.get_sprite(11)->set_background_img_selected("assets/frutibouilleur/decoiffe_hover.tga");
	h_layer4.get_sprite(11)->use_background_img();
	h_layer4.add_sprite(12, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 72)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(12)->set_background_img("assets/frutibouilleur/elvis.tga");
	h_layer4.get_sprite(12)->set_background_img_selected("assets/frutibouilleur/elvis_hover.tga");
	h_layer4.get_sprite(12)->use_background_img();
	h_layer4.add_sprite(13, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 96)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(13)->set_background_img("assets/frutibouilleur/epi1.tga");
	h_layer4.get_sprite(13)->set_background_img_selected("assets/frutibouilleur/epi1_hover.tga");
	h_layer4.get_sprite(13)->use_background_img();
	h_layer4.add_sprite(14, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 120)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(14)->set_background_img("assets/frutibouilleur/epi2.tga");
	h_layer4.get_sprite(14)->set_background_img_selected("assets/frutibouilleur/epi2_hover.tga");
	h_layer4.get_sprite(14)->use_background_img();
	h_layer4.add_sprite(15, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 144)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(15)->set_background_img("assets/frutibouilleur/epi3.tga");
	h_layer4.get_sprite(15)->set_background_img_selected("assets/frutibouilleur/epi3_hover.tga");
	h_layer4.get_sprite(15)->use_background_img();
	h_layer4.add_sprite(16, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 168)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(16)->set_background_img("assets/frutibouilleur/herisson.tga");
	h_layer4.get_sprite(16)->set_background_img_selected("assets/frutibouilleur/herisson_hover.tga");
	h_layer4.get_sprite(16)->use_background_img();
	h_layer4.add_sprite(17, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 192)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(17)->set_background_img("assets/frutibouilleur/iroquois.tga");
	h_layer4.get_sprite(17)->set_background_img_selected("assets/frutibouilleur/iroquois_hover.tga");
	h_layer4.get_sprite(17)->use_background_img();
	h_layer4.add_sprite(18, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 216)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(18)->set_background_img("assets/frutibouilleur/kaspa.tga");
	h_layer4.get_sprite(18)->set_background_img_selected("assets/frutibouilleur/kaspa_hover.tga");
	h_layer4.get_sprite(18)->use_background_img();
	h_layer4.add_sprite(19, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 240)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(19)->set_background_img("assets/frutibouilleur/meche.tga");
	h_layer4.get_sprite(19)->set_background_img_selected("assets/frutibouilleur/meche_hover.tga");
	h_layer4.get_sprite(19)->use_background_img();
	h_layer4.add_sprite(20, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 264)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(20)->set_background_img("assets/frutibouilleur/mixte.tga");
	h_layer4.get_sprite(20)->set_background_img_selected("assets/frutibouilleur/mixte_hover.tga");
	h_layer4.get_sprite(20)->use_background_img();
	h_layer4.add_sprite(21, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 288)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer4.get_sprite(21)->set_background_img("assets/frutibouilleur/steady.tga");
	h_layer4.get_sprite(21)->set_background_img_selected("assets/frutibouilleur/steady_hover.tga");
	h_layer4.get_sprite(21)->use_background_img();

	Layer& h_layer5 = home_page.get_layer(5);
	h_layer5.set_visibility(false);
	h_layer5.add_sprite(22, glm::vec2(610, 728 - (140 + 48 * 2 + 12)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(22)->set_background_img("assets/frutibouilleur/au_bol.tga");
	h_layer5.get_sprite(22)->set_background_img_selected("assets/frutibouilleur/au_bol_hover.tga");
	h_layer5.get_sprite(22)->use_background_img();
	h_layer5.add_sprite(23, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 24)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(23)->set_background_img("assets/frutibouilleur/couettes.tga");
	h_layer5.get_sprite(23)->set_background_img_selected("assets/frutibouilleur/couettes_hover.tga");
	h_layer5.get_sprite(23)->use_background_img();
	h_layer5.add_sprite(24, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 48)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(24)->set_background_img("assets/frutibouilleur/decoiffe.tga");
	h_layer5.get_sprite(24)->set_background_img_selected("assets/frutibouilleur/decoiffe_hover.tga");
	h_layer5.get_sprite(24)->use_background_img();
	h_layer5.add_sprite(25, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 72)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(25)->set_background_img("assets/frutibouilleur/frange.tga");
	h_layer5.get_sprite(25)->set_background_img_selected("assets/frutibouilleur/frange_hover.tga");
	h_layer5.get_sprite(25)->use_background_img();
	h_layer5.add_sprite(26, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 96)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(26)->set_background_img("assets/frutibouilleur/medusa.tga");
	h_layer5.get_sprite(26)->set_background_img_selected("assets/frutibouilleur/medusa_hover.tga");
	h_layer5.get_sprite(26)->use_background_img();
	h_layer5.add_sprite(27, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 120)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(27)->set_background_img("assets/frutibouilleur/mi_long.tga");
	h_layer5.get_sprite(27)->set_background_img_selected("assets/frutibouilleur/mi_long_hover.tga");
	h_layer5.get_sprite(27)->use_background_img();
	h_layer5.add_sprite(28, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 144)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(28)->set_background_img("assets/frutibouilleur/mickey.tga");
	h_layer5.get_sprite(28)->set_background_img_selected("assets/frutibouilleur/mickey_hover.tga");
	h_layer5.get_sprite(28)->use_background_img();
	h_layer5.add_sprite(29, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 168)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(29)->set_background_img("assets/frutibouilleur/mixte.tga");
	h_layer5.get_sprite(29)->set_background_img_selected("assets/frutibouilleur/mixte_hover.tga");
	h_layer5.get_sprite(29)->use_background_img();
	h_layer5.add_sprite(30, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 192)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(30)->set_background_img("assets/frutibouilleur/nattes.tga");
	h_layer5.get_sprite(30)->set_background_img_selected("assets/frutibouilleur/nattes_hover.tga");
	h_layer5.get_sprite(30)->use_background_img();
	h_layer5.add_sprite(31, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 216)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(31)->set_background_img("assets/frutibouilleur/ponytail.tga");
	h_layer5.get_sprite(31)->set_background_img_selected("assets/frutibouilleur/ponytail_hover.tga");
	h_layer5.get_sprite(31)->use_background_img();
	h_layer5.add_sprite(32, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 240)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(32)->set_background_img("assets/frutibouilleur/raie.tga");
	h_layer5.get_sprite(32)->set_background_img_selected("assets/frutibouilleur/raie_hover.tga");
	h_layer5.get_sprite(32)->use_background_img();
	h_layer5.add_sprite(33, glm::vec2(610, 728 - (140 + 48 * 2 + 12 + 264)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer5.get_sprite(33)->set_background_img("assets/frutibouilleur/seventies.tga");
	h_layer5.get_sprite(33)->set_background_img_selected("assets/frutibouilleur/seventies_hover.tga");
	h_layer5.get_sprite(33)->use_background_img();

	Layer& h_layer6 = home_page.get_layer(6);
	h_layer6.set_visibility(false);
	h_layer6.add_sprite(34, glm::vec2(610, 728 - (140 + 48 * 3 + 12)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer6.get_sprite(34)->set_background_img("assets/frutibouilleur/manga.tga");
	h_layer6.get_sprite(34)->set_background_img_selected("assets/frutibouilleur/manga_hover.tga");
	h_layer6.get_sprite(34)->use_background_img();
	h_layer6.add_sprite(35, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 24)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer6.get_sprite(35)->set_background_img("assets/frutibouilleur/gros.tga");
	h_layer6.get_sprite(35)->set_background_img_selected("assets/frutibouilleur/gros_hover.tga");
	h_layer6.get_sprite(35)->use_background_img();
	h_layer6.add_sprite(36, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 48)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer6.get_sprite(36)->set_background_img("assets/frutibouilleur/colere.tga");
	h_layer6.get_sprite(36)->set_background_img_selected("assets/frutibouilleur/colere_hover.tga");
	h_layer6.get_sprite(36)->use_background_img();
	h_layer6.add_sprite(37, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 72)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer6.get_sprite(37)->set_background_img("assets/frutibouilleur/fatigue.tga");
	h_layer6.get_sprite(37)->set_background_img_selected("assets/frutibouilleur/fatigue_hover.tga");
	h_layer6.get_sprite(37)->use_background_img();
	h_layer6.add_sprite(38, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 96)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer6.get_sprite(38)->set_background_img("assets/frutibouilleur/psycho.tga");
	h_layer6.get_sprite(38)->set_background_img_selected("assets/frutibouilleur/psycho_hover.tga");
	h_layer6.get_sprite(38)->use_background_img();
	h_layer6.add_sprite(39, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 120)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer6.get_sprite(39)->set_background_img("assets/frutibouilleur/amande.tga");
	h_layer6.get_sprite(39)->set_background_img_selected("assets/frutibouilleur/amande_hover.tga");
	h_layer6.get_sprite(39)->use_background_img();

	Layer& h_layer7 = home_page.get_layer(7);
	h_layer7.set_visibility(false);
	h_layer7.add_sprite(40, glm::vec2(610, 728 - (140 + 48 * 3 + 12)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer7.get_sprite(40)->set_background_img("assets/frutibouilleur/egypte.tga");
	h_layer7.get_sprite(40)->set_background_img_selected("assets/frutibouilleur/egypte_hover.tga");
	h_layer7.get_sprite(40)->use_background_img();
	h_layer7.add_sprite(41, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 24)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer7.get_sprite(41)->set_background_img("assets/frutibouilleur/manga.tga");
	h_layer7.get_sprite(41)->set_background_img_selected("assets/frutibouilleur/manga_hover.tga");
	h_layer7.get_sprite(41)->use_background_img();
	h_layer7.add_sprite(42, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 48)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer7.get_sprite(42)->set_background_img("assets/frutibouilleur/inquiete.tga");
	h_layer7.get_sprite(42)->set_background_img_selected("assets/frutibouilleur/inquiete_hover.tga");
	h_layer7.get_sprite(42)->use_background_img();
	h_layer7.add_sprite(43, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 72)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer7.get_sprite(43)->set_background_img("assets/frutibouilleur/colere.tga");
	h_layer7.get_sprite(43)->set_background_img_selected("assets/frutibouilleur/colere_hover.tga");
	h_layer7.get_sprite(43)->use_background_img();
	h_layer7.add_sprite(44, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 96)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer7.get_sprite(44)->set_background_img("assets/frutibouilleur/fatigue.tga");
	h_layer7.get_sprite(44)->set_background_img_selected("assets/frutibouilleur/fatigue_hover.tga");
	h_layer7.get_sprite(44)->use_background_img();
	h_layer7.add_sprite(45, glm::vec2(610, 728 - (140 + 48 * 3 + 12 + 120)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer7.get_sprite(45)->set_background_img("assets/frutibouilleur/psycho.tga");
	h_layer7.get_sprite(45)->set_background_img_selected("assets/frutibouilleur/psycho_hover.tga");
	h_layer7.get_sprite(45)->use_background_img();

	Layer& h_layer8 = home_page.get_layer(8);
	h_layer8.set_visibility(false);
	h_layer8.add_sprite(46, glm::vec2(610, 728 - (140 + 48 * 4 + 12)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer8.get_sprite(46)->set_background_img("assets/frutibouilleur/bouche_petite.tga");
	h_layer8.get_sprite(46)->set_background_img_selected("assets/frutibouilleur/bouche_petite_hover.tga");
	h_layer8.get_sprite(46)->use_background_img();
	h_layer8.add_sprite(47, glm::vec2(610, 728 - (140 + 48 * 4 + 12 + 24)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer8.get_sprite(47)->set_background_img("assets/frutibouilleur/bouche_moyenne.tga");
	h_layer8.get_sprite(47)->set_background_img_selected("assets/frutibouilleur/bouche_moyenne_hover.tga");
	h_layer8.get_sprite(47)->use_background_img();
	h_layer8.add_sprite(48, glm::vec2(610, 728 - (140 + 48 * 4 + 12 + 48)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer8.get_sprite(48)->set_background_img("assets/frutibouilleur/bouche_grande.tga");
	h_layer8.get_sprite(48)->set_background_img_selected("assets/frutibouilleur/bouche_grande_hover.tga");
	h_layer8.get_sprite(48)->use_background_img();
	h_layer8.add_sprite(49, glm::vec2(610, 728 - (140 + 48 * 4 + 12 + 72)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer8.get_sprite(49)->set_background_img("assets/frutibouilleur/sourire.tga");
	h_layer8.get_sprite(49)->set_background_img_selected("assets/frutibouilleur/sourire_hover.tga");
	h_layer8.get_sprite(49)->use_background_img();
	h_layer8.add_sprite(50, glm::vec2(610, 728 - (140 + 48 * 4 + 12 + 96)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer8.get_sprite(50)->set_background_img("assets/frutibouilleur/machoire.tga");
	h_layer8.get_sprite(50)->set_background_img_selected("assets/frutibouilleur/machoire_hover.tga");
	h_layer8.get_sprite(50)->use_background_img();
	h_layer8.add_sprite(51, glm::vec2(610, 728 - (140 + 48 * 4 + 12 + 120)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer8.get_sprite(51)->set_background_img("assets/frutibouilleur/pacifier.tga");
	h_layer8.get_sprite(51)->set_background_img_selected("assets/frutibouilleur/pacifier_hover.tga");
	h_layer8.get_sprite(51)->use_background_img();

	Layer& h_layer9 = home_page.get_layer(9);
	h_layer9.set_visibility(false);
	h_layer9.add_sprite(52, glm::vec2(610, 728 - (140 + 48 * 5 + 12)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer9.get_sprite(52)->set_background_img("assets/frutibouilleur/sac1.tga");
	h_layer9.get_sprite(52)->set_background_img_selected("assets/frutibouilleur/sac1_hover.tga");
	h_layer9.get_sprite(52)->use_background_img();
	h_layer9.get_sprite(52)->select();
	h_layer9.add_sprite(53, glm::vec2(610, 728 - (140 + 48 * 5 + 12 + 24)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer9.get_sprite(53)->set_background_img("assets/frutibouilleur/sac2.tga");
	h_layer9.get_sprite(53)->set_background_img_selected("assets/frutibouilleur/sac2_hover.tga");
	h_layer9.get_sprite(53)->use_background_img();
	h_layer9.add_sprite(54, glm::vec2(610, 728 - (140 + 48 * 5 + 12 + 48)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer9.get_sprite(54)->set_background_img("assets/frutibouilleur/sac3.tga");
	h_layer9.get_sprite(54)->set_background_img_selected("assets/frutibouilleur/sac3_hover.tga");
	h_layer9.get_sprite(54)->use_background_img();
	h_layer9.add_sprite(55, glm::vec2(610, 728 - (140 + 48 * 5 + 12 + 72)), glm::vec2(130, 24), c_screen_width, c_screen_height);
	h_layer9.get_sprite(55)->set_background_img("assets/frutibouilleur/none.tga");
	h_layer9.get_sprite(55)->set_background_img_selected("assets/frutibouilleur/none_hover.tga");
	h_layer9.get_sprite(55)->use_background_img();

	Layer& h_layer10 = home_page.get_layer(10);
	h_layer10.add_sprite(56, glm::vec2(97, 728 - 275), glm::vec2(32, 32), c_screen_width, c_screen_height);
	h_layer10.get_sprite(56)->set_background_img("assets/frutibouilleur/female.tga");
	h_layer10.get_sprite(56)->use_background_img();
	h_layer10.add_sprite(57, glm::vec2(142, 728 - 270), glm::vec2(32, 32), c_screen_width, c_screen_height);
	h_layer10.get_sprite(57)->set_background_img("assets/frutibouilleur/male.tga");
	h_layer10.get_sprite(57)->use_background_img();
	h_layer10.add_sprite(58, glm::vec2(105, 728 - 275 - 21), glm::vec2(16, 16), c_screen_width, c_screen_height);
	h_layer10.get_sprite(58)->set_background_img("assets/frutibouilleur/unchecked_checkbox.tga");
	h_layer10.get_sprite(58)->set_background_img_selected("assets/frutibouilleur/checked_checkbox.tga");
	h_layer10.get_sprite(58)->use_background_img();
	h_layer10.add_sprite(59, glm::vec2(147, 728 - 275 - 21), glm::vec2(16, 16), c_screen_width, c_screen_height);
	h_layer10.get_sprite(59)->set_background_img("assets/frutibouilleur/unchecked_checkbox.tga");
	h_layer10.get_sprite(59)->set_background_img_selected("assets/frutibouilleur/checked_checkbox.tga");
	h_layer10.get_sprite(59)->use_background_img_selected();
	h_layer10.get_sprite(59)->select();

	Layer& h_layer11 = home_page.get_layer(11);
	h_layer11.add_sprite(60, glm::vec2(250, 728 - (140 + 48 * 5 + 12 + 24)), glm::vec2(300, 300), c_screen_width, c_screen_height);
	h_layer11.get_sprite(60)->set_background_img_gl(m_graphics.avatarFBO->getAttachments()[0].id);
	h_layer11.get_sprite(60)->use_background_img_gl();

	Layer& h_layer12 = home_page.get_layer(12);
	h_layer12.add_sprite(61, glm::vec2(225, 728 - 285), glm::vec2(50, 50), c_screen_width, c_screen_height);
	h_layer12.get_sprite(61)->set_background_img("assets/frutibouilleur/couleur_left.tga");
	h_layer12.get_sprite(61)->use_background_img();
	h_layer12.add_sprite(62, glm::vec2(525, 728 - 285), glm::vec2(50, 50), c_screen_width, c_screen_height);
	h_layer12.get_sprite(62)->set_background_img("assets/frutibouilleur/couleur_right.tga");
	h_layer12.get_sprite(62)->use_background_img();
	h_layer12.add_sprite(63, glm::vec2(984, 728 - 65), glm::vec2(50, 50), c_screen_width, c_screen_height);
	h_layer12.get_sprite(63)->set_background_img("assets/home_page/off.tga");
	h_layer12.get_sprite(63)->use_background_img();

	Layer& h_layer13 = home_page.get_layer(13);
	h_layer13.add_sprite(64, glm::vec2(525 - 75, 728 - (140 + 48 * 7 - 12)), glm::vec2(150, 30), c_screen_width, c_screen_height);
	h_layer13.get_sprite(64)->set_background_color(glm::vec4(0.906f, 0.957f, 0.808f, 1.0f));
	h_layer13.add_sprite(65, glm::vec2(525 - 75, 728 - (140 + 48 * 8 - 24)), glm::vec2(150, 24), c_screen_width, c_screen_height);
	h_layer13.get_sprite(65)->set_background_img("assets/home_page/connexion.tga");
	h_layer13.get_sprite(65)->set_background_img_selected("assets/home_page/connexion_hover.tga");
	h_layer13.get_sprite(65)->use_background_img();

	Layer& h_layer14 = home_page.get_layer(14);
	h_layer14.add_sprite(66, glm::vec2(91, 728 - 432 - 70), glm::vec2(99, 70), c_screen_width, c_screen_height);
	h_layer14.get_sprite(66)->set_background_img("assets/home_page/internet.tga");
	h_layer14.get_sprite(66)->set_background_img_selected("assets/home_page/internet_on.tga");
	h_layer14.get_sprite(66)->use_background_img();
	h_layer14.get_sprite(66)->set_bloom_strength(100.0f);

	Layer& h_layer15 = home_page.get_layer(15);
	h_layer15.set_visibility(false);
	h_layer15.add_sprite(67, glm::vec2(525 - 40, 240), glm::vec2(80, 24), c_screen_width, c_screen_height);
	h_layer15.get_sprite(67)->set_background_img("assets/home_page/play.tga");
	h_layer15.get_sprite(67)->set_background_img_selected("assets/home_page/play_hover.tga");
	h_layer15.get_sprite(67)->use_background_img();

	Layer& h_layer16 = home_page.get_layer(16);
	h_layer16.set_visibility(false);
	h_layer16.add_sprite(68, glm::vec2(525 - 40, 240), glm::vec2(80, 24), c_screen_width, c_screen_height);
	h_layer16.get_sprite(68)->set_background_img("assets/home_page/stop_search.tga");
	h_layer16.get_sprite(68)->set_background_img_selected("assets/home_page/stop_search_hover.tga");
	h_layer16.get_sprite(68)->use_background_img();

	Layer& h_layer17 = home_page.get_layer(17);
	h_layer17.set_visibility(false);
	h_layer17.add_sprite(69, glm::vec2(191, 728 - 337 - 196), glm::vec2(667, 337), c_screen_width, c_screen_height);
	h_layer17.get_sprite(69)->add_texture("assets/home_page/opponent_gave_up.tga");
	h_layer17.get_sprite(69)->add_texture("assets/home_page/opponent_disconnected.tga");
	h_layer17.get_sprite(69)->add_texture("assets/home_page/connection_server_lost.tga");
	h_layer17.get_sprite(69)->use_background_img_gl();
	h_layer17.add_sprite(70, glm::vec2(436, 728 - 78 - 196 - 212), glm::vec2(178, 78), c_screen_width, c_screen_height);
	h_layer17.get_sprite(70)->set_background_img("assets/common/ok.tga");
	h_layer17.get_sprite(70)->set_background_img_selected("assets/common/ok_hover.tga");
	h_layer17.get_sprite(70)->use_background_img();
}

void Bandas::create_game_page()
{
	m_ui.add_page();

	Page& game_page = m_ui.get_page(1);
	game_page.add_layer(0); // game background + avatar + sound & abandon buttons
	game_page.add_layer(1); // chat input
	game_page.add_layer(2); // game arrows
	game_page.add_layer(3); // cards
	game_page.add_layer(4); // card description

	Layer& g_layer0 = game_page.get_layer(0);
	g_layer0.add_sprite(0, glm::vec2(0.0f), glm::vec2(c_screen_width, c_screen_height), c_screen_width, c_screen_height);
	g_layer0.get_sprite(0)->set_background_img("assets/game_page/game.tga");
	g_layer0.get_sprite(0)->use_background_img();
	g_layer0.add_sprite(1, glm::vec2(21, 728 - 23 - 130), glm::vec2(130, 130), c_screen_width, c_screen_height);
	g_layer0.get_sprite(1)->set_background_img_gl(m_graphics.avatarFBO->getAttachments()[0].id);
	g_layer0.get_sprite(1)->use_background_img_gl();
	g_layer0.add_sprite(2, glm::vec2(896, 728 - 23 - 130), glm::vec2(130, 130), c_screen_width, c_screen_height);
	g_layer0.get_sprite(2)->set_background_img_gl(m_graphics.opponentAvatarFBO->getAttachments()[0].id);
	g_layer0.get_sprite(2)->use_background_img_gl();
	g_layer0.add_sprite(3, glm::vec2(1050 - 120, 0), glm::vec2(120, 30), c_screen_width, c_screen_height);
	g_layer0.get_sprite(3)->set_background_img("assets/game_page/give_up.tga");
	g_layer0.get_sprite(3)->set_background_img_selected("assets/game_page/give_up_hover.tga");
	g_layer0.get_sprite(3)->use_background_img();
	g_layer0.add_sprite(4, glm::vec2(1050 - 120-91-5, 0), glm::vec2(91, 30), c_screen_width, c_screen_height);
	g_layer0.get_sprite(4)->set_background_img("assets/game_page/sound.tga");
	g_layer0.get_sprite(4)->set_background_img_selected("assets/game_page/sound_hover.tga");
	g_layer0.get_sprite(4)->use_background_img();

	Layer& g_layer1 = game_page.get_layer(1);
	g_layer1.add_sprite(5, glm::vec2(240, 728 - 698 - 20), glm::vec2(572, 25), c_screen_width, c_screen_height);
	g_layer1.get_sprite(5)->set_background_img("assets/game_page/chat_input.tga");
	g_layer1.get_sprite(5)->set_background_img_selected("assets/game_page/chat_input_hover.tga");
	g_layer1.get_sprite(5)->use_background_img();

	Layer& g_layer2 = game_page.get_layer(2);
	g_layer2.add_sprite(6, glm::vec2(473, 728 + 17 - 105), glm::vec2(105), c_screen_width, c_screen_height);
	g_layer2.get_sprite(6)->set_background_img("assets/game_page/arrow_up.tga");
	g_layer2.get_sprite(6)->set_background_img_selected("assets/game_page/arrow_up_hover.tga");
	g_layer2.get_sprite(6)->use_background_img();
	g_layer2.add_sprite(7, glm::vec2(473, 728 - 465 - 105), glm::vec2(105), c_screen_width, c_screen_height);
	g_layer2.get_sprite(7)->set_background_img("assets/game_page/arrow_down.tga");
	g_layer2.get_sprite(7)->set_background_img_selected("assets/game_page/arrow_down_hover.tga");
	g_layer2.get_sprite(7)->use_background_img();
	g_layer2.add_sprite(8, glm::vec2(719, 728 - 225 - 105), glm::vec2(105), c_screen_width, c_screen_height);
	g_layer2.get_sprite(8)->set_background_img("assets/game_page/arrow_right.tga");
	g_layer2.get_sprite(8)->set_background_img_selected("assets/game_page/arrow_right_hover.tga");
	g_layer2.get_sprite(8)->use_background_img();
	g_layer2.add_sprite(9, glm::vec2(229, 728 - 225 - 105), glm::vec2(105), c_screen_width, c_screen_height);
	g_layer2.get_sprite(9)->set_background_img("assets/game_page/arrow_left.tga");
	g_layer2.get_sprite(9)->set_background_img_selected("assets/game_page/arrow_left_hover.tga");
	g_layer2.get_sprite(9)->use_background_img();

	Layer& g_layer3 = game_page.get_layer(3);
	g_layer3.add_sprite(10, c_orange_card_pos[0][0], c_card_size_140, c_screen_width, c_screen_height);
	for (int i = 0; i < 14; ++i) {
		g_layer3.get_sprite(10)->add_texture(c_card_texture[i]);
	}
	g_layer3.get_sprite(10)->use_background_img_gl();

	g_layer3.add_sprite(11, c_orange_card_pos[1][0], c_card_size_140, c_screen_width, c_screen_height);
	for (int i = 0; i < 14; ++i) {
		g_layer3.get_sprite(11)->add_texture(c_card_texture[i]);
	}
	g_layer3.get_sprite(11)->use_background_img_gl();

	g_layer3.add_sprite(12, c_orange_card_pos[0][1], c_card_size_140, c_screen_width, c_screen_height);
	for (int i = 0; i < 14; ++i) {
		g_layer3.get_sprite(12)->add_texture(c_card_texture[i]);
	}
	g_layer3.get_sprite(12)->use_background_img_gl();

	g_layer3.add_sprite(13, c_banana_card_pos[0][0], c_card_size_140, c_screen_width, c_screen_height);
	for (int i = 0; i < 14; ++i) {
		g_layer3.get_sprite(13)->add_texture(c_card_texture[i]);
	}
	g_layer3.get_sprite(13)->use_background_img_gl();

	g_layer3.add_sprite(14, c_banana_card_pos[1][0], c_card_size_140, c_screen_width, c_screen_height);
	for (int i = 0; i < 14; ++i) {
		g_layer3.get_sprite(14)->add_texture(c_card_texture[i]);
	}
	g_layer3.get_sprite(14)->use_background_img_gl();

	g_layer3.add_sprite(15, c_banana_card_pos[0][1], c_card_size_140, c_screen_width, c_screen_height);
	for (int i = 0; i < 14; ++i) {
		g_layer3.get_sprite(15)->add_texture(c_card_texture[i]);
	}
	g_layer3.get_sprite(15)->use_background_img_gl();

	Layer& g_layer4 = game_page.get_layer(4);
	g_layer4.set_visibility(false);
	g_layer4.add_sprite(16, glm::vec2(237, 728-559-164), glm::vec2(577, 164), c_screen_width, c_screen_height);
	for (int i = 0; i < 12; ++i) {
		g_layer4.get_sprite(16)->add_texture(c_card_desc_texture[i]);
	}
	g_layer4.get_sprite(16)->use_background_img_gl();

	m_ui_advertiser.add_page();
	m_ui_advertiser.set_active_page(0);
	Page& advert_page = m_ui_advertiser.get_page(0);
	advert_page.add_layer(0);
	Layer& advert_layer0 = advert_page.get_layer(0);
	advert_layer0.add_sprite(0, glm::vec2(-50, 339), glm::vec2(1150, 60), c_screen_width, c_screen_height);
	for (int i = 0; i < 24; ++i) {
		advert_layer0.get_sprite(0)->add_texture(c_advertiser_texture[i]);
	}
	advert_layer0.get_sprite(0)->use_background_img_gl();
	advert_layer0.get_sprite(0)->set_background_img_gl(advert_layer0.get_sprite(0)->get_texture_id(0));

	m_ui_end_game.add_page();
	m_ui_end_game.set_active_page(0);
	Page& end_game_page = m_ui_end_game.get_page(0);
	end_game_page.add_layer(0); // end game message (victory/defeat) + button to leave game

	Layer& popup_layer = end_game_page.get_layer(0);
	popup_layer.add_sprite(0, glm::vec2(191,728-337-196), glm::vec2(667, 337), c_screen_width, c_screen_height);
	popup_layer.get_sprite(0)->add_texture("assets/game_page/victory.tga");
	popup_layer.get_sprite(0)->add_texture("assets/game_page/defeat.tga");
	popup_layer.get_sprite(0)->use_background_img_gl();
	popup_layer.get_sprite(0)->set_background_img_gl(popup_layer.get_sprite(0)->get_texture_id(1));
	popup_layer.add_sprite(1, glm::vec2(436, 728 - 78 - 196 - 212), glm::vec2(178, 78), c_screen_width, c_screen_height);
	popup_layer.get_sprite(1)->set_background_img("assets/common/ok.tga");
	popup_layer.get_sprite(1)->set_background_img_selected("assets/common/ok_hover.tga");
	popup_layer.get_sprite(1)->use_background_img();
}

void Bandas::init_cards(std::string cards)
{
	Layer& g_layer3 = m_ui.get_page(1).get_layer(3);
	int next_token{ -1 };
	int card_id{ -1 };

	// orange cards
	next_token = cards.find_first_of('.');
	card_id = std::atoi(cards.substr(0, next_token).data());
	cards = cards.substr(next_token + 1);
	m_orange_cards[0].m_id = card_id;

	next_token = cards.find_first_of('.');
	card_id = std::atoi(cards.substr(0, next_token).data());
	cards = cards.substr(next_token + 1);
	m_orange_cards[1].m_id = card_id;

	next_token = cards.find_first_of('.');
	card_id = std::atoi(cards.substr(0, next_token).data());
	cards = cards.substr(next_token + 1);
	m_orange_cards[2].m_id = card_id;

	// banana cards
	next_token = cards.find_first_of('.');
	card_id = std::atoi(cards.substr(0, next_token).data());
	cards = cards.substr(next_token + 1);
	m_banana_cards[0].m_id = card_id;

	next_token = cards.find_first_of('.');
	card_id = std::atoi(cards.substr(0, next_token).data());
	cards = cards.substr(next_token + 1);
	m_banana_cards[1].m_id = card_id;

	next_token = cards.find_first_of('.');
	card_id = std::atoi(cards.substr(0, next_token).data());
	cards = cards.substr(next_token + 1);
	m_banana_cards[2].m_id = card_id;
}

void Bandas::update_home_page(std::bitset<10> user_input, std::string txt_input, float delta)
{
	glm::ivec2 mouse_pos = m_mouse.get_position();
	std::shared_ptr<Sprite> hovered = m_ui.get_hovered_sprite(mouse_pos.x, c_screen_height - mouse_pos.y);

	if (!hovered) { return; }
	Page& home_page{ m_ui.get_page(0) };
	int sprite_id{ hovered->get_id() };

	hovering_home_page(home_page, sprite_id);
	if (user_input.test(2) && user_input.test(9)) // if click & release events
	{
		click_home_page(home_page, sprite_id);
		update_avatar(home_page, sprite_id);
	}
	if (m_writer.m_cursor.m_focus == 0)
	{
		int boundX = home_page.get_layer(13).get_sprite(64)->get_position().x + home_page.get_layer(13).get_sprite(64)->get_size().x;
		glm::vec3 cursor_shape = m_text.get_cursor_shape(m_writer.m_textInput[0], 525 - 72, 272, 1, m_writer.m_cursor.m_pos);
		m_writer.write(txt_input, user_input, delta, boundX, cursor_shape);
		m_me.m_pseudo = m_writer.m_textInput[0];
	}
}

void Bandas::hovering_home_page(Page& page, int id)
{
	if (id >= 2 && id <= 6) // hover face
	{
		page.get_layer(2).get_sprite(id)->use_background_img_selected();
		for (int i = 2; i <= 6; ++i) {
			if (i == id) { continue; }
			else { page.get_layer(2).get_sprite(i)->use_background_img(); }
		}
	}
	else
	{
		for (int i = 2; i <= 6; ++i)
		{
			page.get_layer(2).get_sprite(i)->use_background_img();
		}
	}

	if (id == 7 || id == 8) // visage type
	{
		page.get_layer(3).get_sprite(id)->use_background_img_selected();
		for (int i = 7; i <= 8; ++i) {
			if (i == id) { continue; }
			else { page.get_layer(3).get_sprite(i)->use_background_img(); }
		}
	}
	else
	{
		page.get_layer(3).get_sprite(7)->use_background_img();
		page.get_layer(3).get_sprite(8)->use_background_img();
	}

	if (id >= 9 && id <= 21) // cheveux homme
	{
		page.get_layer(4).get_sprite(id)->use_background_img_selected();
		for (int i = 9; i <= 21; ++i) {
			if (i == id) { continue; }
			else { page.get_layer(4).get_sprite(i)->use_background_img(); }
		}
	}
	else
	{
		for (int i = 9; i <= 21; ++i)
		{
			page.get_layer(4).get_sprite(i)->use_background_img();
		}
	}

	if (id >= 22 && id <= 33) // cheveux femme
	{
		page.get_layer(5).get_sprite(id)->use_background_img_selected();
		for (int i = 22; i <= 33; ++i) {
			if (i == id) { continue; }
			else { page.get_layer(5).get_sprite(i)->use_background_img(); }
		}
	}
	else
	{
		for (int i = 22; i <= 33; ++i)
		{
			page.get_layer(5).get_sprite(i)->use_background_img();
		}
	}

	if (id >= 34 && id <= 39) // yeux homme
	{
		page.get_layer(6).get_sprite(id)->use_background_img_selected();
		for (int i = 34; i <= 39; ++i) {
			if (i == id) { continue; }
			else { page.get_layer(6).get_sprite(i)->use_background_img(); }
		}
	}
	else
	{
		for (int i = 34; i <= 39; ++i)
		{
			page.get_layer(6).get_sprite(i)->use_background_img();
		}
	}

	if (id >= 40 && id <= 45) // yeux femme
	{
		page.get_layer(7).get_sprite(id)->use_background_img_selected();
		for (int i = 40; i <= 45; ++i) {
			if (i == id) { continue; }
			else { page.get_layer(7).get_sprite(i)->use_background_img(); }
		}
	}
	else
	{
		for (int i = 40; i <= 45; ++i)
		{
			page.get_layer(7).get_sprite(i)->use_background_img();
		}
	}

	if (id >= 46 && id <= 51) // bouche
	{
		page.get_layer(8).get_sprite(id)->use_background_img_selected();
		for (int i = 46; i <= 51; ++i) {
			if (i == id) { continue; }
			else { page.get_layer(8).get_sprite(i)->use_background_img(); }
		}
	}
	else
	{
		for (int i = 46; i <= 51; ++i)
		{
			page.get_layer(8).get_sprite(i)->use_background_img();
		}
	}

	if (id >= 52 && id <= 55) // sac
	{
		page.get_layer(9).get_sprite(id)->use_background_img_selected();
		for (int i = 52; i <= 55; ++i) {
			if (i == id) { continue; }
			else { page.get_layer(9).get_sprite(i)->use_background_img(); }
		}
	}
	else
	{
		for (int i = 52; i <= 55; ++i)
		{
			page.get_layer(9).get_sprite(i)->use_background_img();
		}
	}

	if (id == 61) // choix couleur
	{
		page.get_layer(12).get_sprite(id)->set_bloom_strength(1.5f);
	}
	else if (id == 62) // choix couleur
	{
		page.get_layer(12).get_sprite(id)->set_bloom_strength(1.5f);
	}
	else
	{
		page.get_layer(12).get_sprite(61)->set_bloom_strength(1.0f);
		page.get_layer(12).get_sprite(62)->set_bloom_strength(1.0f);
	}

	if (id == 63) // quitter le jeu
	{
		page.get_layer(12).get_sprite(id)->set_bloom_strength(100000.0f);
	}
	else
	{
		page.get_layer(12).get_sprite(63)->set_bloom_strength(1.0f);
	}

	if (id == 65) // connexion
	{
		page.get_layer(13).get_sprite(id)->use_background_img_selected();
	}
	else
	{
		page.get_layer(13).get_sprite(65)->use_background_img();
	}

	if (id == 67) // connexion
	{
		page.get_layer(15).get_sprite(id)->use_background_img_selected();
	}
	else
	{
		page.get_layer(15).get_sprite(67)->use_background_img();
	}

	if (id == 68) // connexion
	{
		page.get_layer(16).get_sprite(id)->use_background_img_selected();
	}
	else
	{
		page.get_layer(16).get_sprite(68)->use_background_img();
	}

	if (id == 70) // ok button from popup messages
	{
		page.get_layer(17).get_sprite(id)->use_background_img_selected();
	}
	else
	{
		page.get_layer(17).get_sprite(70)->use_background_img();
	}
}

void Bandas::click_home_page(Page& page, int id)
{
	if (id != 64)
	{
		m_writer.m_cursor.m_focus = 2;
		page.get_layer(13).get_sprite(64)->set_background_color(glm::vec4(0.906f, 0.957f, 0.808f, 1.0f));
	}

	if (id == 2) // face
	{
		page.get_layer(2).get_sprite(id)->select();
		page.get_layer(2).get_sprite(3)->unselect();
		page.get_layer(2).get_sprite(4)->unselect();
		page.get_layer(2).get_sprite(5)->unselect();
		page.get_layer(2).get_sprite(6)->unselect();
		// highlight
		page.get_layer(1).get_sprite(1)->set_pos(glm::vec2(400, 728 - (188 + 48 * (id - 2))));
		page.get_layer(1).set_visibility(true);
		// show options
		if(page.get_layer(3).m_visible){ page.get_layer(3).set_visibility(false); }
		else{ page.get_layer(3).set_visibility(true); }
		// hide others
		page.get_layer(4).set_visibility(false);
		page.get_layer(5).set_visibility(false);
		page.get_layer(6).set_visibility(false);
		page.get_layer(7).set_visibility(false);
		page.get_layer(8).set_visibility(false);
		page.get_layer(9).set_visibility(false);
	}
	else if (id == 3) // hair
	{
		page.get_layer(2).get_sprite(id)->select();
		page.get_layer(2).get_sprite(2)->unselect();
		page.get_layer(2).get_sprite(4)->unselect();
		page.get_layer(2).get_sprite(5)->unselect();
		page.get_layer(2).get_sprite(6)->unselect();
		// highlight
		page.get_layer(1).get_sprite(1)->set_pos(glm::vec2(400, 728 - (188 + 48 * (id - 2))));
		page.get_layer(1).set_visibility(true);
		// show options
		if(m_me.m_avatar.m_gender == Avatar::GENDER::MALE)
		{
			if (page.get_layer(4).m_visible) { page.get_layer(4).set_visibility(false); }
			else { page.get_layer(4).set_visibility(true); }
			// hide others
			page.get_layer(3).set_visibility(false);
			page.get_layer(5).set_visibility(false);
			page.get_layer(6).set_visibility(false);
			page.get_layer(7).set_visibility(false);
			page.get_layer(8).set_visibility(false);
			page.get_layer(9).set_visibility(false);
		}
		else
		{
			if (page.get_layer(5).m_visible) { page.get_layer(5).set_visibility(false); }
			else { page.get_layer(5).set_visibility(true); }
			// hide others
			page.get_layer(3).set_visibility(false);
			page.get_layer(4).set_visibility(false);
			page.get_layer(6).set_visibility(false);
			page.get_layer(7).set_visibility(false);
			page.get_layer(8).set_visibility(false);
			page.get_layer(9).set_visibility(false);
		}
	}
	else if (id == 4) // eyes
	{
		page.get_layer(2).get_sprite(id)->select();
		page.get_layer(2).get_sprite(2)->unselect();
		page.get_layer(2).get_sprite(3)->unselect();
		page.get_layer(2).get_sprite(5)->unselect();
		page.get_layer(2).get_sprite(6)->unselect();
		// highlight
		page.get_layer(1).get_sprite(1)->set_pos(glm::vec2(400, 728 - (188 + 48 * (id - 2))));
		page.get_layer(1).set_visibility(true);
		// show options
		if (m_me.m_avatar.m_gender == Avatar::GENDER::MALE)
		{
			if (page.get_layer(6).m_visible) { page.get_layer(6).set_visibility(false); }
			else { page.get_layer(6).set_visibility(true); }
			// hide others
			page.get_layer(3).set_visibility(false);
			page.get_layer(4).set_visibility(false);
			page.get_layer(5).set_visibility(false);
			page.get_layer(7).set_visibility(false);
			page.get_layer(8).set_visibility(false);
			page.get_layer(9).set_visibility(false);
		}
		else
		{
			if (page.get_layer(7).m_visible) { page.get_layer(7).set_visibility(false); }
			else { page.get_layer(7).set_visibility(true); }
			// hide others
			page.get_layer(3).set_visibility(false);
			page.get_layer(4).set_visibility(false);
			page.get_layer(5).set_visibility(false);
			page.get_layer(6).set_visibility(false);
			page.get_layer(8).set_visibility(false);
			page.get_layer(9).set_visibility(false);
		}
	}
	else if (id == 5) // mouth
	{
		page.get_layer(2).get_sprite(id)->select();
		page.get_layer(2).get_sprite(2)->unselect();
		page.get_layer(2).get_sprite(3)->unselect();
		page.get_layer(2).get_sprite(4)->unselect();
		page.get_layer(2).get_sprite(6)->unselect();
		// highlight
		page.get_layer(1).get_sprite(1)->set_pos(glm::vec2(400, 728 - (188 + 48 * (id - 2))));
		page.get_layer(1).set_visibility(true);
		// show options
		if (page.get_layer(8).m_visible) { page.get_layer(8).set_visibility(false); }
		else { page.get_layer(8).set_visibility(true); }
		// hide others
		page.get_layer(3).set_visibility(false);
		page.get_layer(4).set_visibility(false);
		page.get_layer(5).set_visibility(false);
		page.get_layer(6).set_visibility(false);
		page.get_layer(7).set_visibility(false);
		page.get_layer(9).set_visibility(false);
	}
	else if (id == 6) // burlap sack
	{
		page.get_layer(2).get_sprite(id)->select();
		page.get_layer(2).get_sprite(2)->unselect();
		page.get_layer(2).get_sprite(3)->unselect();
		page.get_layer(2).get_sprite(4)->unselect();
		page.get_layer(2).get_sprite(5)->unselect();
		// highlight
		page.get_layer(1).get_sprite(1)->set_pos(glm::vec2(400, 728 - (188 + 48 * (id - 2))));
		page.get_layer(1).set_visibility(true);
		// show options
		if (page.get_layer(9).m_visible) { page.get_layer(9).set_visibility(false); }
		else { page.get_layer(9).set_visibility(true); }
		// hide others
		page.get_layer(3).set_visibility(false);
		page.get_layer(4).set_visibility(false);
		page.get_layer(5).set_visibility(false);
		page.get_layer(6).set_visibility(false);
		page.get_layer(7).set_visibility(false);
		page.get_layer(8).set_visibility(false);
	}
	else
	{
		page.get_layer(3).set_visibility(false);
		page.get_layer(4).set_visibility(false);
		page.get_layer(5).set_visibility(false);
		page.get_layer(6).set_visibility(false);
		page.get_layer(7).set_visibility(false);
		page.get_layer(8).set_visibility(false);
		page.get_layer(9).set_visibility(false);
	}

	if (id == 63) // quitter le jeu
	{
		g_msg2server_mtx.lock();
		g_leave_game = true;
		g_msg2server_mtx.unlock();
		g_cv_connect_leave.notify_one();
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}

	if (id == 64) // pseudo input
	{
		m_writer.m_cursor.m_focus = 0;
		page.get_layer(13).get_sprite(64)->set_background_color(glm::vec4(0.849f, 0.898f, 0.754f, 1.0f));
	}

	if (id == 65) // clicked on connect
	{
		g_msg2server_mtx.lock();
		g_msg2server.emplace("0");
		g_try2connect = true;
		g_msg2server_mtx.unlock();
		g_cv_connect_leave.notify_one();
	}

	if (id == 67) // clicked on play
	{
		g_msg2server_mtx.lock();
		g_msg2server.emplace("1");
		g_msg2server_mtx.unlock();
	}

	if (id == 68) // clicked on stop search opponent
	{
		g_msg2server_mtx.lock();
		g_msg2server.emplace("2");
		g_msg2server_mtx.unlock();
	}

	if (id == 70)
	{
		page.get_layer(17).set_visibility(false);
	}
}

void Bandas::update_avatar(Page& page, int id)
{	
	if (id == 59) // switch to male
	{
		page.get_layer(10).get_sprite(id)->use_background_img_selected();
		page.get_layer(10).get_sprite(id)->select();
		page.get_layer(10).get_sprite(58)->use_background_img();
		page.get_layer(10).get_sprite(58)->unselect();
		m_me.m_avatar.m_gender = Avatar::GENDER::MALE;
	}
	else if (id == 58) // switch to female
	{
		page.get_layer(10).get_sprite(id)->use_background_img_selected();
		page.get_layer(10).get_sprite(id)->select();
		page.get_layer(10).get_sprite(59)->use_background_img();
		page.get_layer(10).get_sprite(59)->unselect();
		m_me.m_avatar.m_gender = Avatar::GENDER::FEMALE;
	}

	if (m_me.m_avatar.m_gender == Avatar::GENDER::MALE)
	{
		// visage homme
		if(id == 7) { m_me.m_avatar.m_homme.m_face = Avatar::FACE::NORMAL; }
		else if (id == 8) { m_me.m_avatar.m_homme.m_face = Avatar::FACE::TACHES; }
		// cheveux homme
		if (id == 9) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::ARRIERE; }
		else if (id == 10) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::BULBE; }
		else if (id == 11) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::DECOIFFE; }
		else if (id == 12) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::ELVIS; }
		else if (id == 13) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::EPI1; }
		else if (id == 14) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::EPI2; }
		else if (id == 15) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::EPI3; }
		else if (id == 16) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::HERISSON; }
		else if (id == 17) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::CRETE; }
		else if (id == 18) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::KASPA; }
		else if (id == 19) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::MECHE_AVANT; }
		else if (id == 20) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::MIXTE; }
		else if (id == 21) { m_me.m_avatar.m_homme.m_hair = Avatar::HAIR::STEADY; }
		// yeux homme
		if (id == 34) { m_me.m_avatar.m_homme.m_eyes = Avatar::EYES::MANGA; }
		else if (id == 35) { m_me.m_avatar.m_homme.m_eyes = Avatar::EYES::GROS; }
		else if (id == 36) { m_me.m_avatar.m_homme.m_eyes = Avatar::EYES::COLERE; }
		else if (id == 37) { m_me.m_avatar.m_homme.m_eyes = Avatar::EYES::FATIGUE; }
		else if (id == 38) { m_me.m_avatar.m_homme.m_eyes = Avatar::EYES::PSYCHO; }
		else if (id == 39) { m_me.m_avatar.m_homme.m_eyes = Avatar::EYES::AMANDE; }
		// bouche homme
		if (id == 46) { m_me.m_avatar.m_homme.m_mouth = Avatar::MOUTH::PETITE; }
		else if (id == 47) { m_me.m_avatar.m_homme.m_mouth = Avatar::MOUTH::MOYENNE; }
		else if (id == 48) { m_me.m_avatar.m_homme.m_mouth = Avatar::MOUTH::GRANDE; }
		else if (id == 49) { m_me.m_avatar.m_homme.m_mouth = Avatar::MOUTH::SOURIRE; }
		else if (id == 50) { m_me.m_avatar.m_homme.m_mouth = Avatar::MOUTH::MACHOIRE_SOURIRE; }
		else if (id == 51) { m_me.m_avatar.m_homme.m_mouth = Avatar::MOUTH::TETINE; }
		// sac homme
		if (id == 52) { m_me.m_avatar.m_homme.m_sac = Avatar::SAC::LVL1; }
		else if (id == 53) { m_me.m_avatar.m_homme.m_sac = Avatar::SAC::LVL2; }
		else if (id == 54) { m_me.m_avatar.m_homme.m_sac = Avatar::SAC::LVL3; }
		else if (id == 55) { m_me.m_avatar.m_homme.m_sac = Avatar::SAC::NONE; }
		// couleur
		if (id == 61) // gauche
		{
			if (page.get_layer(2).get_sprite(2)->is_selected()) // visage
			{
				m_me.m_avatar.m_homme.m_skin_color_id--;
				if (m_me.m_avatar.m_homme.m_skin_color_id < 0) { m_me.m_avatar.m_homme.m_skin_color_id = m_me.m_avatar.m_skin_color.size() - 1; }
			}
			else if (page.get_layer(2).get_sprite(3)->is_selected()) // cheveux
			{
				m_me.m_avatar.m_homme.m_hair_color_id--;
				if (m_me.m_avatar.m_homme.m_hair_color_id < 0) { m_me.m_avatar.m_homme.m_hair_color_id = m_me.m_avatar.m_hair_color.size() - 1; }
			}
			else if (page.get_layer(2).get_sprite(4)->is_selected()) // yeux
			{
				m_me.m_avatar.m_homme.m_eyes_color_id--;
				if (m_me.m_avatar.m_homme.m_eyes_color_id < 0) { m_me.m_avatar.m_homme.m_eyes_color_id = m_me.m_avatar.m_eyes_color.size() - 1; }
			}
		}
		else if (id == 62) // droite
		{
			if (page.get_layer(2).get_sprite(2)->is_selected()) // visage
			{
				m_me.m_avatar.m_homme.m_skin_color_id++;
				if (m_me.m_avatar.m_homme.m_skin_color_id > m_me.m_avatar.m_skin_color.size() - 1) { m_me.m_avatar.m_homme.m_skin_color_id = 0; }
			}
			else if (page.get_layer(2).get_sprite(3)->is_selected()) // cheveux
			{
				m_me.m_avatar.m_homme.m_hair_color_id++;
				if (m_me.m_avatar.m_homme.m_hair_color_id > m_me.m_avatar.m_hair_color.size() - 1) { m_me.m_avatar.m_homme.m_hair_color_id = 0; }
			}
			else if (page.get_layer(2).get_sprite(4)->is_selected()) // yeux
			{
				m_me.m_avatar.m_homme.m_eyes_color_id++;
				if (m_me.m_avatar.m_homme.m_eyes_color_id > m_me.m_avatar.m_eyes_color.size() - 1) { m_me.m_avatar.m_homme.m_eyes_color_id = 0; }
			}
		}
	}
	else
	{
		// visage femme
		if (id == 7) { m_me.m_avatar.m_femme.m_face = Avatar::FACE::NORMAL; }
		else if (id == 8) { m_me.m_avatar.m_femme.m_face = Avatar::FACE::TACHES; }
		// cheveux femme
		if (id == 22) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::AU_BOL; }
		else if (id == 23) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::COUETTES; }
		else if (id == 24) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::DECOIFFE; }
		else if (id == 25) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::FRANGE; }
		else if (id == 26) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::MEDUSE; }
		else if (id == 27) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::MI_LONG; }
		else if (id == 28) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::BOULES; }
		else if (id == 29) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::MIXTE; }
		else if (id == 30) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::NATTES; }
		else if (id == 31) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::PONYTAIL; }
		else if (id == 32) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::RAIE; }
		else if (id == 33) { m_me.m_avatar.m_femme.m_hair = Avatar::HAIR::SEVENTIES; }
		// yeux femme
		if (id == 40) { m_me.m_avatar.m_femme.m_eyes = Avatar::EYES::EGYPTE; }
		else if (id == 41) { m_me.m_avatar.m_femme.m_eyes = Avatar::EYES::MANGA; }
		else if (id == 42) { m_me.m_avatar.m_femme.m_eyes = Avatar::EYES::INQUIETE; }
		else if (id == 43) { m_me.m_avatar.m_femme.m_eyes = Avatar::EYES::COLERE; }
		else if (id == 44) { m_me.m_avatar.m_femme.m_eyes = Avatar::EYES::FATIGUE; }
		else if (id == 45) { m_me.m_avatar.m_femme.m_eyes = Avatar::EYES::PSYCHO; }
		// bouche femme
		if (id == 46) { m_me.m_avatar.m_femme.m_mouth = Avatar::MOUTH::PETITE; }
		else if (id == 47) { m_me.m_avatar.m_femme.m_mouth = Avatar::MOUTH::MOYENNE; }
		else if (id == 48) { m_me.m_avatar.m_femme.m_mouth = Avatar::MOUTH::GRANDE; }
		else if (id == 49) { m_me.m_avatar.m_femme.m_mouth = Avatar::MOUTH::SOURIRE; }
		else if (id == 50) { m_me.m_avatar.m_femme.m_mouth = Avatar::MOUTH::MACHOIRE_SOURIRE; }
		else if (id == 51) { m_me.m_avatar.m_femme.m_mouth = Avatar::MOUTH::TETINE; }
		// sac femme
		if (id == 52) { m_me.m_avatar.m_femme.m_sac = Avatar::SAC::LVL1; }
		else if (id == 53) { m_me.m_avatar.m_femme.m_sac = Avatar::SAC::LVL2; }
		else if (id == 54) { m_me.m_avatar.m_femme.m_sac = Avatar::SAC::LVL3; }
		else if (id == 55) { m_me.m_avatar.m_femme.m_sac = Avatar::SAC::NONE; }
		// couleur
		if (id == 61) // gauche
		{
			if (page.get_layer(2).get_sprite(2)->is_selected()) // visage
			{
				m_me.m_avatar.m_femme.m_skin_color_id--;
				if (m_me.m_avatar.m_femme.m_skin_color_id < 0) { m_me.m_avatar.m_femme.m_skin_color_id = m_me.m_avatar.m_skin_color.size() - 1; }
			}
			else if (page.get_layer(2).get_sprite(3)->is_selected()) // cheveux
			{
				m_me.m_avatar.m_femme.m_hair_color_id--;
				if (m_me.m_avatar.m_femme.m_hair_color_id < 0) { m_me.m_avatar.m_femme.m_hair_color_id = m_me.m_avatar.m_hair_color.size() - 1; }
			}
			else if (page.get_layer(2).get_sprite(4)->is_selected()) // yeux
			{
				m_me.m_avatar.m_femme.m_eyes_color_id--;
				if (m_me.m_avatar.m_femme.m_eyes_color_id < 0) { m_me.m_avatar.m_femme.m_eyes_color_id = m_me.m_avatar.m_eyes_color.size() - 1; }
			}
		}
		else if (id == 62) // droite
		{
			if (page.get_layer(2).get_sprite(2)->is_selected()) // visage
			{
				m_me.m_avatar.m_femme.m_skin_color_id++;
				if (m_me.m_avatar.m_femme.m_skin_color_id > m_me.m_avatar.m_skin_color.size() - 1) { m_me.m_avatar.m_femme.m_skin_color_id = 0; }
			}
			else if (page.get_layer(2).get_sprite(3)->is_selected()) // cheveux
			{
				m_me.m_avatar.m_femme.m_hair_color_id++;
				if (m_me.m_avatar.m_femme.m_hair_color_id > m_me.m_avatar.m_hair_color.size() - 1) { m_me.m_avatar.m_femme.m_hair_color_id = 0; }
			}
			else if (page.get_layer(2).get_sprite(4)->is_selected()) // yeux
			{
				m_me.m_avatar.m_femme.m_eyes_color_id++;
				if (m_me.m_avatar.m_femme.m_eyes_color_id > m_me.m_avatar.m_eyes_color.size() - 1) { m_me.m_avatar.m_femme.m_eyes_color_id = 0; }
			}
		}
	}

}

void Bandas::draw_home_page(float delta)
{
	Page& home_page{ m_ui.get_page(0) };

	// draw avatar
	m_graphics.avatarFBO->bind();
	glViewport(0, 0, c_avatar_width, c_avatar_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_me.m_avatar.draw();

	// draw home page
	m_graphics.userInterfaceFBO->bind();
	glViewport(0, 0, c_screen_width, c_screen_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_ui.get_page(0).draw();

	// check connection status
	if (m_net.is_trying_to_connect())
	{
		m_text.print("trying to connect...", 525 - 85, 272, 1, glm::vec3(0));
		home_page.get_layer(13).set_visibility(false);
		home_page.get_layer(15).set_visibility(false);
		home_page.get_layer(16).set_visibility(false);
	}
	else
	{
		if (m_net.is_connected())
		{
			home_page.get_layer(14).get_sprite(66)->use_background_img_selected();
			home_page.get_layer(13).set_visibility(false);
			if (m_net.is_searching_game())
			{
				m_text.print("searching opponent...", 525 - 86, 272, 1, glm::vec3(0));
				home_page.get_layer(15).set_visibility(false);
				home_page.get_layer(16).set_visibility(true);

				// no more avatar editing
				home_page.get_layer(1).m_sg.set_grey(true);
				home_page.get_layer(1).m_sg.set_selectable(false);
				home_page.get_layer(2).m_sg.set_grey(true);
				home_page.get_layer(2).m_sg.set_selectable(false);
				home_page.get_layer(10).m_sg.set_grey(true);
				home_page.get_layer(10).m_sg.set_selectable(false);
				home_page.get_layer(12).get_sprite(61)->set_grey(true);
				home_page.get_layer(12).get_sprite(61)->set_selectable(false);
				home_page.get_layer(12).get_sprite(62)->set_grey(true);
				home_page.get_layer(12).get_sprite(62)->set_selectable(false);
			}
			else
			{
				home_page.get_layer(15).set_visibility(true);
				home_page.get_layer(16).set_visibility(false);

				// avatar editing available
				home_page.get_layer(1).m_sg.set_grey(false);
				home_page.get_layer(1).m_sg.set_selectable(true);
				home_page.get_layer(2).m_sg.set_grey(false);
				home_page.get_layer(2).m_sg.set_selectable(true);
				home_page.get_layer(10).m_sg.set_grey(false);
				home_page.get_layer(10).m_sg.set_selectable(true);
				home_page.get_layer(12).get_sprite(61)->set_grey(false);
				home_page.get_layer(12).get_sprite(61)->set_selectable(true);
				home_page.get_layer(12).get_sprite(62)->set_grey(false);
				home_page.get_layer(12).get_sprite(62)->set_selectable(true);
			}
		}
		else
		{
			home_page.get_layer(14).get_sprite(66)->use_background_img();
			home_page.get_layer(13).set_visibility(true);
			home_page.get_layer(15).set_visibility(false);
			home_page.get_layer(16).set_visibility(false);
			// draw pseudo input and cursor
			if (!home_page.get_layer(17).m_visible)
			{
				m_text.print(m_writer.m_textInput[0], 525 - 72, 272, 1, glm::vec3(0));
				if (m_writer.m_cursor.m_focus == 0)
				{
					glm::vec3 cursor_shape = m_text.get_cursor_shape(m_writer.m_textInput[0], 525 - 72, 272, 1, m_writer.m_cursor.m_pos);
					m_writer.m_cursor.draw(cursor_shape, delta);
				}
			}
		}
	}
}

void Bandas::start_game()
{
	m_ui.set_active_page(1);
	// stop focus text input
	m_writer.m_cursor.m_focus = 2;
	m_writer.m_cursor.m_pos = 0;
	// no more searching game
	m_net.search_game(false);
}

void Bandas::quit_game()
{
	m_ui.set_active_page(0);
	// if end game, set abandon button as selectable again
	m_ui.get_page(1).get_layer(0).get_sprite(3)->set_selectable(true);
	// stop focus text input
	m_writer.m_cursor.m_focus = 2;
	m_writer.m_cursor.m_pos = static_cast<int>(m_writer.m_textInput[0].size());
	// clear chat log
	m_writer.m_chatLog.clear();
	// hide music sound controller
	music.visible = false;
	// reset board
	m_board.reset();
	// reset game logic
	m_logic.reset();
	// reset base end game texture to "defeat"
	GLuint tex_id = m_ui_end_game.get_page(0).get_layer(0).get_sprite(0)->get_texture_id(1);
	m_ui_end_game.get_page(0).get_layer(0).get_sprite(0)->set_background_img_gl(tex_id);
	// reset chrono
	m_me.m_chrono.reset();
	m_enemy.m_chrono.reset();
	// flush advertiser queue
	m_advertiser.clear();
}

void Bandas::enemy_gave_up()
{
	quit_game();
	m_ui.get_page(0).get_layer(17).set_visibility(true);
	Layer& h_layer17 = m_ui.get_page(0).get_layer(17);
	h_layer17.get_sprite(69)->set_background_img_gl(h_layer17.get_sprite(69)->get_texture_id(0));
}

void Bandas::enemy_disconnected()
{
	quit_game();
	m_ui.get_page(0).get_layer(17).set_visibility(true);
	Layer& h_layer17 = m_ui.get_page(0).get_layer(17);
	h_layer17.get_sprite(69)->set_background_img_gl(h_layer17.get_sprite(69)->get_texture_id(1));
}

void Bandas::lost_server_connection()
{
	quit_game();
	m_ui.get_page(0).get_layer(17).set_visibility(true);
	Layer& h_layer17 = m_ui.get_page(0).get_layer(17);
	h_layer17.get_sprite(69)->set_background_img_gl(h_layer17.get_sprite(69)->get_texture_id(2));
	// avatar editing available
	Page& home_page = m_ui.get_page(0);
	home_page.get_layer(1).m_sg.set_grey(false);
	home_page.get_layer(1).m_sg.set_selectable(true);
	home_page.get_layer(2).m_sg.set_grey(false);
	home_page.get_layer(2).m_sg.set_selectable(true);
	home_page.get_layer(10).m_sg.set_grey(false);
	home_page.get_layer(10).m_sg.set_selectable(true);
	home_page.get_layer(12).get_sprite(61)->set_grey(false);
	home_page.get_layer(12).get_sprite(61)->set_selectable(true);
	home_page.get_layer(12).get_sprite(62)->set_grey(false);
	home_page.get_layer(12).get_sprite(62)->set_selectable(true);
}

void Bandas::update_game_page(std::array<int, 3> mouse_data, std::bitset<10> user_input, std::string txt_input, float delta)
{
	glm::ivec2 mouse_pos = m_mouse.get_position();
	std::shared_ptr<Sprite> hovered = m_ui.get_hovered_sprite(mouse_pos.x, c_screen_height - mouse_pos.y);

	if (!hovered && !m_logic.game_is_finished) { return; }
	Page& game_page{ m_ui.get_page(1) };
	int sprite_id{ hovered->get_id() };

	hovering_game_page(game_page, sprite_id, mouse_pos);
	if (user_input.test(2) && user_input.test(9)) // if click & release events
	{
		click_game_page(game_page, sprite_id, mouse_pos);
	}
	else if (user_input.test(2) && !user_input.test(9)) // hold left click
	{
		hold_left_click_game_page(game_page, sprite_id, mouse_data);
	}
	else
	{
		music.grab_ctrl = false;
	}
	update_chat_input(user_input, txt_input, delta);

	// check winner
	if ((m_me.m_team == 0 && m_board.banana_count == 0) || (m_me.m_team == 1 && m_board.orange_count == 0) || m_enemy.m_chrono.m_time == 0.0f)
	{
		if (!m_logic.game_is_finished)
		{
			m_logic.game_is_finished = true;
			GLuint tex_id = m_ui_end_game.get_page(0).get_layer(0).get_sprite(0)->get_texture_id(0);
			m_ui_end_game.get_page(0).get_layer(0).get_sprite(0)->set_background_img_gl(tex_id);
			g_msg2server_mtx.lock();
			g_msg2server.emplace("6:" + std::to_string(m_me.m_team));
			g_msg2server_mtx.unlock();
		}
	}

	// events on end game popup
	if (m_logic.game_is_finished)
	{
		m_ui.get_page(1).get_layer(0).get_sprite(3)->set_selectable(false);
		Page& popup{ m_ui_end_game.get_page(0) };
		hovered = m_ui_end_game.get_hovered_sprite(mouse_pos.x, c_screen_height - mouse_pos.y);
		if (!hovered)
		{
			return;
		}
		sprite_id = hovered->get_id();
		if (sprite_id == 1) // ok button
		{
			popup.get_layer(0).get_sprite(1)->use_background_img_selected();
		}
		else
		{
			popup.get_layer(0).get_sprite(1)->use_background_img();
		}
		if (user_input.test(2) && user_input.test(9)) // if click & release events
		{
			if (sprite_id == 1) // ok button
			{
				quit_game();
			}
		}
	}
}

void Bandas::hovering_game_page(Page& page, int id, glm::ivec2 mouse_coords)
{
	// process card events first
	if (m_logic.card_effect.select_enemy_banda || m_logic.card_effect.select_ally_banda || m_logic.card_effect.anvil || m_logic.card_effect.cow)
	{
		glm::ivec2 tile_coords = m_board.get_tile_coords_from_mouse_position(mouse_coords.x, c_screen_height - mouse_coords.y);
		if (tile_coords != glm::ivec2(-1, -1))
		{
			m_board.reset_hovered();
			m_board.tile[tile_coords.x][tile_coords.y].hovered = true;
		}
	}

	if (id == 3) // abandon button
	{
		page.get_layer(0).get_sprite(id)->use_background_img_selected();
	}
	else
	{
		page.get_layer(0).get_sprite(3)->use_background_img();
	}

	if (id == 4) // sound button
	{
		page.get_layer(0).get_sprite(id)->use_background_img_selected();
	}
	else
	{
		page.get_layer(0).get_sprite(4)->use_background_img();
	}

	if (id == 6) // up arrow
	{
		page.get_layer(2).get_sprite(id)->use_background_img_selected();
	}
	else
	{
		page.get_layer(2).get_sprite(6)->use_background_img();
	}
	if (id == 7) // down arrow
	{
		page.get_layer(2).get_sprite(id)->use_background_img_selected();
	}
	else
	{
		page.get_layer(2).get_sprite(7)->use_background_img();
	}
	if (id == 8) // right arrow
	{
		page.get_layer(2).get_sprite(id)->use_background_img_selected();
	}
	else
	{
		page.get_layer(2).get_sprite(8)->use_background_img();
	}
	if (id == 9) // left arrow
	{
		page.get_layer(2).get_sprite(id)->use_background_img_selected();
	}
	else
	{
		page.get_layer(2).get_sprite(9)->use_background_img();
	}

	// print card description
	Layer& g_layer4 = m_ui.get_page(1).get_layer(4);
	if (id == 10 && m_me.m_team == 0 && m_orange_cards[0].m_id != -1)
	{
		g_layer4.set_visibility(true);
		g_layer4.get_sprite(16)->set_background_img_gl(g_layer4.get_sprite(16)->get_texture_id(m_orange_cards[0].m_id));
	}
	else if(id == 11 && m_me.m_team == 0 && m_orange_cards[1].m_id != -1)
	{
		g_layer4.set_visibility(true);
		g_layer4.get_sprite(16)->set_background_img_gl(g_layer4.get_sprite(16)->get_texture_id(m_orange_cards[1].m_id));
	}
	else if (id == 12 && m_me.m_team == 0 && m_orange_cards[2].m_id != -1)
	{
		g_layer4.set_visibility(true);
		g_layer4.get_sprite(16)->set_background_img_gl(g_layer4.get_sprite(16)->get_texture_id(m_orange_cards[2].m_id));
	}
	else if (id == 13 && m_me.m_team == 1 && m_banana_cards[0].m_id != -1)
	{
		g_layer4.set_visibility(true);
		g_layer4.get_sprite(16)->set_background_img_gl(g_layer4.get_sprite(16)->get_texture_id(m_banana_cards[0].m_id));
	}
	else if (id == 14 && m_me.m_team == 1 && m_banana_cards[1].m_id != -1)
	{
		g_layer4.set_visibility(true);
		g_layer4.get_sprite(16)->set_background_img_gl(g_layer4.get_sprite(16)->get_texture_id(m_banana_cards[1].m_id));
	}
	else if (id == 15 && m_me.m_team == 1 && m_banana_cards[2].m_id != -1)
	{
		g_layer4.set_visibility(true);
		g_layer4.get_sprite(16)->set_background_img_gl(g_layer4.get_sprite(16)->get_texture_id(m_banana_cards[2].m_id));
	}
	else
	{
		g_layer4.set_visibility(false);
	}
}

void Bandas::hold_left_click_game_page(Page& page, int id, std::array<int, 3> mouse_data)
{
	if (!music.grab(m_mouse.get_position()))
	{
		music.visible = false;
	}
	else
	{
		music.tweak_sound_lvl(mouse_data);
	}
}

void Bandas::click_game_page(Page& page, int id, glm::ivec2 mouse_coords)
{
	// process card events first
	if (m_logic.card_effect.select_enemy_banda)
	{
		glm::ivec2 tile_coords = m_board.get_tile_coords_from_mouse_position(mouse_coords.x, c_screen_height - mouse_coords.y);
		char banda_type = (m_me.m_team == 0) ? 'b' : 'o';
		int index;
		for (int i = 0; i < 3; ++i) {
			if (m_orange_cards[i].m_id == 0 || m_banana_cards[i].m_id == 0) { index = i; break; }
		}
		if (m_board.tile[tile_coords.x][tile_coords.y].state == Tile::STATE::ALIVE && m_board.tile[tile_coords.x][tile_coords.y].fruit.type == banda_type) {
			g_msg2server_mtx.lock();
			g_msg2server.emplace("7:0." + std::to_string(index) + "." + std::to_string(tile_coords.x) + "." + std::to_string(tile_coords.y));
			g_msg2server_mtx.unlock();
		}
		else {
			m_logic.used_a_card = false;
		}
		m_logic.card_effect.select_enemy_banda = false;
		m_mouse.use_normal();
		m_board.reset_hovered();
		return;
	}
	else if (m_logic.card_effect.select_ally_banda)
	{
		glm::ivec2 tile_coords = m_board.get_tile_coords_from_mouse_position(mouse_coords.x, c_screen_height - mouse_coords.y);
		char banda_type = (m_me.m_team == 0) ? 'o' : 'b';
		int index;
		for (int i = 0; i < 3; ++i) {
			if (m_orange_cards[i].m_id == 10 || m_banana_cards[i].m_id == 10) { index = i; break; }
		}
		if (m_board.tile[tile_coords.x][tile_coords.y].state == Tile::STATE::ALIVE && m_board.tile[tile_coords.x][tile_coords.y].fruit.type == banda_type) {
			g_msg2server_mtx.lock();
			g_msg2server.emplace("7:10." + std::to_string(index) + "." + std::to_string(tile_coords.x) + "." + std::to_string(tile_coords.y));
			g_msg2server_mtx.unlock();
		}
		else {
			m_logic.used_a_card = false;
		}
		m_logic.card_effect.select_ally_banda = false;
		m_mouse.use_normal();
		m_board.reset_hovered();
		return;
	}
	else if (m_logic.card_effect.anvil)
	{
		glm::ivec2 tile_coords = m_board.get_tile_coords_from_mouse_position(mouse_coords.x, c_screen_height - mouse_coords.y);
		int index;
		for (int i = 0; i < 3; ++i) {
			if (m_orange_cards[i].m_id == 6 || m_banana_cards[i].m_id == 6) { index = i; break; }
		}
		if (m_board.tile[tile_coords.x][tile_coords.y].state == Tile::STATE::ALIVE) {
			g_msg2server_mtx.lock();
			g_msg2server.emplace("7:6." + std::to_string(index) + "." + std::to_string(tile_coords.x) + "." + std::to_string(tile_coords.y));
			g_msg2server_mtx.unlock();
		}
		else {
			m_logic.used_a_card = false;
		}
		m_logic.card_effect.anvil = false;
		m_mouse.use_normal();
		m_board.reset_hovered();
		return;
	}
	else if (m_logic.card_effect.cow)
	{
		glm::ivec2 tile_coords = m_board.get_tile_coords_from_mouse_position(mouse_coords.x, c_screen_height - mouse_coords.y);
		int index;
		for (int i = 0; i < 3; ++i) {
			if (m_orange_cards[i].m_id == 11 || m_banana_cards[i].m_id == 11) { index = i; break; }
		}
		if (m_board.tile[tile_coords.x][tile_coords.y].state == Tile::STATE::ALIVE) {
			g_msg2server_mtx.lock();
			g_msg2server.emplace("7:11." + std::to_string(index) + "." + std::to_string(tile_coords.x) + "." + std::to_string(tile_coords.y));
			g_msg2server_mtx.unlock();
		}
		else {
			m_logic.used_a_card = false;
		}
		m_logic.card_effect.cow = false;
		m_mouse.use_normal();
		m_board.reset_hovered();
		return;
	}
	else if (m_logic.card_effect.petrify)
	{
		glm::ivec2 tile_coords = m_board.get_tile_coords_from_mouse_position(mouse_coords.x, c_screen_height - mouse_coords.y);
		int index;
		for (int i = 0; i < 3; ++i) {
			if (m_orange_cards[i].m_id == 7 || m_banana_cards[i].m_id == 7) { index = i; break; }
		}
		if (m_board.tile[tile_coords.x][tile_coords.y].state == Tile::STATE::ALIVE && m_board.tile[tile_coords.x][tile_coords.y].fruit.type != 'x') {
			g_msg2server_mtx.lock();
			g_msg2server.emplace("7:7." + std::to_string(index) + "." + std::to_string(tile_coords.x) + "." + std::to_string(tile_coords.y));
			g_msg2server_mtx.unlock();
		}
		else {
			m_logic.used_a_card = false;
		}
		m_logic.card_effect.petrify = false;
		m_mouse.use_normal();
		m_board.reset_hovered();
		return;
	}

	if (id == 3) // abandon button
	{
		g_msg2server_mtx.lock();
		g_msg2server.emplace("3");
		g_msg2server_mtx.unlock();
		quit_game();
	}
	
	if (id == 4) // sound button
	{
		music.visible = true;
	}

	if (id == 5) // chat input
	{
		m_writer.m_cursor.m_focus = 1;
		page.get_layer(1).get_sprite(id)->use_background_img_selected();
	}
	else
	{
		m_writer.m_cursor.m_focus = 2;
		page.get_layer(1).get_sprite(5)->use_background_img();
	}

	if (id == 6) // up arrow
	{
		int move_direction = 2;
		if (m_logic.card_effect.disorder && m_logic.card_effect.disorder_destination == m_me.m_team) {
			move_direction = 3;
		}
		g_msg2server_mtx.lock();
		g_msg2server.emplace("5:" + std::to_string(move_direction) + "." + std::to_string(m_me.m_chrono.m_time));
		g_msg2server_mtx.unlock();
	}

	if (id == 7) // down arrow
	{
		int move_direction = 3;
		if (m_logic.card_effect.disorder && m_logic.card_effect.disorder_destination == m_me.m_team) {
			move_direction = 2;
		}
		g_msg2server_mtx.lock();
		g_msg2server.emplace("5:" + std::to_string(move_direction) + "." + std::to_string(m_me.m_chrono.m_time));
		g_msg2server_mtx.unlock();
	}

	if (id == 8) // right arrow
	{
		int move_direction = 0;
		if (m_logic.card_effect.disorder && m_logic.card_effect.disorder_destination == m_me.m_team) {
			move_direction = 1;
		}
		g_msg2server_mtx.lock();
		g_msg2server.emplace("5:" + std::to_string(move_direction) + "." + std::to_string(m_me.m_chrono.m_time));
		g_msg2server_mtx.unlock();
	}

	if (id == 9) // left arrow
	{
		int move_direction = 1;
		if (m_logic.card_effect.disorder && m_logic.card_effect.disorder_destination == m_me.m_team) {
			move_direction = 0;
		}
		g_msg2server_mtx.lock();
		g_msg2server.emplace("5:" + std::to_string(move_direction) + "." + std::to_string(m_me.m_chrono.m_time));
		g_msg2server_mtx.unlock();
	}

	if (!m_logic.used_a_card && m_me.m_team == 0 && m_logic.turn == 0 && (id == 10 || id == 11 || id == 12)) // clicked on orange card
	{
		click_on_orange_card(id - 10);
		m_logic.used_a_card = true;
	}

	if (!m_logic.used_a_card && m_me.m_team == 1 && m_logic.turn == 1 && (id == 13 || id == 14 || id == 15)) // clicked on banana card
	{
		click_on_banana_card(id - 13);
		m_logic.used_a_card = true;
	}
}

void Bandas::draw_avatar_game_page()
{
	// draw avatar
	m_graphics.avatarFBO->bind();
	glViewport(0, 0, c_avatar_width, c_avatar_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_me.m_avatar.draw();
	bool mirror = false;
	if (m_me.m_team == 0) {
		mirror = true;
	}
	if (mirror) {
		m_graphics.cs_horizontal_mirror.use();
		m_graphics.cs_horizontal_mirror.setInt("screen_width", c_avatar_width);
		glBindImageTexture(0, m_graphics.avatarFBO->getAttachments()[0].id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
		glBindImageTexture(1, m_graphics.avatar_mirror_rt.getId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
		m_graphics.cs_horizontal_mirror.dispatch(c_screen_width / 8, c_screen_height / 8, 1, GL_ALL_BARRIER_BITS);
	}

	// draw enemy avatar
	m_graphics.opponentAvatarFBO->bind();
	glViewport(0, 0, c_avatar_width, c_avatar_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_enemy.m_avatar.draw();
	if (!mirror) {
		m_graphics.cs_horizontal_mirror.use();
		m_graphics.cs_horizontal_mirror.setInt("screen_width", c_avatar_width);
		glBindImageTexture(0, m_graphics.opponentAvatarFBO->getAttachments()[0].id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
		glBindImageTexture(1, m_graphics.avatar_mirror_rt.getId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
		m_graphics.cs_horizontal_mirror.dispatch(c_screen_width / 8, c_screen_height / 8, 1, GL_ALL_BARRIER_BITS);
	}
	
	if (m_me.m_team == 0)
	{
		m_ui.get_page(1).get_layer(0).get_sprite(1)->set_background_img_gl(m_graphics.avatar_mirror_rt.getId());			// orange
		m_ui.get_page(1).get_layer(0).get_sprite(2)->set_background_img_gl(m_graphics.opponentAvatarFBO->getAttachments()[0].id);	// banane
	}
	else
	{
		m_ui.get_page(1).get_layer(0).get_sprite(1)->set_background_img_gl(m_graphics.avatar_mirror_rt.getId());	// orange
		m_ui.get_page(1).get_layer(0).get_sprite(2)->set_background_img_gl(m_graphics.avatarFBO->getAttachments()[0].id);			// banane
	}
}

void Bandas::draw_game_page(float delta)
{
	draw_avatar_game_page();
	
	// draw background
	if (m_logic.turn == m_me.m_team && m_logic.move.dir == -1 && !m_logic.change_turn) {
		m_ui.get_page(1).get_layer(2).set_visibility(true);
	}
	else {
		//std::cout << "I DONT PLAY : turn = " << m_logic.turn << "m_me.m_team = " << m_me.m_team << "move dir = " << m_logic.move.dir << "change turn = " << m_logic.change_turn << std::endl;
		m_ui.get_page(1).get_layer(2).set_visibility(false);
	}

	m_graphics.userInterfaceFBO->bind();
	glViewport(0, 0, c_screen_width, c_screen_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_ui.get_page(1).draw();

	// draw chrono
	m_me.m_chrono.draw(m_me.m_team, m_logic, delta);
	m_enemy.m_chrono.draw(m_enemy.m_team, m_logic, delta);
	if (m_me.m_team == 0) {
		m_text.print(m_me.m_chrono.to_string(), 159, 728-152, 1, glm::vec3(0));
		m_text.print(m_enemy.m_chrono.to_string(), 829, 728-152, 1, glm::vec3(0));
	}
	else {
		m_text.print(m_me.m_chrono.to_string(), 829, 728 - 152, 1, glm::vec3(0));
		m_text.print(m_enemy.m_chrono.to_string(), 159, 728 - 152, 1, glm::vec3(0));
	}

	// board
	bool standby = false;
	g_advertiser_mtx.lock();
	if (!m_advertiser.empty()) {
		standby = (m_advertiser.empty()) ? false : (m_advertiser.back().m_show) ? true : false;
	}
	g_advertiser_mtx.unlock();
	m_board.draw(m_logic, delta, standby);

	// draw anvil
	if (m_logic.card_effect.throw_anvil && m_logic.card_effect.anvil_coords != glm::ivec2(-1, -1)) {
		int x = m_logic.card_effect.anvil_coords.x;
		int y = m_logic.card_effect.anvil_coords.y;
		glm::vec2 from = glm::vec2(m_board.tile[x][y].pos.x, 728.0f);
		glm::vec2 to = m_board.tile[x][y].pos;
		if (m_anvil.smoke_on) {
			m_anvil.draw_smoke(to, delta);
			if (!m_anvil.smoke_on) {
				m_logic.card_effect.anvil_coords = glm::ivec2(-1, -1);
				m_logic.card_effect.throw_anvil = false;
			}
		}
		else {
			m_anvil.draw(from, to, delta);
			if (m_anvil.smoke_on) {
				m_board.tile[x][y].state = Tile::STATE::DEAD;
				m_board.tile[x][y].animTimer = 42.0f;
				m_board.tile[x][y].fruit.type = 'x';
				m_board.tile[x][y].fruit.animTimer = 0.0f;
			}
		}
	}
	// draw cow
	if (m_logic.card_effect.cow_charge) {
		int x = m_logic.card_effect.cow_coords.x;
		int y = m_logic.card_effect.cow_coords.y;
		if (!m_cow.draw(m_board, m_board.tile[x][y].pos.x, delta)) {
			m_logic.card_effect.cow_charge = false;
		}
	}

	// print pseudo
	if (m_me.m_team == 0)
	{
		m_text.print(m_me.m_pseudo, 19, 728 - 159 - 20, 1, glm::vec3(0));
		m_text.print(m_enemy.m_pseudo, 839, 728 - 159 - 20, 1, glm::vec3(0));
	}
	else
	{
		m_text.print(m_enemy.m_pseudo, 19, 728 - 159 - 20, 1, glm::vec3(0));
		m_text.print(m_me.m_pseudo, 839, 728 - 159 - 20, 1, glm::vec3(0));
	}

	//print remaining fruits
	m_text.print(std::to_string(m_board.orange_count), 190, 728 - 52, 1, glm::vec3(0));
	m_text.print(std::to_string(m_board.banana_count), 856, 728 - 52, 1, glm::vec3(0));

	// draw cards
	draw_cards();

	// draw advertiser
	g_advertiser_mtx.lock();
	if (!m_advertiser.empty()) {
		struct Advertiser & advertiser = m_advertiser.back();
		if (advertiser.m_show) {
			Layer& advert_layer = m_ui_advertiser.get_page(0).get_layer(0);
			glm::vec2 position = advertiser.get_pos(delta);
			int texture_index = advertiser.m_index * 2;
			texture_index = (advertiser.m_green) ? texture_index : texture_index + 1;
			advert_layer.get_sprite(0)->set_background_img_gl(advert_layer.get_sprite(0)->get_texture_id(texture_index));
			advert_layer.get_sprite(0)->set_pos(position);
			m_ui_advertiser.get_page(0).draw();
		}
		else if (advertiser.erase) {
			if (advertiser.m_index == 0) {
				char bandas_type = (m_logic.turn == 0) ? 'o' : 'b';
				int x = m_logic.card_effect.conversion_coords.x;
				int y = m_logic.card_effect.conversion_coords.y;
				m_board.tile[x][y].fruit.type = bandas_type;
				m_logic.card_effect.conversion_coords = glm::ivec2(-1, -1);
			}
			else if (advertiser.m_index == 6) {
				m_logic.card_effect.throw_anvil = true;
			}
			else if (advertiser.m_index == 7) {
				int x = m_logic.card_effect.petrify_coords.x;
				int y = m_logic.card_effect.petrify_coords.y;
				m_board.tile[x][y].fruit.state = Fruit::STATE::TURN_STONE;
			}
			else if (advertiser.m_index == 9) {
				char bandas_type = (m_logic.turn == 0) ? 'o' : 'b';
				for (int i = 0; i < 3; ++i) {
					int x = m_logic.card_effect.reinforcement[i * 2];
					int y = m_logic.card_effect.reinforcement[i * 2 + 1];
					if (x != -1 && y != -1) {
						m_board.tile[x][y].fruit.type = bandas_type;
						m_board.tile[x][y].fruit.state = Fruit::STATE::STAND_STILL;
						m_board.tile[x][y].fruit.animTimer = 0.0f;
					}
					else {
						m_logic.card_effect.reinforcement = std::array<int, 6>{-1, -1, -1, -1, -1, -1};
						break;
					}
				}
			}
			else if (advertiser.m_index == 11) {
				m_logic.card_effect.cow_charge = true;
				m_cow.create_smoke_data(m_board);
			}
			advertiser.m_index = -1;
			m_advertiser.erase(m_advertiser.begin() + m_advertiser.size()-1);
		}
	}
	g_advertiser_mtx.unlock();

	// print chat
	Layer& g_layer4 = m_ui.get_page(1).get_layer(4);
	if (!g_layer4.m_visible) {
		draw_chat(delta);
	}

	// draw music controller
	music.draw();

	// draw end game popup
	if (m_logic.game_is_finished)
	{
		m_ui_end_game.get_page(0).draw();
	}
}

void Bandas::add_chat_message(std::string msg)
{
	if (m_writer.m_chatLog.size() == 6)
	{
		for (int i{ 0 }; i < 5; ++i)
		{
			m_writer.m_chatLog[i] = m_writer.m_chatLog[i + 1];
		}
		m_writer.m_chatLog[5] = msg;
	}
	else
	{
		m_writer.m_chatLog.push_back(msg);
	}
}

void Bandas::draw_chat(float delta)
{
	// chat input
	m_text.print(m_writer.m_textInput[1], 240 + 13, 728 - 698 - 12, 1, glm::vec3(0));
	// cursor
	if (m_writer.m_cursor.m_focus == 1)
	{
		glm::vec3 cursor_shape = m_text.get_cursor_shape(m_writer.m_textInput[1], 240 + 13, 728 - 698 - 12, 1, m_writer.m_cursor.m_pos);
		m_writer.m_cursor.draw(cursor_shape, delta);
	}
	// conversation
	for (int i{ 1 }; i <= m_writer.m_chatLog.size(); ++i)
	{
		if (m_writer.m_chatLog[i - 1].find_first_of('>') != std::string::npos) {
			m_text.print(m_writer.m_chatLog[i - 1], 240 + 13, 728 - 568 - 20 * i, 1, glm::vec3(0));
		}
		else {
			m_text.print(m_writer.m_chatLog[i - 1], 240 + 13, 728 - 568 - 20 * i, 1, glm::vec3(1,0,0));
		}
	}
}

void Bandas::update_chat_input(std::bitset<10> user_input, std::string txt_input, float delta)
{
	Page& game_page{ m_ui.get_page(1) };
	if (m_writer.m_cursor.m_focus == 1 && !user_input.test(5))
	{
		int boundX = game_page.get_layer(1).get_sprite(5)->get_position().x + game_page.get_layer(1).get_sprite(5)->get_size().x;
		glm::vec3 cursor_shape = m_text.get_cursor_shape(m_writer.m_textInput[1], 525 - 72, 272, 1, m_writer.m_cursor.m_pos);
		m_writer.write(txt_input, user_input, delta, boundX, cursor_shape);
	}
	else if (m_writer.m_cursor.m_focus == 1 && user_input.test(5))
	{
		if (m_writer.m_textInput[1].size() > 0)
		{
			std::string data("4:");
			data += m_writer.m_textInput[1];
			g_msg2server_mtx.lock();
			g_msg2server.emplace(data);
			g_msg2server_mtx.unlock();
			// clear chat input
			m_writer.m_textInput[1].clear();
			// reset cursor pos to zero
			m_writer.m_cursor.m_pos = 0;
		}
	}
}

void Bandas::draw_cards()
{
	Layer& g_layer3 = m_ui.get_page(1).get_layer(3);
	if (m_me.m_team == 0)
	{
		if (m_orange_cards[0].m_id == -1) {
			g_layer3.get_sprite(10)->set_background_img_gl(g_layer3.get_sprite(10)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(10)->set_background_img_gl(g_layer3.get_sprite(10)->get_texture_id(m_orange_cards[0].m_id));
		}
		if (m_orange_cards[1].m_id == -1) {
			g_layer3.get_sprite(11)->set_background_img_gl(g_layer3.get_sprite(11)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(11)->set_background_img_gl(g_layer3.get_sprite(11)->get_texture_id(m_orange_cards[1].m_id));
		}
		if (m_orange_cards[2].m_id == -1) {
			g_layer3.get_sprite(12)->set_background_img_gl(g_layer3.get_sprite(12)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(12)->set_background_img_gl(g_layer3.get_sprite(12)->get_texture_id(m_orange_cards[2].m_id));
		}

		if (m_banana_cards[0].m_id == -1) {
			g_layer3.get_sprite(13)->set_background_img_gl(g_layer3.get_sprite(13)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(13)->set_background_img_gl(g_layer3.get_sprite(13)->get_texture_id(12));
		}
		if (m_banana_cards[1].m_id == -1) {
			g_layer3.get_sprite(14)->set_background_img_gl(g_layer3.get_sprite(14)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(14)->set_background_img_gl(g_layer3.get_sprite(14)->get_texture_id(12));
		}
		if (m_banana_cards[2].m_id == -1) {
			g_layer3.get_sprite(15)->set_background_img_gl(g_layer3.get_sprite(15)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(15)->set_background_img_gl(g_layer3.get_sprite(15)->get_texture_id(12));
		}
	}
	else
	{
		if (m_banana_cards[0].m_id == -1) {
			g_layer3.get_sprite(13)->set_background_img_gl(g_layer3.get_sprite(13)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(13)->set_background_img_gl(g_layer3.get_sprite(13)->get_texture_id(m_banana_cards[0].m_id));
		}
		if (m_banana_cards[1].m_id == -1) {
			g_layer3.get_sprite(14)->set_background_img_gl(g_layer3.get_sprite(14)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(14)->set_background_img_gl(g_layer3.get_sprite(14)->get_texture_id(m_banana_cards[1].m_id));
		}
		if (m_banana_cards[2].m_id == -1) {
			g_layer3.get_sprite(15)->set_background_img_gl(g_layer3.get_sprite(15)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(15)->set_background_img_gl(g_layer3.get_sprite(15)->get_texture_id(m_banana_cards[2].m_id));
		}

		if (m_orange_cards[0].m_id == -1) {
			g_layer3.get_sprite(10)->set_background_img_gl(g_layer3.get_sprite(10)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(10)->set_background_img_gl(g_layer3.get_sprite(10)->get_texture_id(12));
		}
		if (m_orange_cards[1].m_id == -1) {
			g_layer3.get_sprite(11)->set_background_img_gl(g_layer3.get_sprite(11)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(11)->set_background_img_gl(g_layer3.get_sprite(11)->get_texture_id(12));
		}
		if (m_orange_cards[2].m_id == -1) {
			g_layer3.get_sprite(12)->set_background_img_gl(g_layer3.get_sprite(12)->get_texture_id(13));
		}
		else {
			g_layer3.get_sprite(12)->set_background_img_gl(g_layer3.get_sprite(12)->get_texture_id(12));
		}
	}
}

void Bandas::remove_card(int id)
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_banana_cards[i].m_id == id)
		{
			m_banana_cards[i].m_id = -1;
			return;
		}
		else if (m_orange_cards[i].m_id == id)
		{
			m_orange_cards[i].m_id = -1;
			return;
		}
	}
}

void Bandas::process_card_effect(int id, bool delay)
{
	if (!delay)
	{
		remove_card(id);
		switch (id)
		{
		case 2:
			m_logic.card_effect.second_wave = true;
			break;
		case 3:
			m_logic.card_effect.charge = true;
			break;
		case 5:
			m_logic.turn = (m_advertiser.back().m_green) ? m_enemy.m_team : m_me.m_team;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (id)
		{
		case 4:
			m_logic.card_effect.disorder = true;
			break;
		default:
			break;
		}
	}
}

void Bandas::click_on_orange_card(int index)
{
	std::vector<int> list;
	std::string reinforcement_position;
	switch (m_orange_cards[index].m_id)
	{
	case 0: // conversion
		m_mouse.use_target();
		m_logic.card_effect.select_enemy_banda = true;
		break;
	case 1: // confiscation
		std::cout << "clicked on confiscation card" << std::endl;
		break;
	case 2: // celerite
		g_msg2server_mtx.lock();
		g_msg2server.emplace("7:2." + std::to_string(index));
		g_msg2server_mtx.unlock();
		break;
	case 3: // charge
		g_msg2server_mtx.lock();
		g_msg2server.emplace("7:3." + std::to_string(index));
		g_msg2server_mtx.unlock();
		break;
	case 4: // desordre
		g_msg2server_mtx.lock();
		g_msg2server.emplace("7:4." + std::to_string(index) + "." + std::to_string(m_enemy.m_team));
		g_msg2server_mtx.unlock();
		remove_card(4);
		break;
	case 5: // entracte
		g_msg2server_mtx.lock();
		g_msg2server.emplace("7:5." + std::to_string(index));
		g_msg2server_mtx.unlock();
		break;
	case 6: // enclume
		m_mouse.use_target();
		m_logic.card_effect.anvil = true;
		break;
	case 7: // petrification
		m_mouse.use_target();
		m_logic.card_effect.petrify = true;
		break;
	case 8: // piege
		std::cout << "clicked on piege card" << std::endl;
		break;
	case 9: // renfort
		list = m_board.get_free_tiles();
		if (list.size() < 2) {
			add_chat_message("Not enough place to spawn up to 3 bandas");
		}
		else {
			reinforcement_position = m_board.get_reinforcement_position(list);
			g_msg2server_mtx.lock();
			g_msg2server.emplace("7:9." + std::to_string(index) + "." + reinforcement_position);
			g_msg2server_mtx.unlock();
		}
		break;
	case 10: // solo
		m_mouse.use_target();
		m_logic.card_effect.select_ally_banda = true;
		break;
	case 11: // vachette
		m_mouse.use_target();
		m_logic.card_effect.cow = true;
		break;
	default:
		break;
	}
}

void Bandas::click_on_banana_card(int index)
{
	std::vector<int> list;
	std::string reinforcement_position;
	switch (m_banana_cards[index].m_id)
	{
	case 0: // conversion
		m_mouse.use_target();
		m_logic.card_effect.select_enemy_banda = true;
		break;
	case 1: // confiscation
		std::cout << "clicked on confiscation card" << std::endl;
		break;
	case 2: // celerite
		g_msg2server_mtx.lock();
		g_msg2server.emplace("7:2." + std::to_string(index));
		g_msg2server_mtx.unlock();
		break;
	case 3: // charge
		g_msg2server_mtx.lock();
		g_msg2server.emplace("7:3." + std::to_string(index));
		g_msg2server_mtx.unlock();
		break;
	case 4: // desordre
		g_msg2server_mtx.lock();
		g_msg2server.emplace("7:4." + std::to_string(index) + "." + std::to_string(m_enemy.m_team));
		g_msg2server_mtx.unlock();
		remove_card(4);
		break;
	case 5: // entracte
		g_msg2server_mtx.lock();
		g_msg2server.emplace("7:5." + std::to_string(index));
		g_msg2server_mtx.unlock();
		break;
	case 6: // enclume
		m_mouse.use_target();
		m_logic.card_effect.anvil = true;
		break;
	case 7: // petrification
		m_mouse.use_target();
		m_logic.card_effect.petrify = true;
		break;
	case 8: // piege
		std::cout << "clicked on piege card" << std::endl;
		break;
	case 9: // renfort
		list = m_board.get_free_tiles();
		if (list.size() < 2) {
			add_chat_message("Not enough place to spawn up to 3 bandas");
		}
		else {
			reinforcement_position = m_board.get_reinforcement_position(list);
			g_msg2server_mtx.lock();
			g_msg2server.emplace("7:9." + std::to_string(index) + "." + reinforcement_position);
			g_msg2server_mtx.unlock();
		}
		break;
	case 10: // solo
		m_mouse.use_target();
		m_logic.card_effect.select_ally_banda = true;
		break;
	case 11: // vachette
		m_mouse.use_target();
		m_logic.card_effect.cow = true;
		break;
	default:
		break;
	}
}
