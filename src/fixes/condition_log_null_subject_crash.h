#pragma once

namespace Fixes::ConditionLogNullSubjectCrash
{
    // VR's IsInCombat condition callback handles a null/non-actor subject as false, but its
    // diagnostic path still calls the reference display-name helper with that null subject.
    // The helper immediately locks the reference's display-name cache at subject+0x70; its lock
    // object starts at +0x10, so a null subject becomes the observed read from address 0x80 at
    // SkyrimVR+0xC4215F.
    //
    // Preserve condition evaluation and its diagnostic message. Only substitute a stable name
    // when the callback asks the display-name helper to format a null subject. Both result branches
    // in the callback contain the same call and are guarded so a future true/false-path change
    // cannot expose the sibling site.
    namespace detail
    {
        inline constexpr std::array<std::uintptr_t, 2> kDisplayNameCallOffsetsVR{
            0x2ECFE7,
            0x2ECFF5,
        };

        inline constexpr std::uintptr_t kGetDisplayNameOffsetVR = 0x2A78F0;

        inline REL::Relocation<const char*(RE::TESObjectREFR*)> GetDisplayName{
            REL::Offset{ kGetDisplayNameOffsetVR }
        };

        inline const char* GuardedGetDisplayName(RE::TESObjectREFR* a_subject)
        {
            static constexpr char nullSubject[] = "<null>";
            return a_subject ? GetDisplayName(a_subject) : nullSubject;
        }

        inline bool IsExpectedCall(std::uintptr_t a_site, std::uintptr_t a_target)
        {
            if (*reinterpret_cast<const std::uint8_t*>(a_site) != 0xE8)
                return false;

            std::int32_t displacement{};
            std::memcpy(&displacement, reinterpret_cast<const void*>(a_site + 1), sizeof(displacement));
            return a_site + 5 + displacement == a_target;
        }
    }

    inline void Install()
    {
        if (!REL::Module::IsVR())
            return;

        const auto expectedTarget = detail::GetDisplayName.address();
        for (const auto offset : detail::kDisplayNameCallOffsetsVR) {
            const REL::Relocation<std::uintptr_t> site{ REL::Offset{ offset } };
            if (!detail::IsExpectedCall(site.address(), expectedTarget)) {
                logger::warn("IsInCombat null-subject log crash fix: unexpected call at {:X}; skipping fix"sv,
                    site.address());
                return;
            }
        }

        for (const auto offset : detail::kDisplayNameCallOffsetsVR) {
            REL::Relocation<std::uintptr_t>{ REL::Offset{ offset } }.write_call<5>(
                detail::GuardedGetDisplayName);
        }

        logger::info("installed IsInCombat null-subject log crash fix (VR, 2 sites)"sv);
    }
}
