#include "GeometryObject.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"

GeometryObject::GeometryObject(const Geometry::Geometry3D* geometry) {
	this->geometry = geometry;
    drawingDistance = 1.f;
}

const Geometry::Geometry3D* GeometryObject::Geometry() const {
	return geometry;
}

void GeometryObject::Render(Camera* camera, const glm::vec2& screenSize) const {
}

float GeometryObject::RelativeDrawingDistance() const {
	return drawingDistance;
}

void GeometryObject::SetRelativeDrawingDistance(float drawingDistance) {
	this->drawingDistance = drawingDistance;
}
