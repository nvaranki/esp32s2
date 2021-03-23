// RTC_CNTL

// Power management unit of the MCU.
//
// © 2021 Nikolai Varankine

#ifndef H_PowerManagementUnit
#define H_PowerManagementUnit

#include "SleepAndWakeupController.hpp"
#include "bits/FlagRO.hpp"
#include "bits/SubValueRO.hpp"

class PowerManagementUnit
{
private:
    SleepAndWakeupController* swc;
public:
    SubValueRO* const status; //!< rtc main state machine status TODO undocumented
    FlagRO* const idle; //!< rtc main state machine is in idle state TODO undocumented
    FlagRO* const sleep; //!< rtc main state machine is in sleep state TODO undocumented
    FlagRO* const waitXTAL; //!< rtc main state machine is in wait xtal state TODO undocumented
    FlagRO* const waitPLL; //!< rtc main state machine is in wait pll state TODO undocumented
    FlagRO* const waitO8M; //!< rtc main state machine is in wait 8m (clock?) state TODO undocumented
    FlagRO* const standby; //!< rtc main state machine is in the states of low power TODO undocumented
    FlagRO* const wakeup; //!< rtc main state machine is in the states of wakeup process TODO undocumented
    FlagRO* const wait; //!< rtc main state machine has been waited for some cycles TODO undocumented
    FlagRO* const ready; //!< rtc main state machine is ready to receive wake up trigger from wake up source
    FlagRO* const pll; //!< rtc main state machine is in states that pll should be running TODO undocumented
    FlagRO* const xtalISO; //!< rtc main state machine: no use any more TODO undocumented
    /**
     * When found true, indicates a state of ULP FSM/RISCV when it finished a program. Usually this is an
     * inverse value of "cocpuRuns" property.
     * TODO undocumented
     */
    FlagRO* const cocpuDone; //!< ulp/cocpu is done TODO undocumented
    /**
     * When found true, indicates a sleep state of ULP FSM/RISCV. Usually this is an
     * inverse value of "cocpuWork" property.
     * TODO undocumented
     */
    FlagRO* const cocpuSleep;
    /**
     * When found true, indicates a working state of ULP FSM/RISCV. Usually this is an
     * inverse value of "cocpuSleep" property.
     * TODO undocumented
     */
    FlagRO* const cocpuWork;
    /**
     * When found true, indicates a state of ULP FSM/RISCV when it runs a program. Usually this is an
     * inverse value of "cocpuDone" property.
     * TODO undocumented
     */
    FlagRO* const cocpuRuns;
    FlagRO* const touchDone; //!< touch is done TODO undocumented
    FlagRO* const touchSleep; //!< touch is in sleep state TODO undocumented
    FlagRO* const touchWork; //!< touch is about to working. Switch rtc main state TODO undocumented
    FlagRO* const touchStart; //!< touch should start to work TODO undocumented
    FlagRO* const digiOff; //!< digital wrap power down TODO undocumented
    FlagRO* const digiISO; //!< digital wrap iso TODO undocumented
    FlagRO* const wifiOff; //!< wifi wrap power down TODO undocumented
    FlagRO* const wifiISO; //!< wifi iso TODO undocumented
    FlagRO* const periOff; //!< rtc peripheral power down TODO undocumented
    FlagRO* const periISO; //!< rtc peripheral iso TODO undocumented
    FlagRO* const dcdcOff; //!< External DCDC power down TODO undocumented
    FlagRO* const rom0Off; //!< rom0 power down TODO undocumented
public:
    PowerManagementUnit();
    virtual ~PowerManagementUnit();
public:
    SleepAndWakeupController* getSleepAndWakeupController();
    FastClockRTC* getFastClockRTC();
    SlowClockRTC* getSlowClockRTC();
    Oscillator32K* getOscillator32K();
    Oscillator8M*  getOscillator8M();
    OscillatorRTC* getOscillatorRTC();
};

#endif