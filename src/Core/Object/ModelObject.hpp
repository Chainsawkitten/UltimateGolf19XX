#pragma once

#include "GeometryObject.hpp"
#include "../Geometry/Model.hpp"


class ModelObject : public GeometryObject{
	ModelObject(const Geometry::Model* Model, std::string diffusePath, std::string normalPath, std::string specularPath);

	/// Render the object.
	/**
	* @param shaderProgram %Shader program to render the object with.
	* @param camera %Camera to render through.
	* @param screenSize Size of the screen in pixels.
	*/
	virtual void Render(Camera* camera, const glm::vec2& screenSize) const;
private:
	Shader* vertexShader;
	Shader* fragmentShader;
	Shader* geometryShader;
	const Geometry::Geometry3D* geometry;
	ShaderProgram* shaderProgram;
	Texture2D* diffuse;
	Texture2D* normal;
	Texture2D* specular;
};