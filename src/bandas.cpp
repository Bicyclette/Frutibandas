#include "bandas.hpp"

Bandas::Bandas(Graphics& graphics) :
	m_graphics(graphics),
    m_text(c_screen_width, c_screen_height),
    m_mouse(glm::ivec2(0, 0), glm::ivec2(25, 25), "assets/mouse/normal.tga", "assets/mouse/hover.tga", "assets/mouse/viseur.tga", c_screen_width, c_screen_height),
	m_writer(c_screen_width, c_screen_height)
{
    m_mouse.activate();
	// user interface
	createUI();

    // load some fonts and set an active font
    m_text.load_police("assets/fonts/ebrima.ttf", 20);
    m_text.load_police("assets/fonts/ebrima.ttf", 15);
    m_text.use_police(0);
}

Bandas::~Bandas()
{

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
	home_page.add_layer(12); // choix de couleur
	//home_page.add_layer(13); // user inputs (pseudo, connexion, chercher un adversaire)
	//home_page.add_layer(14); // chercher adversaire
	//home_page.add_layer(15); // stop chercher adversaire

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
/*
	Layer& h_layer12 = home_page.get_layer(12);
	h_layer12.add_sprite(33, glm::vec2(525 - 75, 728 - (140 + 48 * 7 - 12)), glm::vec2(150, 30), c_screen_width, c_screen_height);
	h_layer12.get_sprite(33)->set_background_img("assets/pseudo.tga");
	h_layer12.get_sprite(33)->set_background_img_selected("assets/pseudo_hover.tga");
	h_layer12.get_sprite(33)->use_background_img();
	h_layer12.add_sprite(34, glm::vec2(525 - 75, 728 - (140 + 48 * 8 - 24)), glm::vec2(150, 24), c_screen_width, c_screen_height);
	h_layer12.get_sprite(34)->set_background_img("assets/connexion.tga");
	h_layer12.get_sprite(34)->set_background_img_selected("assets/connexion_hover.tga");
	h_layer12.get_sprite(34)->use_background_img();

	Layer& h_layer13 = home_page.get_layer(13);
	h_layer13.add_sprite(35, glm::vec2(525 - 75, 246), glm::vec2(150, 24), c_screen_width, c_screen_height);
	h_layer13.get_sprite(35)->set_background_img("assets/jouer.tga");
	h_layer13.get_sprite(35)->set_background_img_selected("assets/jouer_hover.tga");
	h_layer13.get_sprite(35)->use_background_img();

	Layer& h_layer14 = home_page.get_layer(14);
	h_layer14.add_sprite(36, glm::vec2(525 - 40, 240), glm::vec2(80, 24), c_screen_width, c_screen_height);
	h_layer14.get_sprite(36)->set_background_img("assets/stop_search.tga");
	h_layer14.get_sprite(36)->set_background_img_selected("assets/stop_search_hover.tga");
	h_layer14.get_sprite(36)->use_background_img();
	*/

	m_ui.set_active_page(0);
}

void Bandas::swap_gender_features(Avatar::GENDER from, Avatar::GENDER to)
{
	
}

void Bandas::update_home_page(std::bitset<10>& user_input, char* txt_input, float delta)
{
	glm::ivec2 mouse_pos = m_mouse.get_position();
	glm::ivec2 mouse_size = m_mouse.get_size();
	std::shared_ptr<Sprite> hovered = m_ui.get_hovered_sprite(mouse_pos.x, mouse_pos.y);

	if (!hovered) { return; }
	Page& home_page{ m_ui.get_page(0) };
	int sprite_id{ hovered->get_id() };

	hovering_home_page(home_page, sprite_id);
	if (user_input.test(2) && user_input.test(9)) // if click event
	{
		click_home_page(home_page, sprite_id);
		update_avatar(home_page, sprite_id);
	}
	/*
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
			Layer& layer{ home_page.get_layer(hovered->get_layer_id()) };
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
				else if (i >= 18 && i <= 20) // layer 6
					home_page.get_layer(6).get_sprite(i)->use_background_img();
				else if (i >= 21 && i <= 22) // layer 7
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
				else if (i >= 18 && i <= 20) // layer 6
					home_page.get_layer(6).get_sprite(i)->use_background_img();
				else if (i >= 21 && i <= 22) // layer 7
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
		if (sprite_id == 34) // hovered connexion button
		{
			home_page.get_layer(12).get_sprite(sprite_id)->use_background_img_selected();
		}
		else
		{
			home_page.get_layer(12).get_sprite(34)->use_background_img();
		}
		if (sprite_id == 35) // hovered play button
		{
			home_page.get_layer(13).get_sprite(sprite_id)->use_background_img_selected();
		}
		else
		{
			home_page.get_layer(13).get_sprite(35)->use_background_img();
		}
		if (sprite_id == 36) // hovered stop search button
		{
			home_page.get_layer(14).get_sprite(sprite_id)->use_background_img_selected();
		}
		else
		{
			home_page.get_layer(14).get_sprite(36)->use_background_img();
		}
		if (interupt && m_popup_button.mouse_hover(mouse_pos[0], mouse_pos[1]))
		{
			m_popup_button.set_background_img_gl(m_popup_tex[8].id);
		}
		else
		{
			m_popup_button.set_background_img_gl(m_popup_tex[7].id);
		}
		if (interupt && m_popup_button.mouse_hover(mouse_pos[0], mouse_pos[1]) && inputs.test(2) && inputs.test(9)) // clicked on OK for popup
		{
			g_interupt_mutex.lock();
			m_interupt = false;
			g_interupt_mutex.unlock();
		}
		else if (sprite_id == 31 && inputs.test(2) && inputs.test(9)) // clicked on quit game
		{
			SDL_Event event;
			event.type = SDL_QUIT;
			SDL_PushEvent(&event);
		}
		else if (!interupt)
		{
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
				home_page.get_layer(12).get_sprite(34)->use_background_img();
				m_writer.m_cursor.m_focus = 2; // 0 = pseudo, 1 = chat, 2 = not writting

				// show options
				if (sprite_id == 2)
				{
					if (home_page.get_layer(3).m_visible)
						home_page.get_layer(3).set_visibility(false);
					else
						home_page.get_layer(3).set_visibility(true);
					for (int i{ 4 }; i < 10; ++i)
						home_page.get_layer(i).set_visibility(false);
				}
				else if (sprite_id == 3)
				{
					if (home_page.get_layer(9).get_sprite(26)->is_selected())
					{
						if (home_page.get_layer(4).m_visible)
							home_page.get_layer(4).set_visibility(false);
						else
							home_page.get_layer(4).set_visibility(true);
						for (int i{ 3 }; i < 10; ++i)
							if (i != 4)
								home_page.get_layer(i).set_visibility(false);
					}
					else if (home_page.get_layer(9).get_sprite(27)->is_selected())
					{
						if (home_page.get_layer(5).m_visible)
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
					if (home_page.get_layer(8).m_visible)
						home_page.get_layer(8).set_visibility(false);
					else
						home_page.get_layer(8).set_visibility(true);
					for (int i{ 3 }; i < 10; ++i)
						if (i != 8)
							home_page.get_layer(i).set_visibility(false);
				}
				else if (sprite_id == 6)
				{
					if (home_page.get_layer(9).m_visible)
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
				else if (sprite_id >= 18 && sprite_id <= 20)
				{
					for (int i{ 18 }; i <= 20; ++i)
					{
						if (i == sprite_id)
							continue;
						home_page.get_layer(6).get_sprite(i)->unselect();
					}
				}
				else if (sprite_id >= 21 && sprite_id <= 22)
				{
					for (int i{ 21 }; i <= 22; ++i)
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
						else if (sprite_id == 20)
							m_avatar.m_eyes = Avatar::EYES::GROS;

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
						if (sprite_id == 21)
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
							m_avatar.m_skin_color_id = m_avatar.m_skin_color.size() - 1;
						}
					}
					else
					{
						m_avatar.m_skin_color_id++;
						if (m_avatar.m_skin_color_id >= m_avatar.m_skin_color.size())
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
							m_avatar.m_hair_color_id = m_avatar.m_hair_color.size() - 1;
						}
					}
					else
					{
						m_avatar.m_hair_color_id++;
						if (m_avatar.m_hair_color_id >= m_avatar.m_hair_color.size())
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
							m_avatar.m_eyes_color_id = m_avatar.m_eyes_color.size() - 1;
						}
					}
					else
					{
						m_avatar.m_eyes_color_id++;
						if (m_avatar.m_eyes_color_id >= m_avatar.m_eyes_color.size())
						{
							m_avatar.m_eyes_color_id = 0;
						}
					}
				}

				// stop focus pseudo input
				home_page.get_layer(12).get_sprite(33)->use_background_img();
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
			else if (sprite_id == 33 && inputs.test(2) && inputs.test(9)) // clicked on pseudo
			{
				home_page.get_layer(12).get_sprite(sprite_id)->use_background_img_selected();
				m_writer.m_cursor.m_focus = 0; // 0 = pseudo, 1 = chat, 2 = not writting
			}
			else if (sprite_id == 34 && inputs.test(2) && inputs.test(9)) // clicked on connect
			{
				g_try_connection = true;
				std::string data("0:" + m_writer.m_textInput[0] + ":");
				// gender
				if (m_avatar.m_gender == Avatar::GENDER::MALE) {
					data += "0.";
					// hair
					switch (m_avatar.m_hair)
					{
					case Avatar::HAIR::MIXTE:
						data += "0.";
						break;
					case Avatar::HAIR::HERISSON:
						data += "1.";
						break;
					case Avatar::HAIR::DECOIFFE:
						data += "2.";
						break;
					case Avatar::HAIR::ARRIERE:
						data += "3.";
						break;
					case Avatar::HAIR::MECHE_AVANT:
						data += "4.";
						break;
					default:
						break;
					};
					// eyes
					switch (m_avatar.m_eyes)
					{
					case Avatar::EYES::MANGA:
						data += "0.";
						break;
					case Avatar::EYES::AMANDE:
						data += "1.";
						break;
					case Avatar::EYES::GROS:
						data += "2.";
						break;
					default:
						break;
					};
				}
				else {
					data += "1.";
					// hair
					switch (m_avatar.m_hair)
					{
					case Avatar::HAIR::MIXTE:
						data += "0.";
						break;
					case Avatar::HAIR::MI_LONG:
						data += "5.";
						break;
					case Avatar::HAIR::FRANGE:
						data += "6.";
						break;
					case Avatar::HAIR::AU_BOL:
						data += "7.";
						break;
					case Avatar::HAIR::PONYTAIL:
						data += "8.";
						break;
					default:
						break;
					};
					// eyes
					switch (m_avatar.m_eyes)
					{
					case Avatar::EYES::MANGA:
						data += "0.";
						break;
					case Avatar::EYES::EGYPTE:
						data += "3.";
						break;
					case Avatar::EYES::MASCARA:
						data += "4.";
						break;
					default:
						break;
					};
				}
				// mouth
				switch (m_avatar.m_mouth)
				{
				case Avatar::MOUTH::PETITE:
					data += "0.";
					break;
				case Avatar::MOUTH::MOYENNE:
					data += "1.";
					break;
				case Avatar::MOUTH::GRANDE:
					data += "2.";
					break;
				};
				// skin color
				data += std::to_string(m_avatar.m_skin_color_id) + ".";
				// hair color
				data += std::to_string(m_avatar.m_hair_color_id) + ".";
				// eyes color
				data += std::to_string(m_avatar.m_eyes_color_id);

				g_msg2server_mutex.lock();
				g_msg2server_queue.emplace(data);
				g_msg2server_mutex.unlock();

				// stop focus pseudo input
				home_page.get_layer(12).get_sprite(33)->use_background_img();
				m_writer.m_cursor.m_focus = 2; // 0 = pseudo, 1 = chat, 2 = not writting
			}
			else if (sprite_id == 35 && inputs.test(2) && inputs.test(9)) // clicked on play
			{
				g_search_opponent = true;
				g_msg2server_mutex.lock();
				g_msg2server_queue.emplace("1");
				g_msg2server_mutex.unlock();
			}
			else if (sprite_id == 36 && inputs.test(2) && inputs.test(9)) // clicked on stop search opponent
			{
				g_search_opponent = false;
				g_msg2server_mutex.lock();
				g_msg2server_queue.emplace("2");
				g_msg2server_mutex.unlock();
			}
			else if (inputs.test(2) && inputs.test(9))
			{
				// stop focus pseudo input
				home_page.get_layer(12).get_sprite(33)->use_background_img();
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
		}
		if (m_writer.m_cursor.m_focus == 0)
		{
			int boundX = home_page.get_layer(12).get_sprite(33)->get_position().x + home_page.get_layer(12).get_sprite(33)->get_size().x;
			glm::vec3 cursor_shape = textRenderer->get_cursor_shape(m_writer.m_textInput[0], 525 - 72, 272, 1, m_writer.m_cursor.m_pos);
			m_writer.write(text_input, inputs, delta, boundX, cursor_shape);
		}
	}
	*/
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
}

void Bandas::click_home_page(Page& page, int id)
{
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

void Bandas::draw_home_page()
{
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
}

void Bandas::update_game_page(std::bitset<10>& user_input, char* txt_input, float delta)
{

}

void Bandas::draw_game_page()
{
	m_graphics.userInterfaceFBO->bind();
	glViewport(0, 0, c_screen_width, c_screen_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_ui.get_page(1).draw();
}