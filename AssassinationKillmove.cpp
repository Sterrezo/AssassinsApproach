#include "AssassinationKillmove.h"

namespace logger = SKSE::log;

namespace AssassinationKillmove {
    namespace {
        bool PlayPairedIdle(RE::AIProcess* a_proc, RE::Actor* a_attacker, RE::DEFAULT_OBJECT a_actionIdle, RE::TESIdleForm* a_idle, bool a_arg5, bool a_arg6, RE::TESObjectREFR* a_target) {
            using func_t = decltype(&PlayPairedIdle);
            REL::Relocation<func_t> func{RELOCATION_ID(38290, 39256)};
            return func(a_proc, a_attacker, a_actionIdle, a_idle, a_arg5, a_arg6, a_target);
        }
    }

    bool PlayKillmove(RE::Actor* a_attacker, RE::Actor* a_target, RE::TESIdleForm* a_idle) {
        if (!a_attacker || !a_target || !a_idle) {
            return false;
        }

        auto* currentProcess = a_attacker->GetActorRuntimeData().currentProcess;
        if (!currentProcess) {
            return false;
        }

        if (a_target->GetOccupiedFurniture()) {
            a_target->StopInteractingQuick(true);
            a_target->EvaluatePackage(true, true);
        }

        bool result = PlayPairedIdle(currentProcess, a_attacker, RE::DEFAULT_OBJECT::kActionIdle, a_idle, true, false, a_target);

        return result;
    }
}