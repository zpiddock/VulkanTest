//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <graphics/ShaderPipeline.h>

#include "GameWindow.h"

namespace vulkangame {
    class GameProgram {

    public:

        auto run() -> void;

    private:
        GameWindow gameWindow;
        VulkanDevice vulkanDevice{gameWindow};
        ShaderPipeline simple_shader{
            vulkanDevice,
            "simple_shader",
            ShaderPipeline::defaultPipelineConfigInfo(800, 600)
        };
    };
}
