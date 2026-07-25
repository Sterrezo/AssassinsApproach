#include "TargetConditions.h"
#include "Settings.h"

namespace TargetConditions {
    bool IsValidTarget(RE::Actor* a_target, RE::Actor* a_attacker) {
        if (!a_target) { return false; }
        if (!a_attacker) { return false; }
        if (!a_target->Get3D()) { return false; }

        if (cachedActorTypeNPC && !a_target->HasKeyword(cachedActorTypeNPC)) { return false; }

        if (cachedActorTypeGhost && a_target->HasKeyword(cachedActorTypeGhost)) { return false; }
        if (cachedActorTypeUndead && a_target->HasKeyword(cachedActorTypeUndead)) { return false; }

        bool bAllowEssentialTargets = Settings::GetSingleton()->bAllowEssentialTargets;
        if (a_target->IsEssential() && !bAllowEssentialTargets) { return false; }
        bool bAllowProtectedTargets = Settings::GetSingleton()->bAllowProtectedTargets;
        if (a_target->IsProtected() && !bAllowProtectedTargets) { return false; }

        if (a_target->IsPlayerTeammate()) { return false; }
        if (a_target->IsDead()) { return false; }
        if (a_target->IsChild()) { return false; }
        if (a_target->IsGhost()) { return false; }
        if (a_target->IsInKillMove()) { return false; }

        bool bAllowTargetsInCombat = Settings::GetSingleton()->bAllowTargetsInCombat;
        if (a_target->IsInCombat() && !bAllowTargetsInCombat) { return false; }

        auto avOwner = a_target->AsActorValueOwner();
        if (avOwner) {
            if (avOwner->GetActorValue(RE::ActorValue::kInvisibility) > 0.0f) { return false; }
            if (avOwner->GetActorValue(RE::ActorValue::kParalysis) > 0.0f) { return false; }
        }

        // check for races if I end up needing it
        //if (auto race = a_target->GetRace()) {
        //    if (race->GetFormEditorID() == "ArgonianRace"sv) {
        //
        //    }
        //}

        // Detection
        int iMaximumDetectionValue = Settings::GetSingleton()->iMaximumDetectionValue;
        if (a_target->RequestDetectionLevel(a_attacker) > iMaximumDetectionValue) {
            return false;
        }

        // States
        auto targetState = a_target->AsActorState();
        if (targetState) {
            // Flags
            if (targetState->IsBleedingOut() || targetState->IsUnconscious() || targetState->IsSwimming() || targetState->IsFlying()) {
                return false;
            }

            // Ragdoll
            if (targetState->GetKnockState() != RE::KNOCK_STATE_ENUM::kNormal) {
                return false;
            }

            auto sitSleepState = targetState->GetSitSleepState();

            // Sleeping
            bool bAllowSleepingTargets = Settings::GetSingleton()->bAllowSleepingTargets;
            if (!bAllowSleepingTargets) {
                if (sitSleepState == RE::SIT_SLEEP_STATE::kIsSleeping || sitSleepState == RE::SIT_SLEEP_STATE::kWantToSleep || sitSleepState == RE::SIT_SLEEP_STATE::kWaitingForSleepAnim) {
                    return false;
                }
            }

            // Sitting
            bool bAllowSittingTargets = Settings::GetSingleton()->bAllowSittingTargets;
            if (!bAllowSittingTargets) {
                if (sitSleepState == RE::SIT_SLEEP_STATE::kIsSitting || sitSleepState == RE::SIT_SLEEP_STATE::kWantToSit || sitSleepState == RE::SIT_SLEEP_STATE::kWaitingForSitAnim) {
                    return false;
                }
            }
        }

        return true;
    }
}