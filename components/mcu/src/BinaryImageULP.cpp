// Binary image of program to run on Ultra Low Power (ULP) co-processor.
//
// © 2021 Nikolai Varankine

#include <cstring>
#include "esp32s2/ulp.h"
#include "BinaryImageULP.hpp"

BinaryImageULP::BinaryImageULP( const uint8_t* image , const size_t size ) :
    image( image ), size( size )
{
    header.magic = 0;
}

esp_err_t BinaryImageULP::loadAt( uint32_t* address )
{
    if( address == nullptr ) return ESP_ERR_INVALID_ARG;
    if( image == nullptr ) return ESP_ERR_INVALID_ARG;

    if( header.magic == 0 ) 
    {
        // extract a header (in case a program_binary isn't aligned)
        if( size < sizeof( header ) ) return ESP_ERR_INVALID_SIZE;
        memcpy( &header, image, sizeof( header ) );
    }
    if( header.magic != ULP_BINARY_MAGIC ) return ESP_ERR_NOT_SUPPORTED;
    size_t text_data_size = header.text_size + header.data_size;
    if( size - header.text_offset != text_data_size ) return ESP_ERR_INVALID_SIZE;

    if( (uint32_t) address < BASE ) return ESP_ERR_INVALID_ARG;
    if( (uint32_t) address + text_data_size + header.bss_size > TOPX ) return ESP_ERR_NO_MEM;

    // copy .text and .data sections
    memcpy( address, image + header.text_offset, text_data_size );

    // init .bss section
    memset( address + text_data_size, 0, header.bss_size );

    return ESP_OK;
}
