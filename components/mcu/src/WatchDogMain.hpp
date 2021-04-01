// MWDT registers are part of the timer submodule.
#ifndef H_WatchDogMain
#define H_WatchDogMain

#include <stdint.h>
#include "soc/timer_group_reg.h"
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/WordRW.hpp"
#include "bits/WordWO.hpp"
// xtensa/config/specreg.h
#undef INTERRUPT

class WatchDogMain
{
public:
    class Stage
    {
    private:
        /** Action on expired timeout */
        SubValueRW* const action;
    public:
        /** Stage timeout value, in MWDT clock cycles */
        WordRW* const timeout;
        const uint8_t id;
    public:
        Stage( const uint32_t timeout_r, const uint32_t action_r, 
            const uint32_t action_m, const uint8_t action_s, const uint8_t id );
        virtual ~Stage();
    public:
        enum class Action : uint32_t
        {
            OFF = 0,
            INTERRUPT = 1,
            RESET_CPU = 2,
            RESET_SYSTEM = 3,
        };
        void setAction( Action a ) { action->set( static_cast<uint32_t>( a ) ); };
        Action getAction() const { return (Action) action->get(); }; //TODO test it!
    };
    Stage* const stage0;
    Stage* const stage1;
    Stage* const stage2;
    Stage* const stage3;
    /** When set, a level type interrupt will occur at the timeout of a stage configured to generate an interrupt */
    FlagRW* interruptLevel;
    /** When set, an edge type interrupt will occur at the timeout of a stage configured to generate an interrupt */
    FlagRW* interruptEdge;
    /** When set, MWDT is enabled */
    FlagRW* enable;
    /** When set, flash boot protection is enabled */
    FlagRW* protectFlashBoot;
    /** When set, WDT is enabled to reset CPU #0 (PRO) */
    FlagRW* resetCPU0;
//  /** When set, WDT is enabled to reset CPU #1 (APP) */
//  FlagRW* resetCPU1;
    /** MWDT clock prescaler value (factor to clock period) */
    SubValueRW* const scale;
    /** Write any value to feed the MWDT (reset counting to the beginning) */
    WordWO* const feed;
    /** Reset signal duration */
    enum class Duration : uint32_t
    {
        T100NS  = TIMG_WDT_RESET_LENGTH_100_NS,
        T200NS  = TIMG_WDT_RESET_LENGTH_200_NS,
        T300NS  = TIMG_WDT_RESET_LENGTH_300_NS,
        T400NS  = TIMG_WDT_RESET_LENGTH_400_NS,
        T500NS  = TIMG_WDT_RESET_LENGTH_500_NS,
        T800NS  = TIMG_WDT_RESET_LENGTH_800_NS,
        T1600NS = TIMG_WDT_RESET_LENGTH_1600_NS,
        T3200NS = TIMG_WDT_RESET_LENGTH_3200_NS,
    };
    enum class Target : bool
    {
        CPU,
        SYSTEM
    };
private:
    /** CPU reset signal length selection */
    SubValueRW* const durationResetCPU;
    /** System reset signal length selection */
    SubValueRW* const durationResetSystem;
    /** If the register contains a different value than its reset value, write protection is enabled */
    WordWO* const lock;
public:
    //TODO move out here
    /** Clear RTC WDT interrupt state */
    FlagWO* const interruptClear;
    /** RTC WDT interrupt state */
    FlagRO* const interruptState;
    /** RTC WDT interrupt raw */
    FlagRO* const interruptRaw;
    /** enable/disable RTC WDT interrupt */
    FlagRW* const interruptEnable;
public:
    WatchDogMain( const uint8_t tg );
    virtual ~WatchDogMain();
public:
    /**
     * @param t reset target.
     * @return reset signal duration for specified target.
     */
    Duration getResetDuration( Target t ) const;
    /**
     * @param t reset target.
     * @param d reset signal duration for specified target.
     */
    void setResetDuration( Target t, Duration d );
    /**
     * Controls whether watchdoc parameters can be changed.
     * @param p true: disable mutations; false: enable mutations
     */
    void protect( const bool p );
};

#endif
