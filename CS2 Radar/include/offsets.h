//This file will contain all offsets needed for reading memory
#pragma once

#include <cstddef>
#include <cstdint>

namespace client_dll {
    constexpr std::ptrdiff_t dwEntityList = 0x271FBB8;
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x2542548;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x256A948;

    constexpr std::ptrdiff_t m_hPlayerPawn = 0x92C;
    constexpr std::ptrdiff_t m_sSanitizedPlayerName = 0x878;
    constexpr std::ptrdiff_t m_iTeamNum = 0x3E7;
    constexpr std::ptrdiff_t m_iHealth = 0x34C;
    constexpr std::ptrdiff_t m_vOldOrigin = 0x14A4;
}

namespace matchmaking_dll {
    constexpr std::ptrdiff_t dwGameTypes = 0x1B0FD0;
    constexpr std::ptrdiff_t dwGameTypes_mapName = 0x120;
}