// One bit command (write only) value within 32 bit register.
//
// © 2021 Nikolai Varankine

#ifndef H_FlagWO
#define H_FlagWO

#include <stdint.h>

class FlagWO
{
protected:
    volatile uint32_t* const addr; //!< address of the register
    const uint32_t view; //!< mask of participating bit
public:
    /**
     * @param a address of the register
     * @param s zero based bit index within 32 bit register
     */
    FlagWO( const uint32_t a, const uint8_t s );
    virtual ~FlagWO() {}
public:
    /**
     * @param value to be set
     */
    void set( const bool value );
};

#endif
