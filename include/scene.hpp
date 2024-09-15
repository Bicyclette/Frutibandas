#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <cstdlib>
#include "camera.hpp"
#include "color.hpp"
#include "graphics.hpp"
#include "audio.hpp"

enum class DRAW_TYPE
{
    DRAW_OPAQUE,
    DRAW_TRANSPARENT,
    DRAW_BOTH
};

class Scene
{
	public:

		Scene(std::string pName, int aId);
		int getId();
		void addCamera(CAM_TYPE type, glm::ivec2 scrDim, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 target = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float fov = 45.0f, float near = 0.1f, float far = 100.0f);
		void setActiveCamera(int index);
		void updateCameraPerspective(glm::ivec2 scrDim);
		std::vector<Camera>& getCameras();
		Camera& getActiveCamera();
		std::string & getName();

		// sound management
		void addAudioFile(std::string file);
		void addSoundSource(glm::vec3 position, glm::vec3 direction = glm::vec3(0.0f), float inner_angle = 0.0f, float outer_angle = 0.0f, float volume = 1.0f, bool loop = false);
		void playSound(int source_index, int audio_index);
		void stopSound(int source_index, int audio_index);
		Source& getSoundSource(int index);

    public:
        static inline glm::vec3 sortCamPos;

	private:

		int ID;
		std::string name;
		int activeCamera;
		std::vector<Camera> cameras;
		Audio audio; // collection of audio files
		std::vector<Source> sound_source; // collection of sound emitters
};

#endif
