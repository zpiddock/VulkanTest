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

    struct PointLightComponent {
        float lightIntensity = 1.f;
    };
    class GameObject {

    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, GameObject>;

        static auto createGameObject() -> GameObject {
            static id_t currentId = 0;
            return GameObject{currentId++};
        };

        static auto createPointLight(float intensity = 10.f, float radius = 0.05f, glm::vec3 colour = glm::vec3(1.f)) -> GameObject;

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject& operator=(GameObject&&) = default;

        auto getId() -> id_t const { return id; }

        glm::vec3 colour{};

        TransformComponent transform{};

        std::shared_ptr<BasicModel> model{};
        std::unique_ptr<PointLightComponent> pointLight = nullptr;

    private:

        GameObject(id_t objId) : id(objId) {}

        id_t id;
    };
} // heaven_engine