#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> aVertices, std::vector<int> aIndices, Material m, std::string aName, glm::vec3 center) :
	name(aName),
	vertices(aVertices),
	indices(aIndices),
	material(m),
    m_center(center),
    m_center_update(center)
{
	// VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// EBO
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	// Unbind VAO
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ebo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	for(int i{0}; i < material.textures.size(); ++i)
	{
		glDeleteTextures(1, &material.textures[i].id);
	}
}

std::string Mesh::getName()
{
	return name;
}

std::vector<Vertex> const& Mesh::getVertices() const
{
	return vertices;
}

std::vector<int> const& Mesh::getIndices() const
{
	return indices;
}

Material & Mesh::getMaterial()
{
	return material;
}

glm::vec3 Mesh::getCenter()
{
    return m_center;
}

glm::vec3 Mesh::getCenterUpdate()
{
    return m_center_update;
}

void Mesh::setCenterUpdate(glm::vec3 center)
{
    m_center_update = center;
}

void Mesh::bindVAO() const
{
	glBindVertexArray(vao);
}

void Mesh::shaderProcessing(Shader & s)
{
	if(s.getType() == SHADER_TYPE::PBR)
	{
		s.setVec3f("material.albedo", material.color_diffuse);
		s.setVec3f("material.emissiveColor", material.color_emissive);
		s.setFloat("material.emissionIntensity", material.emission_intensity);
		s.setFloat("material.metallic", material.metallic);
		s.setFloat("material.roughness", material.roughness);
		s.setFloat("material.ao", 1.0f);
		s.setFloat("material.opacity", material.opacity);
		s.setInt("material.hasNormal", 0);
		s.setInt("material.hasAlbedo", 0);
		s.setInt("material.hasMetallicRough", 0);
		s.setInt("material.hasEmission", 0);
		s.setInt("material.nbTextures", material.textures.size());
	}

	for(int i{0}; i < material.textures.size(); ++i)
	{
		if(material.textures[i].type == TEXTURE_TYPE::DIFFUSE)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, material.textures[i].id);
			if(s.getType() == SHADER_TYPE::BLINN_PHONG)
			{
				s.setInt("material.diffuse", i);
				s.setInt("material.hasDiffuse", 1);
			}
			else if(s.getType() == SHADER_TYPE::PBR)
			{
				s.setInt("material.albedoMap", i);
				s.setInt("material.hasAlbedo", 1);
			}
			else if(s.getType() == SHADER_TYPE::SHADOWS)
			{
				s.setInt("diffuse", i);
				s.setInt("hasDiffuse", 1);
			}
		}
		else if(material.textures[i].type == TEXTURE_TYPE::SPECULAR)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, material.textures[i].id);
			if(s.getType() == SHADER_TYPE::BLINN_PHONG)
			{
				s.setInt("material.specular", i);
				s.setInt("material.hasSpecular", 1);
			}
		}
		else if(material.textures[i].type == TEXTURE_TYPE::NORMAL)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, material.textures[i].id);
			if(s.getType() == SHADER_TYPE::BLINN_PHONG)
				s.setInt("material.normal", i);
			else if(s.getType() == SHADER_TYPE::PBR)
				s.setInt("material.normalMap", i);
			s.setInt("material.hasNormal", 1);
		}
		else if(material.textures[i].type == TEXTURE_TYPE::METALLIC_ROUGHNESS)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, material.textures[i].id);
			if(s.getType() == SHADER_TYPE::PBR)
			{
				s.setInt("material.metallicRoughMap", i);
				s.setInt("material.hasMetallicRough", 1);
			}
		}
		else if(material.textures[i].type == TEXTURE_TYPE::EMISSIVE)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, material.textures[i].id);
			s.setInt("material.emissionMap", i);
			s.setInt("material.hasEmission", 1);
		}
	}
}

void Mesh::draw(Shader& s, bool instancing, int amount, DRAWING_MODE mode)
{
	// bind vao
	glBindVertexArray(vao);

	// use shader and sets its uniforms
	s.use();
	shaderProcessing(s);

	// draw solid or wireframe
	if(mode == DRAWING_MODE::SOLID)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if(mode == DRAWING_MODE::WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0f);
	}

	// draw
	if(instancing)
	{
		s.setInt("instancing", 1);
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, amount);
	}
	else
	{
		s.setInt("instancing", 0);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}

	// unbind vao
	glBindVertexArray(0);

	// reset draw mode to default solid and active texture
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::recreate(std::vector<Vertex> aVertices, std::vector<int> aIndices, bool dynamicDraw)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ebo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	// VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if(dynamicDraw)
		glBufferData(GL_ARRAY_BUFFER, aVertices.size() * sizeof(Vertex), aVertices.data(), GL_DYNAMIC_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, aVertices.size() * sizeof(Vertex), aVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// EBO
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	if(dynamicDraw)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, aIndices.size() * sizeof(int), aIndices.data(), GL_DYNAMIC_DRAW);
	else
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, aIndices.size() * sizeof(int), aIndices.data(), GL_STATIC_DRAW);

	// Unbind VAO
	glBindVertexArray(0);
}

void Mesh::updateVBO(std::vector<Vertex> aVertices, std::vector<int> aIndices)
{
	vertices.clear();
	vertices = aVertices;
	indices.clear();
	indices = aIndices;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(int), indices.data());
}

bool Mesh::getVertex(glm::vec3 pos, glm::vec3 normal, glm::vec3 lastPos, Vertex & out)
{
	for(int i{0}; i < vertices.size(); ++i)
	{
		Vertex v{vertices[i]};
		if(v.position.x == lastPos.x && v.position.y == lastPos.y && v.position.z == lastPos.z)
		{
			out = v;
			out.position = pos;
			out.normal = normal;
			return true;
		}
	}
	return false;
}
