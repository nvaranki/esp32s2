// FSM (finite state machine) core of ULP co-processor.
//
// © 2021 Nikolai Varankine

#ifndef H_CoreFSM
#define H_CoreFSM

#include "esp_bit_defs.h"
#include "BitSetRW.hpp"

class CoreFSM
{
private:
    BitSetRW* const cfg; //!< configuration register
public:
    CoreFSM();
    virtual ~CoreFSM();
public:
    enum class Configuration : uint32_t
    {
        // RTC_CNTL_ULP_CP_CTRL_REG (0x00FC)
        //TODO reserved bits found described in software
        CLKFO  = BIT28, //!< clock force on @see RTC_CNTL_ULP_CP_CLK_FO
        CLKRST = BIT29, //!< reset the processor clock @see RTC_CNTL_ULP_CP_RESET
        STARTS = BIT30, //!< 1: to start processor by software @see RTC_CNTL_ULP_CP_FORCE_START_TOP
        STARTH = BIT31, //!< 1: to start processor by hardware @see RTC_CNTL_ULP_CP_START_TOP
    };
public:
    bool getConfig( const Configuration test ) const { return cfg->get( static_cast<uint32_t>( test ) ); };
    void setConfig( const Configuration mask, bool value ) { cfg->set( static_cast<uint32_t>( mask ), value ); };
};

#endif