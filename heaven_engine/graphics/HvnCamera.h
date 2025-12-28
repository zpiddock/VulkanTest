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

        auto getProjectionMatrix() const -> glm::mat4 { return projectionMatrix; };

    private:
        glm::mat4 projectionMatrix{1.f};
    };
} // heaven_engine