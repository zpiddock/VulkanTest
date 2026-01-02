//
// Created by Admin on 23/12/2025.
//

#include "GameProgram.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <heaven_engine/GameObject.h>
#include <heaven_engine/graphics/models/BasicModel.h>
#include <heaven_engine/graphics/SimpleRenderSystem.h>

#include <string>

#include <chrono>

#include "GameConfigInfo.h"
#include "graphics/integrations/Heaven_imgui_impl.h"
#include "input/Movement_Controller.h"

namespace heaven_engine {
    struct GlobalUbo {
        glm::mat4 projectionView{1.f};
        glm::vec4 ambientLightColour{1.f, 1.f, 1.f, .02f};
        glm::vec4 lightPosition{-1.f};
        glm::vec4 lightColor{1.f};
    };

    GameProgram::GameProgram() {
        globalDescriptorPool = HvnDescriptorPool::Builder(vulkanDevice)
                .setMaxSets(HeavenVkSwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, HeavenVkSwapChain::MAX_FRAMES_IN_FLIGHT)
                .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
                .build();

        loadObjects();
    }

    GameProgram::~GameProgram() {
    }

    auto GameProgram::loadObjects() -> void {
        std::shared_ptr model = BasicModel::createFromFile(vulkanDevice, "assets/models/smooth_vase.obj");
        std::shared_ptr model2 = BasicModel::createFromFile(vulkanDevice, "assets/models/flat_vase.obj");
        std::shared_ptr quad_model = BasicModel::createFromFile(vulkanDevice, "assets/models/quad.obj");

        auto vase = GameObject::createGameObject();
        vase.model = model;
        vase.transform.translation = {0.f, 0.f, 0.f};
        vase.transform.scale = {0.5f, 0.5f, 0.5f};
        gameObjects.push_back(std::move(vase));

        auto flat_vase = GameObject::createGameObject();
        flat_vase.model = model2;
        flat_vase.transform.translation = {-1.f, 0.f, 0.f};
        flat_vase.transform.scale = {0.5f, 0.5f, 0.5f};
        gameObjects.push_back(std::move(flat_vase));

        auto floor = GameObject::createGameObject();
        floor.model = quad_model;
        floor.transform.translation = {0.f, 0.5f, 0.f};
        floor.transform.scale = {3.f, 1.f, 3.f};
        gameObjects.push_back(std::move(floor));
    }

    auto GameProgram::run() -> void {
        HeavenVkBuffer globalUboBuffer{
            vulkanDevice,
            sizeof(GlobalUbo),
            HeavenVkSwapChain::MAX_FRAMES_IN_FLIGHT,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            vulkanDevice.properties.limits.minUniformBufferOffsetAlignment
        };

        globalUboBuffer.map();

        auto globalSetLayout = HvnDescriptorSetLayout::Builder(vulkanDevice)
        .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
        .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(HeavenVkSwapChain::MAX_FRAMES_IN_FLIGHT);

        for (auto & descriptorSet : globalDescriptorSets) {

            auto bufferInfo = globalUboBuffer.descriptorInfo();
            HvnDescriptorWriter(*globalSetLayout, *globalDescriptorPool)
            .writeBuffer(0, &bufferInfo)
            .build(descriptorSet);
        }
        GameConfigInfo gameConfigInfo{};

        auto imgui = Heaven_imgui_impl(gameWindow, vulkanDevice, renderer.getSwapChainRenderPass(),
                                       renderer.getImageCount());

        SimpleRenderSystem simpleRenderSystem{vulkanDevice, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};

        HvnCamera camera{};

        auto viewerObject = GameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;

        InputManager inputManager{gameWindow};

        Movement_Controller cameraController{gameConfigInfo};
        cameraController.setTargetObject(viewerObject);
        inputManager.addSubscriber(&cameraController);

        ::glfwSetInputMode(gameWindow.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (::glfwRawMouseMotionSupported())
            ::glfwSetInputMode(gameWindow.getWindowPtr(), GLFW_RAW_MOUSE_MOTION,
                               GLFW_TRUE);

        auto currentTime = std::chrono::high_resolution_clock::now();

        //Gameloop
        while (!gameWindow.shouldClose()) {
            ::glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float>(newTime - currentTime).count();
            currentTime = newTime;

            if (!gameConfigInfo.imguiShouldOpen) {
                cameraController.moveInPlaneXZ(gameWindow.getWindowPtr(), frameTime, viewerObject);
                camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
            }

            float aspect = renderer.getAspectRatio();
            // camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProjection(glm::radians(60.0f), aspect, 0.1f, 10.0f);

            if (auto commandBuffer = renderer.beginFrame()) {
                int frameIndex = renderer.getCurrentFrameIndex();

                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex]
                };

                // update
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjectionMatrix() * camera.getViewMatrix();
                globalUboBuffer.writeToIndex(&ubo, frameIndex);
                globalUboBuffer.flushIndex(frameIndex);

                // Do rendering
                if (gameConfigInfo.imguiShouldOpen) {
                    // start imgui rendering
                    imgui.newFrame();
                }

                renderer.beginSwapChainRenderPass(commandBuffer);

                // render game objects first, so they will be rendered in the background. This
                // is the best we can do for now.
                // Once we cover offscreen rendering, we can render the scene to a image/texture rather than
                // directly to the swap chain. This texture of the scene can then be rendered to an imgui
                // subwindow
                simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);

                if (gameConfigInfo.imguiShouldOpen) {
                    // example code telling imgui what windows to render, and their contents
                    // this can be replaced with whatever code/classes you set up configuring your
                    // desired engine UI
                    imgui.runDebugMenu();

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
