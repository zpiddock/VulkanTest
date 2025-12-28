//
// Created by Admin on 23/12/2025.
//

#include "GameProgram.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <heaven_engine/GameObject.h>
#include <heaven_engine/graphics/SimpleRenderSystem.h>

#include <string>

#include "graphics/integrations/Heaven_imgui_impl.h"

namespace heaven_engine {

    // Cube Model
    std::unique_ptr<BasicModel> createCubeModel(HeavenVkDevice &device, glm::vec3 offset) {
        std::vector<BasicModel::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

        };
        for (auto &v: vertices) {
            v.position += offset;
        }
        return std::make_unique<BasicModel>(device, vertices);
    }

    bool imguiShouldRender = false;

    GameProgram::GameProgram() {

        loadObjects();
    }

    GameProgram::~GameProgram() {

    }

    auto GameProgram::loadObjects() -> void {

        std::shared_ptr model = createCubeModel(vulkanDevice, {0.f, 0.f, 0.f});

        auto cube = GameObject::createGameObject();
        cube.model = model;
        cube.transform.translation = {0.f, 0.f, 0.5f};
        cube.transform.scale = {0.5f, 0.5f, 0.5f};
        gameObjects.push_back(std::move(cube));
    }

    auto GameProgram::run() -> void {

        auto imgui = Heaven_imgui_impl(gameWindow, vulkanDevice, renderer.getSwapChainRenderPass(), renderer.getImageCount());

        SimpleRenderSystem simpleRenderSystem{vulkanDevice, renderer.getSwapChainRenderPass()};

        double lastTime = ::glfwGetTime();
        float frameTimer = 0.0f;
        int frameCount = 0;

        ::glfwSetKeyCallback(gameWindow.getWindowPtr(), [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                imguiShouldRender = !imguiShouldRender;
            }
        });

        //Gameloop
        while(!gameWindow.shouldClose()) {
            ::glfwPollEvents();

            double currentTime = ::glfwGetTime();
            float deltaTime = static_cast<float>(currentTime - lastTime);
            lastTime = currentTime;

            // Update FPS in the window title every second
            frameTimer += deltaTime;
            frameCount++;
            if (frameTimer >= 1.0f) {
                std::string fpsTitle = "TestGame - FPS: " + std::to_string(frameCount);
                ::glfwSetWindowTitle(gameWindow.getWindowPtr(), fpsTitle.c_str());
                frameTimer = 0.0f;
                frameCount = 0;
            }

            // Optional: Limit deltaTime to avoid "teleporting" if the app hitches
            deltaTime = glm::min(deltaTime, 0.1f);

            if (auto commandBuffer = renderer.beginFrame()) {

                if (imguiShouldRender) {
                    // start imgui rendering
                    imgui.newFrame();
                }


                renderer.beginSwapChainRenderPass(commandBuffer);

                // render game objects first, so they will be rendered in the background. This
                // is the best we can do for now.
                // Once we cover offscreen rendering, we can render the scene to a image/texture rather than
                // directly to the swap chain. This texture of the scene can then be rendered to an imgui
                // subwindow
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);

                if (imguiShouldRender) {
                    // example code telling imgui what windows to render, and their contents
                    // this can be replaced with whatever code/classes you set up configuring your
                    // desired engine UI
                    imgui.runExample();

                    // as last step in render pass, record the imgui draw commands
                    imgui.render(commandBuffer);
                }

                renderer.endSwapChainRenderPass(commandBuffer);

                renderer.endFrame();
            }
        }

        ::vkDeviceWaitIdle(vulkanDevice.device());
    }
} // heaven_engine