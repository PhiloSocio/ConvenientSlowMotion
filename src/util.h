#pragma once


#define PI 3.1415926535897932f
#define TWOTHIRDS_PI 2.0943951023931955f
#define TWO_PI 6.2831853071795865f
#define PI2 1.5707963267948966f
#define PI3 1.0471975511965977f
#define PI4 0.7853981633974483f
#define PI8 0.3926990816987242f

#define downVec {0.f, 0.f, -1.f}
#define upVec   {0.f, 0.f, 1.f}

static float* g_deltaTime = (float*)RELOCATION_ID(523660, 410199).address();            //  sensitive to slow time spell
static float* g_deltaTimeRealTime = (float*)RELOCATION_ID(523661, 410200).address();    //  const

using namespace RE;

namespace ManipulateUtil
{
    struct Time
    {
        static inline RE::SpellItem* slowTimeSpell = nullptr;
        static inline RE::BGSPerk* playerMagicTimeSlowdownPerk1 = nullptr;
        static inline RE::BGSPerk* playerMagicTimeSlowdownPerk2 = nullptr;

        static void SetTimeMult(const float a_rate, const float a_duration = -1.f, const bool a_resetToDefault = false) //credits to https://github.com/Pentalimbed
        {
            float defMult = BSTimer::GetSingleton()->QGlobalTimeMultiplier();
            BSTimer::GetSingleton()->SetGlobalTimeMultiplier(a_rate, false);
            if (a_duration > 0.f) {
                std::jthread slowtime_thread([=](){
                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(a_duration * 1000.0f)));
                    BSTimer::GetSingleton()->SetGlobalTimeMultiplier(a_resetToDefault ? defMult : 1.f);
                });
                slowtime_thread.detach();
            }
        }

        static void SetTimeMultTES(const float a_rate, const float a_duration, const bool a_threaded = false)
        {
            if (a_duration > 0.f && slowTimeSpell && slowTimeSpell->effects[0]) {
                if (auto player = RE::PlayerCharacter::GetSingleton(); player) {
                    slowTimeSpell->effects[0]->effectItem.duration = a_duration < 1.f ? 1.f : a_duration;
                    slowTimeSpell->effects[0]->effectItem.magnitude = a_rate;
                //    VATS::GetSingleton()->SetMagicTimeSlowdown(a_rate, a_rate);
                    if (a_threaded) {
                        bool hasPlayerMagicTimeSlowdownPerk1 = playerMagicTimeSlowdownPerk1 && player->HasPerk(playerMagicTimeSlowdownPerk1);
                        if (hasPlayerMagicTimeSlowdownPerk1) player->RemovePerk(playerMagicTimeSlowdownPerk1);
                        bool hasPlayerMagicTimeSlowdownPerk2 = playerMagicTimeSlowdownPerk2 && player->HasPerk(playerMagicTimeSlowdownPerk2);
                        if (hasPlayerMagicTimeSlowdownPerk2) player->RemovePerk(playerMagicTimeSlowdownPerk2);
                        player->AddSpell(slowTimeSpell);
                        std::jthread slowtime_thread([=](){
                            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(a_duration * 1000.0f)));
                            player->RemoveSpell(slowTimeSpell);
                            if (hasPlayerMagicTimeSlowdownPerk1) player->AddPerk(playerMagicTimeSlowdownPerk1);
                            if (hasPlayerMagicTimeSlowdownPerk2) player->AddPerk(playerMagicTimeSlowdownPerk2);
                        });
                        slowtime_thread.detach();
                    } else {
                        bool hasPlayerMagicTimeSlowdownPerk1 = playerMagicTimeSlowdownPerk1 && player->HasPerk(playerMagicTimeSlowdownPerk1);
                        if (hasPlayerMagicTimeSlowdownPerk1) player->RemovePerk(playerMagicTimeSlowdownPerk1);
                        bool hasPlayerMagicTimeSlowdownPerk2 = playerMagicTimeSlowdownPerk2 && player->HasPerk(playerMagicTimeSlowdownPerk2);
                        if (hasPlayerMagicTimeSlowdownPerk2) player->RemovePerk(playerMagicTimeSlowdownPerk2);
                        std::jthread slowtime_thread([=](){
                            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(a_duration * 1000.0f)));
                            if (hasPlayerMagicTimeSlowdownPerk1) player->AddPerk(playerMagicTimeSlowdownPerk1);
                            if (hasPlayerMagicTimeSlowdownPerk2) player->AddPerk(playerMagicTimeSlowdownPerk2);
                        });
                        slowtime_thread.detach();
                        player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(slowTimeSpell, false, nullptr, 1.f, false, a_rate, player);
                    }
                }
            } else if (auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
                playerMagicTimeSlowdownPerk1 = dataHandler->LookupForm<RE::BGSPerk>(0x804, "Slow Time Edit.esp");
                playerMagicTimeSlowdownPerk2 = dataHandler->LookupForm<RE::BGSPerk>(0x807, "RealmShifting.esp");
                slowTimeSpell = dataHandler->LookupForm<RE::SpellItem>(0x801, "ConvenientSlowMotion.esp");
            //    slowTimeSpell = dataHandler->LookupForm<RE::SpellItem>(0x800, "Slow Time Edit.esp");
            //    slowTimeSpell = dataHandler->LookupForm<RE::SpellItem>(0x48ad2, "Skyrim.esm");
                if (slowTimeSpell) SetTimeMult(a_rate, a_duration);
                else SetTimeMultImpl(a_rate, a_duration, true);
            }
        }

        static void SetTimeMultImpl(const float a_rate, const float a_duration = -1.f, const bool a_resetToDefault = false) //credits to https://github.com/Pentalimbed
        {
            REL::Relocation<float*> time_mult_1{REL::VariantID(511883, 388443, 0x1EC569C)}; //{REL::VariantID(511882, 388442, 0x1EC5698)};
        //    REL::Relocation<float*> time_mult_2{REL::VariantID(511883, 388443, 0x1EC569C)};
            *time_mult_1 *= a_rate;
        //    *time_mult_2 = *time_mult_1;

            if (a_duration > 0.f) {
                std::jthread slowtime_thread([=](){
                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(a_duration * 1000.0f)));
                    *time_mult_1 /= a_rate;
                    if (*time_mult_1 > 1.f || a_resetToDefault)
                        *time_mult_1 = 1.f;
                //    *time_mult_2 = *time_mult_1;
                });
                slowtime_thread.detach();
            }
        }

        class BSTimer
        {
        public:
            [[nodiscard]] static BSTimer* GetSingleton() noexcept
            {
                REL::Relocation<BSTimer*> singleton{ RELOCATION_ID(523657, 410196) };
                return singleton.get();
            }

            static float QGlobalTimeMultiplier()
            {
                REL::Relocation<float*> value{ RELOCATION_ID(511882, 388442) };
                return *value;
            }

            static float QGlobalTimeMultiplierTarget()
            {
                REL::Relocation<float*> value{ RELOCATION_ID(511883, 388443) };
                return *value;
            }

            void SetGlobalTimeMultiplier(float a_multiplier, bool a_now = true)
            {
                using func_t = decltype(&BSTimer::SetGlobalTimeMultiplier);
                REL::Relocation<func_t> func{ RELOCATION_ID(66988, 68245) };
                return func(this, a_multiplier, a_now);
            }

            // members
            std::uint64_t unk00;                          // 00
            std::uint32_t lastPerformanceCount;           // 08
            float         clamp;                          // 10
            float         clampRemainder;                 // 14
            float         delta;                          // 18
            float         realTimeDelta;                  // 1C
            std::uint32_t unk20;                          // 20
            std::uint32_t unk24;                          // 24
            float         unk28;                          // 28
            std::uint32_t unk2C;                          // 2C
            std::uint32_t unk30;                          // 30
            std::uint32_t unk34;                          // 34
            std::uint8_t  unk38;                          // 38
            std::uint8_t  unk39;                          // 39
            bool          useGlobalTimeMultiplierTarget;  // 3A
            std::uint8_t  pad3B;                          // 3B
            std::uint32_t pad3C;                          // 3C
        };
        static_assert(sizeof(BSTimer) == 0x40);
    };
}

namespace Util
{
    constexpr uint32_t hash(const char* data, size_t const size) noexcept
    {
        uint32_t hash = 5381;

        for (const char* c = data; c < data + size; ++c) {
            hash = ((hash << 5) + hash) + (unsigned char)*c;
        }

        return hash;
    }

    constexpr uint32_t operator""_h(const char* str, size_t size) noexcept
    {
        return hash(str, size);
    }

}