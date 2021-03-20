// One bit read and write value within 32 bit register.
//
// © 2021 Nikolai Varankine

#ifndef H_FlagRW
#define H_FlagRW

#include "FlagRO.hpp"

class FlagRW : public FlagRO
{
public:
    /**
     * @param a address of the register
     * @param s zero based bit index within 32 bit register
     */
    FlagRW( const uint32_t a, const uint8_t s ) : FlagRO( a, s ) {}
    virtual ~FlagRW() {}
public:
    /**
     * @param value to be set
     */
    void set( const bool value );
};

#endif