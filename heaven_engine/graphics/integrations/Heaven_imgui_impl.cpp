#include "Heaven_imgui_impl.h"

// libs
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "heaven_engine/graphics/HeavenDescriptors.h"

// std
#include <filesystem>
#include <stdexcept>

namespace heaven_engine {
    // ok this just initializes imgui using the provided integration files. So in our case we need to
    // initialize the vulkan and glfw imgui implementations, since that's what our engine is built
    // using.
    Heaven_imgui_impl::Heaven_imgui_impl(
        HeavenWindow &window, HeavenVkDevice &device, VkRenderPass renderPass, uint32_t imageCount)
        : hvkDevice{device}, gameWindow(window) {
        // set up a descriptor pool stored on this instance, see header for more comments on this.
        VkDescriptorPoolSize pool_sizes[] = {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
        };

        auto descriptorBuilder = HvnDescriptorPool::Builder(device)
        .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
        .setMaxSets(1000 * IM_ARRAYSIZE(pool_sizes));

        for (VkDescriptorPoolSize poolSize : pool_sizes) {

            descriptorBuilder.addPoolSize(poolSize.type, poolSize.descriptorCount);
        }

        imguiDescriptorPool = descriptorBuilder.build();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        // Initialize imgui for vulkan
        ::ImGui_ImplGlfw_InitForVulkan(window.getWindowPtr(), true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = device.getInstance();
        init_info.PhysicalDevice = device.getPhysicalDevice();
        init_info.Device = device.device();
        init_info.QueueFamily = device.getGraphicsQueueFamily();
        init_info.Queue = device.graphicsQueue();

        // pipeline cache is a potential future optimization, ignoring for now
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = imguiDescriptorPool->getDescriptorPool();
        // todo, I should probably get around to integrating a memory allocator library such as Vulkan
        // memory allocator (VMA) sooner than later. We don't want to have to update adding an allocator
        // in a ton of locations.
        init_info.Allocator = VK_NULL_HANDLE;
        init_info.PipelineInfoMain.RenderPass = renderPass;
        init_info.PipelineInfoMain.Subpass = 0;
        init_info.MinImageCount = 2;
        init_info.ImageCount = imageCount;
        init_info.CheckVkResultFn = check_vk_result;
        ::ImGui_ImplVulkan_Init(&init_info);

        // upload fonts, this is done by recording and submitting a one time use command buffer
        // which can be done easily by using some existing helper functions on the lve device object
        // auto commandBuffer = device.beginSingleTimeCommands();
        // ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        // device.endSingleTimeCommands(commandBuffer);
        // ImGui_ImplVulkan_DestroyFontUploadObjects();

        refreshAssets();
    }

    void Heaven_imgui_impl::refreshAssets() {

        availableAssets.clear();
        std::string path = "assets/models";
        if (!std::filesystem::exists(path)) return;

        for (const auto & entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                auto ext = entry.path().extension().string();
                if (ext == ".obj") {
                    availableAssets.push_back({
                        entry.path().filename().string(),
                        entry.path().string(),
                        ext
                    });
                }
            }
        }
    }

    Heaven_imgui_impl::~Heaven_imgui_impl() {
        ::ImGui_ImplVulkan_Shutdown();
        ::ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Heaven_imgui_impl::newFrame() {
        ::ImGui_ImplVulkan_NewFrame();
        ::ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    // this tells imgui that we're done setting up the current frame,
    // then gets the draw data from imgui and uses it to record to the provided
    // command buffer the necessary draw commands
    void Heaven_imgui_impl::render(VkCommandBuffer commandBuffer) {
        ImGui::Render();
        ImDrawData *drawdata = ImGui::GetDrawData();
        ::ImGui_ImplVulkan_RenderDrawData(drawdata, commandBuffer);
    }

    auto Heaven_imgui_impl::runDebugMenu() -> void {

        ImGui::Begin("Debug Menu");

        ImGui::Text(
                "Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

        ImGui::Separator();
        if (ImGui::Button("Close Game")) {
            ::glfwSetWindowShouldClose(gameWindow.getWindowPtr(), GLFW_TRUE);
        }
        ImGui::Separator();
        ImGui::Checkbox("Model Browser", &show_model_browser);
        ImGui::End();

        if (show_model_browser) {

            runAssetBrowser();
        }
    }
    
    auto Heaven_imgui_impl::runAssetBrowser() -> void {

        if (ImGui::Begin("Asset Browser")) {
            if (ImGui::Button("Refresh")) refreshAssets();
            ImGui::SameLine();
            ImGui::Text("%zu models found", availableAssets.size());

            ImGui::Separator();

            if (ImGui::BeginChild("AssetList")) {
                for (const auto& asset : availableAssets) {
                    // Selectable item for each model
                    if (ImGui::Selectable(asset.name.c_str())) {
                        // Logic for selecting the model could go here
                    }
                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("Path: %s\nType: %s", asset.path.c_str(), asset.extension.c_str());
                    }
                }
                ImGui::EndChild();
            }
        }
        ImGui::End();
    }

    void Heaven_imgui_impl::runExample() {

    }
} // namespace heaven_engine
