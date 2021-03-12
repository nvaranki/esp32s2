// Read and write value within 32 bit register.
//
// © 2021 Nikolai Varankine

#ifndef H_SubValueRW
#define H_SubValueRW

#include "SubValueRO.hpp"

class SubValueRW : public SubValueRO
{
public:
    /**
     * @param a address of the register
     * @param v continuous mask of participating bits
     * @param s shift of the mask relative to bit #0
     */
    SubValueRW( const uint32_t a, const uint32_t v, const uint8_t s ) : SubValueRO( a, v, s ) {}
    virtual ~SubValueRW() {}
public:
    /**
     * Shifts left specified value to align it with bit mask and then inserts masked result into register.
     * @param value a value to insert into register.
     */
    void set( const uint32_t value );
};

#endif