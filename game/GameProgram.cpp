//
// Created by Admin on 23/12/2025.
//

#include "GameProgram.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <../heaven_engine/GameObject.h>
#include <../heaven_engine/graphics/SimpleRenderSystem.h>

#include <glm/gtc/constants.hpp>

namespace heaven_engine {

    GameProgram::GameProgram() {

        loadObjects();
    }

    GameProgram::~GameProgram() {

    }

    auto GameProgram::loadObjects() -> void {

        std::vector<BasicModel::Vertex> vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        };

        auto model = std::make_shared<BasicModel>(vulkanDevice, vertices);

        auto triangle = GameObject::createGameObject();
        triangle.model = model;
        triangle.colour = {0.1f, 0.8f, 0.1f};
        triangle.transform2d.translation.x = 0.2f;
        triangle.transform2d.scale = {2.f, 0.5f};
        triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }

    auto GameProgram::run() -> void {

        SimpleRenderSystem simpleRenderSystem{vulkanDevice, renderer.getSwapChainRenderPass()};

        //Gameloop
        while(!gameWindow.shouldClose()) {
            ::glfwPollEvents();
            if (auto commandBuffer = renderer.beginFrame()) {

                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        ::vkDeviceWaitIdle(vulkanDevice.device());
    }
} // heaven_engine