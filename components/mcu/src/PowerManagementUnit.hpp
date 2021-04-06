// Power management unit of the MCU.
//
// © 2021 Nikolai Varankine

#ifndef H_PowerManagementUnit
#define H_PowerManagementUnit

#include "SleepAndWakeupController.hpp"
#include "FastClockRTC.hpp"
#include "MemoryClockRTC.hpp"
#include "SlowClockRTC.hpp"
#include "TimerRTC.hpp"
#include "Trigger2.hpp"
#include "Trigger3.hpp"
#include "bits/FlagRO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"

class PowerManagementUnit
{
private:
    TimerRTC* timer;
    SleepAndWakeupController* swc;
    FastClockRTC*   lpfc;
    SlowClockRTC*   lpsc;
    MemoryClockRTC* lpmc;
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
    FlagRO* const touchDone; //!< touch is done TODO undocumented
    FlagRO* const touchSleep; //!< touch is in sleep state TODO undocumented
    FlagRO* const touchWork; //!< touch is about to working. Switch rtc main state TODO undocumented
    FlagRO* const touchStart; //!< touch should start to work TODO undocumented
    FlagRO* const dcdcOff; //!< External DCDC power down TODO undocumented
    FlagRO* const rom0Off; //!< rom0 power down TODO undocumented
public:
    /** DC-DC voltage regulators */
    struct Regulator
    {
        /** RTC power domain */
        class Rtc
        {
        public:
            FlagRW* const powerUp; //!< REG force power up, to normal value
            FlagRW* const powerDn; //!< REG force power down, to reduced value
            SubValueRW* const biasSleep;  //!< BIAS during sleep
            SubValueRW* const biasWakeup; //!< BIAS during wakeup
        public:
            Rtc();
            virtual ~Rtc();
        }
        const rtc;
        /** Digital core power domain */
        class Digital
        {
        public:
        //  FlagRW* const powerUp; //!< REG force power up
        //  FlagRW* const powerDn; //!< REG force power down
            SubValueRW* const biasSleep;  //!< BIAS during sleep
            SubValueRW* const biasWakeup; //!< BIAS during wakeup
        public:
            Digital();
            virtual ~Digital();
        }
        const dig;
        /** SD flash power domain */
        class Flash
        {
            const int xxx=1;//TODO same for SDIO flash
        }
        const flash;
    }
    const reg;
    /** On-Off power switches */
    struct Switch
    {
        class Peripherals
        {
        public:
            Trigger3* const isolation;
            Trigger3* const power;
            FlagRW* const sleepDn;  //!< Set this bit to enable power down in sleep
            FlagRW* const holdGPIO;  //!< Set this bit the force hold the RTC GPIOs
        public:
            Peripherals();
            virtual ~Peripherals();
        }
        const peripherals;

        class FastMemory
        {
        public:
            Trigger2* const isolation;
            Trigger2* const power;
            FlagRW*   const sleepDn;  //!< Set this bit to enable power down in sleep
            Trigger2* const retain; //!< force retain (on=1) or not retain(off=1) the RTC memory
            FlagRW*   const followCPU;  //!< Set this bit to force power down when the CPU is powered down
        public:
            FastMemory();
            virtual ~FastMemory();
        }
        const fastMemory;

        class SlowMemory
        {
        public:
            Trigger2* const isolation;
            Trigger2* const power;
            FlagRW*   const sleepDn;  //!< Set this bit to enable power down in sleep
            Trigger2* const retain; //!< force retain (on=1) or not retain(off=1) the RTC memory
            FlagRW*   const followCPU;  //!< Set this bit to force power down when the CPU is powered down
        public:
            SlowMemory();
            virtual ~SlowMemory();
        }
        const slowMemory;

        class Digital
        {
        public:
            Trigger3* const isolation;
            Trigger3* const power;
            FlagRW*   const sleepDn; //!< 1: allow PD to deep/hibernate sleep, 0: stay PU to light/modem sleep and active state
            Trigger2* const memory; //!< FPU/FPD the memories in sleep
        public:
            Digital();
            virtual ~Digital();
        }
        const digital;
        
        class DigitalGPIO // not a power domain
        {
        public:
            Trigger2* const isolation;
            Trigger2* const hold;
            FlagWO* const holdAutoOff;  //!< Set this bit to clear the auto-hold enabler
            FlagRW* const holdAutoOn;  //!< Set this bit to allow enter the auto-hold state
            FlagRO* const holdAuto;  //!< Indicates the auto-hold status of the digital GPIOs
        public:
            DigitalGPIO();
            virtual ~DigitalGPIO();
        }
        const digitalGPIO;
        
        class WiFi
        {
        public:
            Trigger3* const isolation;
            Trigger3* const power; //!< controls digital Wi-Fi subsystem
            FlagRW* const sleepDn; //!< Set this bit to enable power down in sleep
        public:
            WiFi();
            virtual ~WiFi();
        }
        const wifi;
        
        class WiFiRX // not a power domain
        {
        public:
            Trigger2* const powerEST;
            Trigger2* const powerROT;
            Trigger2* const powerVIT;
            Trigger2* const powerDEMAP;
        public:
            WiFiRX();
            virtual ~WiFiRX();
        }
        const wifiRF;

        class WiFiRF // not a power domain
        {
        public:
            Trigger2* const powerIQ;
            Trigger2* const powerTX;
        public:
            WiFiRF();
            virtual ~WiFiRF();
        }
        const wifiRX;

        class SerialI2S // not a power domain
        {
        public:
            FlagRW*   const clockDMA; //!< DMA RAM clock
            Trigger2* const powerDMA; //!< DMA FIFO RAM power
            Trigger2* const powerPLC; //!< I2S memory power
            Trigger2* const powerFIFO; //!< FIFO power
        public:
            SerialI2S();
            virtual ~SerialI2S();
        }
        const i2s;
        
        class BusAPB // not a power domain
        {
        public:
            Trigger2* const powerDC;
            Trigger2* const powerPBUS;
            Trigger2* const powerAGC;
        public:
            BusAPB();
            virtual ~BusAPB();
        }
        const apb;
        
        class BaseBand // not a power domain
        {
        public:
            Trigger2* const powerFFT;
            Trigger2* const powerDC;
        public:
            BaseBand();
            virtual ~BaseBand();
        }
        const bband;
        
    }
    const ctrl;
public:
    PowerManagementUnit();
    virtual ~PowerManagementUnit();
public:
    TimerRTC* getTimer();
    SleepAndWakeupController* getSleepAndWakeupController();
    FastClockRTC* getFastClockRTC();
    SlowClockRTC* getSlowClockRTC();
    MemoryClockRTC* getMemoryClockRTC();
    Oscillator32K* getOscillator32K();
    Oscillator8M*  getOscillator8M();
    OscillatorRTC* getOscillatorRTC();
};

#endif