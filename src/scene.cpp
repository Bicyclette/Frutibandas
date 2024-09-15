#include "scene.hpp"

Scene::Scene(std::string pName, int aId) :
	name(pName),
	ID(aId),
	activeCamera(-1)
{}

int Scene::getId()
{
	return ID;
}

void Scene::addCamera(CAM_TYPE type, glm::ivec2 scrDim, glm::vec3 pos, glm::vec3 target, glm::vec3 up, float fov, float near, float far)
{
	cameras.emplace_back(type, scrDim, pos, target, up, fov, near, far);
}

void Scene::setActiveCamera(int index)
{
	activeCamera = index;
}

void Scene::updateCameraPerspective(glm::ivec2 scrDim)
{
	for(int i{0}; i < cameras.size(); ++i)
	{
		Camera& cam = cameras[i];
		cam.setProjection(scrDim, cam.getNearPlane(), cam.getFarPlane());
	}
}

std::vector<Camera>& Scene::getCameras()
{
	return cameras;
}

Camera& Scene::getActiveCamera()
{
	return cameras[activeCamera];
}

std::string & Scene::getName()
{
	return name;
}

void Scene::addAudioFile(std::string file)
{
	audio.load_sound(file);
}

void Scene::addSoundSource(glm::vec3 position, glm::vec3 direction, float inner_angle, float outer_angle, float volume, bool loop)
{
	sound_source.emplace_back(position, direction, inner_angle, outer_angle, volume, loop);
}

void Scene::playSound(int source_index, int audio_index)
{
	sound_source[source_index].play_sound(audio.sounds[audio_index]);
}

void Scene::stopSound(int source_index, int audio_index)
{
	sound_source[source_index].stop_sound();
}

Source& Scene::getSoundSource(int index)
{
	return sound_source[index];
}
