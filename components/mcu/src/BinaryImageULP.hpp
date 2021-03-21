// Binary image of program to run on Ultra Low Power (ULP) co-processor.
//
// © 2021 Nikolai Varankine

#ifndef H_BinaryImageULP
#define H_BinaryImageULP

#include "esp_err.h"
#include "esp32s2/ulp.h"

class BinaryImageULP 
{
public:
    static constexpr uint32_t BASE = 0x50000000; //TODO known as RTC_SLOW_MEM in "ulp.h"
    static constexpr uint32_t TOPX = BASE + CONFIG_ESP32S2_ULP_COPROC_RESERVE_MEM;
private:
    static constexpr uint32_t ULP_BINARY_MAGIC = 0x00706c75; // known as ULP_BINARY_MAGIC_ESP32 in "ulp.c"
private:
    const uint8_t* image;
    const size_t size;
public:
    /**
     * @param image compiled binary image.
     * @param size size of the image, in bytes.
     */
    BinaryImageULP( const uint8_t* image , const size_t size );
    virtual ~BinaryImageULP() {};
public:
    /**
     * @param address memory location to load binary at, usually beginning of RTC slow memory (RTC_SLOW_MEM) with optional offset.
     * @return completion code.
     */
    esp_err_t loadAt( uint32_t* address );
private:
    struct 
    {
        uint32_t magic;
        uint16_t text_offset;
        uint16_t text_size;
        uint16_t data_size;
        uint16_t bss_size;
    } 
    header;
};

#endif
