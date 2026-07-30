#include "PlayerAttackSink.h"
#include "Settings.h"
#include <spdlog/sinks/basic_file_sink.h>

namespace logger = SKSE::log;

// mrowrpurr said to copy paste this
void SetupLog() {
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
    spdlog::set_default_logger(std::move(loggerPtr));
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);
}

class MenuHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
public:
    RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override {
        if (a_event && !a_event->opening && a_event->menuName == RE::RaceSexMenu::MENU_NAME) {
            if (auto player = RE::PlayerCharacter::GetSingleton()) {
                player->AddAnimationGraphEventSink(PlayerAttackSink::GetSingleton());
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
    static MenuHandler* GetSingleton() {
        static MenuHandler singleton;
        return &singleton;
    }
};

class EquipHandler : public RE::BSTEventSink<RE::TESEquipEvent> {
public:
    RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>*) override {
        if (a_event && a_event->actor && a_event->actor->IsPlayerRef()) {
            if (auto player = RE::PlayerCharacter::GetSingleton()) {
                player->AddAnimationGraphEventSink(PlayerAttackSink::GetSingleton());
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
    static EquipHandler* GetSingleton() {
        static EquipHandler singleton;
        return &singleton;
    }
};

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);

    SetupLog();
    Settings::GetSingleton()->Load();

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message *message) {
        if (message->type == SKSE::MessagingInterface::kDataLoaded) {
            cachedKillmove = RE::TESForm::LookupByEditorID<RE::TESIdleForm>("KillMoveSneak1HMThroatSlit00");
            cachedActorTypeNPC = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("ActorTypeNPC");
            cachedActorTypeGhost = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("ActorTypeGhost");
            cachedActorTypeUndead = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("ActorTypeUndead");

            auto settings = Settings::GetSingleton();
            if (settings->uRequiredPerkFormID != 0) {
                cachedPerk = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSPerk>(settings->uRequiredPerkFormID, settings->sRequiredPerkPlugin);
                logger::info("Successfully cached a configured perk");
            } else {
                logger::info("Perk missing, leaving it as nullptr");
            }

            RE::UI::GetSingleton()->AddEventSink(MenuHandler::GetSingleton());
            if (auto equipSource = RE::ScriptEventSourceHolder::GetSingleton()) {
                equipSource->AddEventSink(EquipHandler::GetSingleton());
            }
        }

        if ((message->type == SKSE::MessagingInterface::kPostLoadGame) || (message->type == SKSE::MessagingInterface::kNewGame)) {
            if (!cachedKillmove || !cachedActorTypeNPC || !cachedActorTypeGhost || !cachedActorTypeUndead) {
                logger::error("Cache missing, skipping registering the attack sink");
                return;
            }

            if (auto player = RE::PlayerCharacter::GetSingleton()) {
                player->AddAnimationGraphEventSink(PlayerAttackSink::GetSingleton());
                logger::info("Player attack event sink registered");
            }
        }
    });

    return true;
}