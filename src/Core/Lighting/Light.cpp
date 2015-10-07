#include "Light.hpp"

Light::Light() {
    position = glm::vec4(0.f, 3.f, 3.f, 1.f);
    intensity = glm::vec3(1.f, 1.f, 1.f);
    diffuseCoefficient = glm::vec3(1.f, 1.f, 1.f);
}
