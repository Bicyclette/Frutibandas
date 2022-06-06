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
	m_enemy.m_avatar.cleanup();
}

void Bandas::createUI()
{
	// home page
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
	h_layer1.get_sprite(1)->set_pos(glm::vec2(400, 728 - 140));
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
	h_layer13.get_sprite(64)->set_background_img("assets/home_page/pseudo.tga");
	h_layer13.get_sprite(64)->set_background_img_selected("assets/home_page/pseudo_hover.tga");
	h_layer13.get_sprite(64)->use_background_img();
	h_layer13.add_sprite(65, glm::vec2(525 - 75, 728 - (140 + 48 * 8 - 24)), glm::vec2(150, 24), c_screen_width, c_screen_height);
	h_layer13.get_sprite(65)->set_background_img("assets/home_page/connexion.tga");
	h_layer13.get_sprite(65)->set_background_img_selected("assets/home_page/connexion_hover.tga");
	h_layer13.get_sprite(65)->use_background_img();

	Layer& h_layer14 = home_page.get_layer(14);
	h_layer14.add_sprite(66, glm::vec2(91, 728 - 432-70), glm::vec2(99, 70), c_screen_width, c_screen_height);
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

	m_ui.set_active_page(0);
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
}

void Bandas::click_home_page(Page& page, int id)
{
	if (id != 64)
	{
		m_writer.m_cursor.m_focus = 2;
		page.get_layer(13).get_sprite(64)->use_background_img();
	}

	if (id == 2) // face
	{
		page.get_layer(2).get_sprite(id)->select();
		page.get_layer(2).get_sprite(3)->unselect();
		page.get_layer(2).get_sprite(4)->unselect();
		page.get_layer(2).get_sprite(5)->unselect();
		page.get_layer(2).get_sprite(6)->unselect();
		// highlight
		page.get_layer(1).get_sprite(1)->set_pos(glm::vec2(400, 728 - (140 + 48 * (id - 2))));
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
		page.get_layer(1).get_sprite(1)->set_pos(glm::vec2(400, 728 - (140 + 48 * (id - 2))));
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
		page.get_layer(1).get_sprite(1)->set_pos(glm::vec2(400, 728 - (140 + 48 * (id - 2))));
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
		page.get_layer(1).get_sprite(1)->set_pos(glm::vec2(400, 728 - (140 + 48 * (id - 2))));
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
		page.get_layer(1).get_sprite(1)->set_pos(glm::vec2(400, 728 - (140 + 48 * (id - 2))));
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
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}

	if (id == 64) // pseudo input
	{
		m_writer.m_cursor.m_focus = 0;
		page.get_layer(13).get_sprite(64)->use_background_img_selected();
	}

	if (id == 65) // clicked on connect
	{
		g_msg2server_mtx.lock();
		g_msg2server.emplace("0");
		g_msg2server_mtx.unlock();
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
	m_me.m_avatar.draw(false);

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
			}
			else
			{
				home_page.get_layer(15).set_visibility(true);
				home_page.get_layer(16).set_visibility(false);
			}
		}
		else
		{
			home_page.get_layer(14).get_sprite(66)->use_background_img();
			home_page.get_layer(13).set_visibility(true);
			home_page.get_layer(15).set_visibility(false);
			home_page.get_layer(16).set_visibility(false);
			// draw pseudo input and cursor
			m_text.print(m_writer.m_textInput[0], 525 - 72, 272, 1, glm::vec3(0));
			if (m_writer.m_cursor.m_focus == 0)
			{
				glm::vec3 cursor_shape = m_text.get_cursor_shape(m_writer.m_textInput[0], 525 - 72, 272, 1, m_writer.m_cursor.m_pos);
				m_writer.m_cursor.draw(cursor_shape, delta);
			}
		}
	}
}

void Bandas::update_game_page(std::bitset<10> user_input, std::string txt_input, float delta)
{

}

void Bandas::draw_game_page(float delta)
{
	m_graphics.userInterfaceFBO->bind();
	glViewport(0, 0, c_screen_width, c_screen_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_ui.get_page(1).draw();
}