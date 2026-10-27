//This file will contain all offsets needed for reading memory
#pragma once

#include <cstddef>
#include <cstdint>

namespace client_dll {
    constexpr std::ptrdiff_t dwEntityList = 0x271C8B8;
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x253EEF8;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x25676D8;
}

namespace matchmaking_dll {
    constexpr std::ptrdiff_t dwGameTypes = 0x1B0FD0;
    constexpr std::ptrdiff_t dwGameTypes_mapName = 0x120;
}