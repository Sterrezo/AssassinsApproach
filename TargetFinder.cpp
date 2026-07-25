#include "TargetFinder.h"
#include "TargetConditions.h"
#include "Settings.h"
#include <cmath>

namespace TargetFinder {
    RE::Actor* FindClosestTarget(RE::Actor* a_attacker, float a_radius) {
        if (!a_attacker) { return nullptr; }

        RE::Actor* closestActor = nullptr;
        float closestDistance = a_radius;

        const auto processLists = RE::ProcessLists::GetSingleton();
        if (!processLists) { return nullptr; }

        auto attackerPos = a_attacker->GetPosition();
        float zAngle = a_attacker->GetAngleZ();
        float forwardX = std::sin(zAngle);
        float forwardY = std::cos(zAngle);

        for (auto& actorHandle : processLists->highActorHandles) {
            auto actorNiPtr = actorHandle.get();
            RE::Actor* actor = actorNiPtr.get();

            if (!actor) { continue; }
            if (actor == a_attacker) { continue; }
            if (actor->IsDead()) { continue; }

            if (!TargetConditions::IsValidTarget(actor, a_attacker)) { continue; }

            auto targetPos = actor->GetPosition();

            float distance = attackerPos.GetDistance(targetPos);
            if (distance > closestDistance) { continue; }

            float dz = std::fabs(targetPos.z - attackerPos.z);
            if (dz > Settings::GetSingleton()->fAssassinationHeightDifference) {
                continue;
            }

            float dx = targetPos.x - attackerPos.x;
            float dy = targetPos.y - attackerPos.y;

            float dist2D = std::sqrt(dx * dx + dy * dy);

            if (dist2D > 0.0f) {
                float dirX = dx / dist2D;
                float dirY = dy / dist2D;

                float attackerDot = (dirX * forwardX) + (dirY * forwardY);
                if (attackerDot < 0.7f) { continue; }

                float tAngle = actor->GetAngleZ();
                float tForwardX = std::sin(tAngle);
                float tForwardY = std::cos(tAngle);

                float targetDot = (dirX * tForwardX) + (dirY * tForwardY);

                if (targetDot < 0.6f) { continue; }
            }
            closestDistance = distance;
            closestActor = actor;
        }
        return closestActor;
    }
}