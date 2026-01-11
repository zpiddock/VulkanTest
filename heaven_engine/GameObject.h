//
// Created by Admin on 27/12/2025.
//

#pragma once

#include "graphics/models/BasicModel.h"
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace heaven_engine {

    struct TransformComponent {
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix

        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    class GameObject {

    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, GameObject>;

        static auto createGameObject() -> GameObject {
            static id_t currentId = 0;
            return GameObject{currentId++};
        };

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject& operator=(GameObject&&) = default;

        auto getId() -> id_t const { return id; }

        std::shared_ptr<BasicModel> model;
        glm::vec3 colour;
        TransformComponent transform{};

    private:

        GameObject(id_t objId) : id(objId) {}

        id_t id;
    };
} // heaven_engine