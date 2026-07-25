#pragma once

#include <string>

inline RE::TESIdleForm* cachedKillmove = nullptr;
inline RE::BGSKeyword* cachedActorTypeNPC = nullptr;
inline RE::BGSKeyword* cachedActorTypeGhost = nullptr;
inline RE::BGSKeyword* cachedActorTypeUndead = nullptr;
inline RE::BGSPerk* cachedPerk = nullptr;

class Settings {
public:
    static Settings* GetSingleton();

    void Load();

    RE::FormID uRequiredPerkFormID = 0;
    std::string sRequiredPerkPlugin = "";
    float fAssassinationDistance = 180.0f;
    float fAssassinationHeightDifference = 40.0f;
    int iMaximumDetectionValue = -10;
    bool bAllowSittingTargets = true;
    bool bAllowSleepingTargets = false;
    bool bAllowEssentialTargets = false;
    bool bAllowProtectedTargets = true;

private:
    Settings() = default;
    Settings(const Settings&) = default;
    Settings& operator=(const Settings&) = delete;
};