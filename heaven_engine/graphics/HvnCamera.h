//
// Created by Admin on 28/12/2025.
//

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#pragma once

namespace heaven_engine {
    class HvnCamera {

    public:

        auto setOrthographicProjection(float left, float right, float bottom, float top, float near, float far) -> void;
        auto setPerspectiveProjection(float fov, float aspectRatio, float near, float far) -> void;

        auto setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f)) -> void;
        auto setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f)) -> void;
        auto setViewYXZ(glm::vec3 position, glm::vec3 rotation) -> void;

        auto getProjectionMatrix() const -> glm::mat4 { return projectionMatrix; };
        auto getViewMatrix() const -> glm::mat4 { return viewMatrix; };

    private:
        glm::mat4 projectionMatrix{1.f};
        glm::mat4 viewMatrix{1.f};
    };
} // heaven_engine