#include "GeometryObject.hpp"

GeometryObject::GeometryObject(const Geometry::Geometry3D* geometry) {
	this->geometry = geometry;
    drawingDistance = 1.f;
}

const Geometry::Geometry3D* GeometryObject::Geometry() const {
	return geometry;
}

void GeometryObject::Render(ShaderProgram* shaderProgram, Camera* camera, const glm::vec2& screenSize) const {

}

float GeometryObject::RelativeDrawingDistance() const {
	return drawingDistance;
}

void GeometryObject::SetRelativeDrawingDistance(float drawingDistance) {
	this->drawingDistance = drawingDistance;
}
