#pragma once

class PlayerAttackSink : public RE::BSTEventSink<RE::BSAnimationGraphEvent> {
public:
    static PlayerAttackSink* GetSingleton();
    RE::BSEventNotifyControl ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>*)
override;

private:
    PlayerAttackSink() = default;
    PlayerAttackSink(const PlayerAttackSink&) = delete;
    PlayerAttackSink& operator=(const PlayerAttackSink&) = delete;
};