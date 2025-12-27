//
// Created by Admin on 27/12/2025.
//

#pragma once

#include "graphics/models/BasicModel.h"
#include <memory>

#include <glm/glm.hpp>

namespace heaven_engine {

    struct Transform2dComponent {
        glm::vec2 translation{};
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2() {
            const float sin = glm::sin(rotation);
            const float cos = glm::cos(rotation);
            glm::mat2 rotationMat{{cos, sin}, {-sin, cos}};
            glm::mat2 scaleMat{{scale.x, 0.f}, {0.f, scale.y}};
            return rotationMat * scaleMat;
        }
    };

    class GameObject {

    public:
        using id_t = unsigned int;

        static auto createGameObject() -> GameObject {
            static id_t currentId = 0;
            return GameObject(currentId++);
        };

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject& operator=(GameObject&&) = default;

        auto getId() -> id_t const { return id; }

        std::shared_ptr<BasicModel> model;
        glm::vec3 colour;
        Transform2dComponent transform2d;

    private:

        GameObject(id_t objId) : id(objId) {}

        id_t id;
    };
} // heaven_engine