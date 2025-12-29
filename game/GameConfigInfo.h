//
// Created by Admin on 29/12/2025.
//

#pragma once

namespace heaven_engine {

    class GameConfigInfo {

    public:
        GameConfigInfo() = default;
        ~GameConfigInfo() = default;

        GameConfigInfo(const GameConfigInfo&) = delete;
        GameConfigInfo& operator=(const GameConfigInfo&) = delete;

        bool imguiShouldOpen = false;
    };
} // heaven_engine