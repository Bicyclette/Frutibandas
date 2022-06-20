#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <glm/glm.hpp>
#include "audio.hpp"
#include "shader_light.hpp"
#include "user_interface.hpp"

constexpr int c_screen_width{ 1050 };
constexpr int c_screen_height{ 728 };

struct Music
{
	Music() :
		volume(0.125f),
		shader("shaders/sound/vertex.glsl", "shaders/sound/fragment.glsl"),
		sound_lvl(6, glm::vec2(1050 - 120 - 91 - 5 + 41, 40), glm::vec2(7, 100), c_screen_width, c_screen_height),
		sound_ctrl(7, glm::vec2(1050 - 120 - 91 - 5 + 34, 35), glm::vec2(20, 20), c_screen_width, c_screen_height),
		visible(false),
		grab_ctrl(false),
		range{ 35, 125 }
	{
		sound_lvl.set_background_img("assets/game_page/sound_lvl.tga");
		sound_lvl.use_background_img();
		sound_ctrl.set_background_img("assets/game_page/sound_ctrl.tga");
		sound_ctrl.use_background_img();
		sound_ctrl.set_pos(glm::vec2(1050 - 120 - 91 - 5 + 34, 35 + volume * (range[1] - range[0])));
	}

	bool grab(glm::ivec2 mouse)
	{
		int posY = c_screen_height - mouse.y;
		if (visible)
		{
			if (mouse.x >= sound_ctrl.get_position().x && mouse.x <= (sound_ctrl.get_position().x + sound_ctrl.get_size().x))
			{
				if (posY >= sound_ctrl.get_position().y && posY <= (sound_ctrl.get_position().y + sound_ctrl.get_size().y))
				{
					grab_ctrl = true;
					return true;
				}
			}
			return (grab_ctrl || false);
		}
		return false;
	}

	void tweak_sound_lvl(std::array<int, 3> mouse_delta)
	{
		int posY = sound_ctrl.get_position().y - mouse_delta[1];
		posY = (posY < range[0]) ? range[0] : posY;
		posY = (posY > range[1]) ? range[1] : posY;
		sound_ctrl.set_pos(glm::vec2(sound_ctrl.get_position().x, posY));
		// set volume
		volume = (posY - 35) / static_cast<float>(range[1] - range[0]);
	}

	void draw()
	{
		if (visible)
		{
			// sound level
			glBindVertexArray(sound_lvl.get_vao());
			shader.use();
			shader.setMatrix("proj", sound_lvl.get_projection_matrix());
			shader.setBool("use_bkg_img", true);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, sound_lvl.get_background_img_id());
			shader.setInt("image", 0);
			shader.setFloat("sound_lvl", volume);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			
			// sound controller
			sound_ctrl.draw();
		}
	}

	float volume; // from 0 to 1
	Shader shader;
	Sprite sound_lvl;
	Sprite sound_ctrl;
	bool visible;
	bool grab_ctrl;
	std::array<int, 2> range;
};

#endif