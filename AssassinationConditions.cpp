#include "AssassinationConditions.h"
#include "Settings.h"

namespace Conditions {
    bool CanTriggerAssassination(RE::Actor* a_attacker) {
        if (!a_attacker) { return false; }

        if (!a_attacker->IsSneaking()) { return false; }
        if (a_attacker->IsInKillMove()) { return false; }

        auto actorState = a_attacker->AsActorState();
        if (!actorState || actorState->GetWeaponState() != RE::WEAPON_STATE::kDrawn) { return false; }

        RE::TESForm* rightHandForm = a_attacker->GetEquippedObject(false);
        if (!rightHandForm) { return false; }

        auto rightWeapon = rightHandForm->As<RE::TESObjectWEAP>();
        if (!rightWeapon) { return false; }

        if (rightWeapon->GetWeaponType() != RE::WEAPON_TYPE::kOneHandDagger) { return false; }

        auto dataHandler = RE::TESDataHandler::GetSingleton();
        if (!dataHandler) { return false; }

        auto* settings = Settings::GetSingleton();
        if (settings->uRequiredPerkFormID == 0) { return true; }

        if (!cachedPerk) { return false; }

        return a_attacker->HasPerk(cachedPerk);
    }
}