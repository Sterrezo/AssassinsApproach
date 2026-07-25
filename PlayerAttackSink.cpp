#include "PlayerAttackSink.h"
#include "AssassinationConditions.h"
#include "TargetFinder.h"
#include "AssassinationKillmove.h"
#include "Settings.h"

namespace logger = SKSE::log;

PlayerAttackSink* PlayerAttackSink::GetSingleton() {
    static PlayerAttackSink singleton;
    return &singleton;
}

RE::BSEventNotifyControl PlayerAttackSink::ProcessEvent(const RE::BSAnimationGraphEvent* a_event,
    RE::BSTEventSource<RE::BSAnimationGraphEvent>*) {
    if (!a_event || !a_event->holder) {
        return RE::BSEventNotifyControl::kContinue;
    }

    if (a_event->holder->IsPlayerRef()) {
        if (a_event->tag == "PowerAttack_Start_end" || a_event->tag == "InitiateWinBegin" || a_event->tag == "preHitframe") {
            RE::Actor* attacker = const_cast<RE::Actor*>(a_event->holder->As<RE::Actor>());

            if (Conditions::CanTriggerAssassination(attacker)) {
                float assassinationDistance = Settings::GetSingleton()->fAssassinationDistance;
                RE::Actor* target = TargetFinder::FindClosestTarget(attacker, assassinationDistance);

                if (target) {
                    AssassinationKillmove::PlayKillmove(attacker, target, cachedKillmove);
                }
            }
        }
    }
    return RE::BSEventNotifyControl::kContinue;
}