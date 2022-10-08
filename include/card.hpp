#ifndef CARD_HPP
#define CARD_HPP

#include "user_interface.hpp"

constexpr glm::vec2 c_card_size_140(91, 121);
constexpr glm::vec2 c_card_size_160(101, 132);

constexpr glm::vec2 c_orange_card_pos[2][2]{
	{ glm::vec2(16, 728 - 190 - 121), glm::vec2(119, 728 - 190 - 121) },
	{ glm::vec2(16, 728 - 315 - 121), glm::vec2(0) }
};

constexpr glm::vec2 c_banana_card_pos[2][2]{
	{ glm::vec2(840, 728 - 190 - 121), glm::vec2(943, 728 - 190 - 121) },
	{ glm::vec2(840, 728 - 315 - 121), glm::vec2(0) }
};

inline const std::string c_card_texture[14] = {
	"assets/cartes/140/conversion.tga",
	"assets/cartes/140/confiscation.tga",
	"assets/cartes/140/celerite.tga",
	"assets/cartes/140/charge.tga",
	"assets/cartes/140/desordre.tga",
	"assets/cartes/140/entracte.tga",
	"assets/cartes/140/enclume.tga",
	"assets/cartes/140/petrification.tga",
	"assets/cartes/140/piege.tga",
	"assets/cartes/140/renfort.tga",
	"assets/cartes/140/solo.tga",
	"assets/cartes/140/vachette.tga",
	"assets/cartes/140/verso.tga",
	"assets/cartes/140/used.tga"
};

inline const std::string c_card_desc_texture[12] = {
	"assets/cartes/conversion_desc.tga",
	"assets/cartes/confiscation_desc.tga",
	"assets/cartes/celerite_desc.tga",
	"assets/cartes/charge_desc.tga",
	"assets/cartes/desordre_desc.tga",
	"assets/cartes/entracte_desc.tga",
	"assets/cartes/enclume_desc.tga",
	"assets/cartes/petrification_desc.tga",
	"assets/cartes/piege_desc.tga",
	"assets/cartes/renfort_desc.tga",
	"assets/cartes/solo_desc.tga",
	"assets/cartes/vachette_desc.tga"
};

inline const std::string c_advertiser_texture[24] = {
	"assets/advertiser/conversion_g.tga",
	"assets/advertiser/conversion_r.tga",
	"assets/advertiser/confiscation_g.tga",
	"assets/advertiser/confiscation_r.tga",
	"assets/advertiser/celerite_g.tga",
	"assets/advertiser/celerite_r.tga",
	"assets/advertiser/charge_g.tga",
	"assets/advertiser/charge_r.tga",
	"assets/advertiser/desordre_g.tga",
	"assets/advertiser/desordre_r.tga",
	"assets/advertiser/entracte_g.tga",
	"assets/advertiser/entracte_r.tga",
	"assets/advertiser/enclume_g.tga",
	"assets/advertiser/enclume_r.tga",
	"assets/advertiser/petrification_g.tga",
	"assets/advertiser/petrification_r.tga",
	"assets/advertiser/piege_g.tga",
	"assets/advertiser/piege_r.tga",
	"assets/advertiser/renfort_g.tga",
	"assets/advertiser/renfort_r.tga",
	"assets/advertiser/solo_g.tga",
	"assets/advertiser/solo_r.tga",
	"assets/advertiser/vachette_g.tga",
	"assets/advertiser/vachette_r.tga"
};

struct Card
{
	int m_id;
	Card() : m_id(-1)
	{}
};

#endif