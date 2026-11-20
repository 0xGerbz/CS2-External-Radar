//This file will contain all offsets needed for reading memory
#pragma once

#include <cstddef>
#include <cstdint>

namespace client_dll {
    constexpr std::ptrdiff_t dwEntityList = 0x270D5B0;
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x252F300;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x2557AB8;

    constexpr std::ptrdiff_t m_hPlayerPawn = 0x92C;
    constexpr std::ptrdiff_t m_sSanitizedPlayerName = 0x878;
    constexpr std::ptrdiff_t m_iTeamNum = 0x3E7;
    constexpr std::ptrdiff_t m_iHealth = 0x34C;
    constexpr std::ptrdiff_t m_vOldOrigin = 0x14A4;
    constexpr std::ptrdiff_t m_iCompTeammateColor = 0x858;
    constexpr std::ptrdiff_t m_angEyeAngles = 0x3630;
}

namespace matchmaking_dll {
    constexpr std::ptrdiff_t dwGameTypes = 0x1AEFC0;
    constexpr std::ptrdiff_t dwGameTypes_mapName = 0x120;
}