#include "event.h"
#include "util.h"
using namespace Util;
/**/
bool AnimationEventTracker::Register()
{
    const auto pc = PlayerCharacter::GetSingleton();

    bool bSinked = false;
    bool bSuccess = pc->AddAnimationGraphEventSink(AnimationEventTracker::GetSingleton());
    if (bSuccess) {
        spdlog::info("Registered {}", typeid(BSAnimationGraphEvent).name());
    } else {
        BSAnimationGraphManagerPtr graphManager;
        pc->GetAnimationGraphManager(graphManager);
        if (graphManager) {
            for (auto& animationGraph : graphManager->graphs) {
                if (bSinked) {
                    break;
                }
                auto eventSource = animationGraph->GetEventSource<BSAnimationGraphEvent>();
                for (auto& sink : eventSource->sinks) {
                    if (sink == AnimationEventTracker::GetSingleton()) {
                        bSinked = true;
                        break;
                    }
                }
            }
        }

        if (!bSinked) {
            spdlog::info("Failed to register {}", typeid(BSAnimationGraphEvent).name());
        }
    }
    return bSuccess || bSinked;
}

EventChecker AnimationEventTracker::ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
{
    if (a_event) {
        std::string eventTag = a_event->tag.data();
        switch (hash(eventTag.data(), eventTag.size())) {
        case ("SlowMotionStart"_h):
            if (a_event->holder && a_event->holder->IsPlayerRef()) {
                float ratio = 0.2f;
                float duration = 0.3f;
                a_event->holder->GetGraphVariableFloat("SlowMotionRatio", ratio);
                a_event->holder->GetGraphVariableFloat("SlowMotionResetTimer", duration);
            //    ManipulateUtil::Time::SetTimeMult(ratio, duration, duration < 1.f);
                ManipulateUtil::Time::SetTimeMult(ratio, duration, false);
                spdlog::debug("Slow motion started for {} seconds with {} time rate.", duration, ratio);
            }
            break;
        }
    }
        return EventChecker::kContinue;
}