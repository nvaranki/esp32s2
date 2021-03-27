// Sleep and wake-up controller of the MCU.
//
// © 2021 Nikolai Varankine

#ifndef H_SleepAndWakeupController
#define H_SleepAndWakeupController

#include "esp_bit_defs.h"
#include "soc/rtc_cntl_reg.h"
#include "bits/BitSetRW.hpp"
#include "bits/BitSetWO.hpp"
#include "bits/FlagRW.hpp"
#include "bits/FlagWO.hpp"

class SleepAndWakeupController
{
private:
    BitSetRW* const wcGPIO; //!< GPIO wakeup configuration register
    BitSetRW* const wcEXT1; //!< EXT1 wakeup configuration register
    BitSetRO* const wsEXT1; //!< EXT1 wakeup source register
public:
    SleepAndWakeupController();
    virtual ~SleepAndWakeupController();
public:
    /** Sleep-reject-wakeup targets, relatively (see Table 196) */
    enum class Peripherals : uint32_t
    {
        // RTC_CNTL_WAKEUP_STATE_REG (0x003C)
        // RTC_CNTL_SLP_REJECT_CAUSE_REG (0x0124)
        // RTC_CNTL_SLP_WAKEUP_CAUSE_REG (0x012C)
        //TODO  (see Table 196) vs WakeupEnable ?!
        EXT0    = BIT0,  //!< EXT0 GPIO wakeup
        EXT1    = BIT1,  //!< EXT1 GPIO wakeup
        GPIO    = BIT2,  //!< GPIO wakeup (light sleep only)
        TIMER   = BIT3,  //!< Timer wakeup
        SDIO    = BIT4,  //!< SDIO wakeup (light sleep only)
        WIFI    = BIT5,  //!< WIFI wakeup (light sleep only)
        UART0   = BIT6,  //!< UART0 wakeup (light sleep only)
        UART1   = BIT7,  //!< UART1 wakeup (light sleep only)
        TOUCH   = BIT8,  //!< Touch wakeup
        FSM     = BIT9,  //!< FSM ULP wakeup
        BT      = BIT10, //!< BT wakeup (light sleep only)
        COCPU   = BIT11, //TODO mismatch with doc
        XTAL32K = BIT12,
        RISCV   = BIT13, //!< RISCV ULP wakeup
        USB     = BIT14,
    };
    class Sleep 
    {
    public:
        FlagRW*   const start; //!< Sends the chip to sleep
    public:
        Sleep();
        virtual ~Sleep();
    }
    const sleep;
    class Reject
    {
    public:
        BitSetRW* const enable; //!< reject bitmap enabling register
        FlagRW*   const enableLightSleep; //!< Set this bit to enable reject-to-light-sleep
        FlagRW*   const enableDeepSleep; //!< Set this bit to enable reject-to-deep-sleep
        FlagRW*   const on; //!< Sleep reject bit
        BitSetRO* const cause; //!< Reject-to-sleep cause
        FlagWO*   const clear; //!< Clears the RTC reject-to-sleep cause
    private:
        inline uint32_t pme( const Peripherals mask ) const { return static_cast<uint32_t>( mask ) << RTC_CNTL_SLEEP_REJECT_ENA_S; }
        inline uint32_t pmc( const Peripherals mask ) const { return static_cast<uint32_t>( mask ) << RTC_CNTL_REJECT_CAUSE_S; }
    public:
        Reject();
        virtual ~Reject();
        bool isCause( const Peripherals test ) const { return cause->get( pmc( test ) ); };
        bool isEnabled( const Peripherals test ) const { return enable->get( pme( test ) ); };
        void setEnabled( const Peripherals mask, bool value ) const { 
            const_cast<BitSetRW *>(enable)->set( pme( mask ), value ); };
    }
    const reject;
    class Wakeup
    {
    public:
        BitSetRW* const enable; //!< Wakeup bitmap enabling register
        FlagRW*   const on; //!< Sleep wakeup bit
        BitSetRO* const cause; //!< Sleep-to-wakeup cause
    private:
        inline uint32_t pme( const Peripherals mask ) const { return static_cast<uint32_t>( mask ) << RTC_CNTL_WAKEUP_ENA_S; }
        inline uint32_t pmc( const Peripherals mask ) const { return static_cast<uint32_t>( mask ) << RTC_CNTL_WAKEUP_CAUSE_S; }
    public:
        Wakeup();
        virtual ~Wakeup();
        bool isEnabled( const Peripherals test ) const { return enable->get( pme( test ) ); };
        void setEnabled( const Peripherals mask, bool value ) const { 
            const_cast<BitSetRW *>(enable)->set( pme( mask ), value ); };
        bool isCause( const Peripherals test ) const { return cause->get( pmc( test ) ); };
    }
    const wakeup;
    FlagWO* const interrupt; //!< Sends a software RTC interrupt to CPU

    enum class WakeupConfigGPIO : uint32_t
    {
        // RTC_CNTL_EXT_WAKEUP_CONF_REG (0x0064)
        FILTER = BIT29, //!< enable the GPIO wakeup event filter
        WEXT0  = BIT30, //!< 0: EXT0 at low level, 1: EXT0 at high level
        WEXT1  = BIT31, //!< EXT1 at low level, 1: EXT1 at high level
    };
    enum class RejectConfig : uint32_t
    {
        // RTC_CNTL_SLP_REJECT_CONF_REG (0x0068)
        //TODO repeat WakeupEnable @13:29 ?
        LIGHT  = BIT30, //!< enable reject-to-light-sleep
        DEEP   = BIT31, //!< enable reject-to-deep-sleep
    };
    enum class WakeupConfigEXT1 : uint32_t
    {
        // RTC_CNTL_EXT_WAKEUP1_REG (0x00DC)
        //TODO RTC GPIO @00:22 ?
        //TODO EN0  = BIT0, //!< select RTC GPIO to be the EXT1 wakeup source
        CLEAR = BIT22, //!< clear EXT1 wakeup status; WO //TODO write-only bit
    };
    enum class WakeupStatusEXT1 : uint32_t
    {
        // RTC_CNTL_EXT_WAKEUP1_REG (0x00DC)
        //TODO RTC GPIO @00:22 ?
        //TODO EN0  = BIT0, //!< RTC GPIO wakeup status
    };
public:
    bool getConfigGPIO( const WakeupConfigGPIO test ) const { return wcGPIO->get( static_cast<uint32_t>( test ) ); };
    void setConfigGPIO( const WakeupConfigGPIO mask, bool value ) { wcGPIO->set( static_cast<uint32_t>( mask ), value ); };
    bool getConfigEXT1( const WakeupConfigEXT1 test ) const { return wcEXT1->get( static_cast<uint32_t>( test ) ); };
    void setConfigEXT1( const WakeupConfigEXT1 mask, bool value ) { wcEXT1->set( static_cast<uint32_t>( mask ), value ); };
    bool isSleepToWakeupCause( const WakeupStatusEXT1 test ) const { return wsEXT1->get( static_cast<uint32_t>( test ) ); };
    //TODO XOR? bool hasAnySleepToWakeupCause() const { return s2wc->get( RTC_CNTL_WAKEUP_CAUSE_M ); };
};

#endif