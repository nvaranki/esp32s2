// FSM (finite state machine) core of ULP co-processor.
//
// © 2021 Nikolai Varankine

#ifndef H_CoreFSM
#define H_CoreFSM

#include "esp_bit_defs.h"
#include "soc/rtc_cntl_reg.h"
#include "bits/BitSetRW.hpp"
#include "bits/BitSetWO.hpp"
#include "bits/SubValueRW.hpp"

class CoreFSM
{
private:
    SubValueRW* const isz; //!< TODO undocumented init memory address size?
    BitSetWO* const cmd; //!< TODO undocumented
    SubValueRW* const asz; //!< TODO undocumented memory address size
    BitSetRW* const cfg; //!< configuration register
public:
    CoreFSM();
    virtual ~CoreFSM();
public:
    enum class Configuration : uint32_t
    {
        // RTC_CNTL_ULP_CP_CTRL_REG (0x00FC)
        //TODO reserved bits found described in software, see undocumented registers in the class
        CLKFO  = RTC_CNTL_ULP_CP_CLK_FO, //!< processor clock force on
        CLKRST = RTC_CNTL_ULP_CP_RESET, //!< software reset the processor clock
        STARTS = RTC_CNTL_ULP_CP_FORCE_START_TOP, //!< 0->1: to start processor by software
        STARTH = RTC_CNTL_ULP_CP_START_TOP, //!< 1: to start processor by hardware
    };
public:
    uint32_t getMemoryAddressInit() { return isz->get(); }; //TODO unspecified
    void setMemoryAddressInit( uint32_t v ) { isz->set( v ); }; //TODO unspecified
    uint32_t getMemoryAddressSize() { return asz->get(); }; //TODO unspecified
    void setMemoryAddressSize( uint32_t v ) { asz->set( v ); }; //TODO unspecified
    void clearMemoryOffset() { cmd->set( RTC_CNTL_ULP_CP_MEM_OFFST_CLR_M, true ); }; //TODO unspecified
    bool getConfig( const Configuration test ) const { return cfg->get( static_cast<uint32_t>( test ) ); };
    void setConfig( const Configuration mask, bool value ) { cfg->set( static_cast<uint32_t>( mask ), value ); };
    void start();
};

#endif