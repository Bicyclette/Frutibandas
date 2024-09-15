#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "shader_light.hpp"

enum class DRAWING_MODE
{
	SOLID,
	WIREFRAME
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	Vertex()
	{
		position = glm::vec3(0.0f);
		normal = glm::vec3(0.0f);
		texCoords = glm::vec2(0.0f);
	}

	Vertex(
		glm::vec3 pos,
		glm::vec3 norm,
		glm::vec2 tex)
	{
		position = pos;
		normal = norm;
		texCoords = tex;
	}
};

class Mesh
{
	public:

		Mesh(std::vector<Vertex> aVertices, std::vector<int> aIndices, Material m, std::string aName, glm::vec3 center);
        ~Mesh();
		std::string getName();
		std::vector<Vertex> const& getVertices() const;
		std::vector<int> const& getIndices() const;
		Material & getMaterial();
		void bindVAO() const;
		void draw(Shader & s, bool instancing = false, int amount = 1, DRAWING_MODE mode = DRAWING_MODE::SOLID);
		void recreate(std::vector<Vertex> aVertices, std::vector<int> aIndices, bool dynamicDraw);
		void updateVBO(std::vector<Vertex> aVertices, std::vector<int> aIndices);
		bool getVertex(glm::vec3 pos, glm::vec3 normal, glm::vec3 lastPos, Vertex & out);
        glm::vec3 getCenter();
        glm::vec3 getCenterUpdate();
        void setCenterUpdate(glm::vec3 center);

	private:

		GLuint vao;
		GLuint vbo;
		GLuint ebo;

		std::string name;
		std::vector<Vertex> vertices;
		std::vector<int> indices;
        glm::vec3 m_center;
        glm::vec3 m_center_update;
		Material material;

		void shaderProcessing(Shader & s); // set proper uniforms according to shader type
};

#endif
