// Ultra low power (ULP) co-processor.
//
// © 2021 Nikolai Varankine

#include <cstring>
#include "soc/rtc_cntl_reg.h"
#include "esp32s2/ulp.h"
#include "CoprocessorULP.hpp"

CoprocessorULP::CoprocessorULP() :
    fsm( new CoreFSM() ),
    riscv( new CoreRISCV() ),
    timer( new TimerULP() ),
    cfgCore( new BitSetRW( RTC_CNTL_COCPU_CTRL_REG, 0x01800000 ) ), //TODO predefined constants
    cfgGPIO( new BitSetRW( RTC_CNTL_ULP_CP_TIMER_REG, 0x60000000 ) ), //TODO predefined constants
    entry( new SubValueRW( RTC_CNTL_ULP_CP_TIMER_REG, RTC_CNTL_ULP_CP_PC_INIT_M, RTC_CNTL_ULP_CP_PC_INIT_S ) )
{
}
    
CoprocessorULP::~CoprocessorULP()
{
    delete fsm;
    delete riscv;
    delete timer;
    delete cfgCore;
    delete cfgGPIO;
    delete entry;
}

typedef struct 
{
    uint32_t magic;
    uint16_t text_offset;
    uint16_t text_size;
    uint16_t data_size;
    uint16_t bss_size;
} 
ulp_binary_header_t;

esp_err_t CoprocessorULP::loadExecCode( uint32_t* address, const uint8_t* image , const size_t size )
{
    if( address == nullptr ) return ESP_ERR_INVALID_ARG;
    if( image == nullptr ) return ESP_ERR_INVALID_ARG;

    // extract a header (in case a program_binary isn't aligned)
    ulp_binary_header_t header;
    if( size < sizeof( ulp_binary_header_t ) ) return ESP_ERR_INVALID_SIZE;
    memcpy( &header, image, sizeof( header ) );
    if( header.magic != ULP_BINARY_MAGIC_ESP32 ) return ESP_ERR_NOT_SUPPORTED;
    size_t text_data_size = header.text_size + header.data_size;

    if( address < RTC_SLOW_MEM ) return ESP_ERR_INVALID_ARG;
    if( (uint8_t*) address + text_data_size + header.bss_size > 
        (uint8_t*) RTC_SLOW_MEM + CONFIG_ESP32S2_ULP_COPROC_RESERVE_MEM ) 
        return ESP_ERR_NO_MEM;

    // copy .text and .data sections
    memcpy( address, image + header.text_offset, text_data_size );

    // init .bss section
    memset( address + text_data_size, 0, header.bss_size );

    return ESP_OK;
}
