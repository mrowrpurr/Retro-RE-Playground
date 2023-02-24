#pragma once

#include <string>

#include "RE/GetAddress.h"
#include "RE/Player.h"

namespace Fallout1 {

    class Player {
        Player()                         = default;
        ~Player()                        = default;
        Player(const Player&)            = delete;
        Player(Player&&)                 = delete;
        Player& operator=(const Player&) = delete;
        Player& operator=(Player&&)      = delete;

    public:
        static Player& GetSingleton() {
            static Player singleton;
            return singleton;
        }

        int  GetAge() { return *RE::GetAddress<int*>(RE::Addresses::PlayerAge); }
        void SetAge(int age) { *RE::GetAddress<int*>(RE::Addresses::PlayerAge) = age; }

        const char* GetName() { return RE::GetAddress<const char*>(RE::Addresses::PlayerName); }
        void        SetName(const char* name) {
            char* playerName = (char*)Fallout1::RE::GetAddress(Fallout1::RE::Addresses::PlayerName);
            strcpy(playerName, (char*)name);
        }
    };

    Player& GetPlayer() { return Player::GetSingleton(); }
}
