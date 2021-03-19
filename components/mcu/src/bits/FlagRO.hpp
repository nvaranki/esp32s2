// One bit read only value within 32 bit register.
//
// © 2021 Nikolai Varankine

#ifndef H_FlagRO
#define H_FlagRO

#include <stdint.h>

class FlagRO
{
protected:
    volatile uint32_t* const addr; //!< address of the register
    const uint32_t view; //!< mask of participating bit
public:
    /**
     * @param a address of the register
     * @param s zero based bit index within 32 bit register
     */
    FlagRO( const uint32_t a, const uint8_t s );
    virtual ~FlagRO() {};
public:
    /**
     * @return current value.
     */
    bool get() const;
};

#endif
