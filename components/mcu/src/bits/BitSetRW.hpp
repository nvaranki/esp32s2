// 32 bit read and write register.
//
// © 2021 Nikolai Varankine

#ifndef H_BitSetRW
#define H_BitSetRW

#include "BitSetRO.hpp"

class BitSetRW : public BitSetRO
{
public:
    /**
     * @param a address of the register
     * @param v mask of participating bits
     */
    BitSetRW( const uint32_t a, const uint32_t v ) : BitSetRO( a, v ) {}
    virtual ~BitSetRW() {}
public:
    /**
     * Sets or clears all specified bits same way.
     * @param test one or combination of bits.
     * @param value true if all specified bits should be set to 1, false to clear them.
     */
    void set( const uint32_t mask, const bool value );
    /**
     * Sets or clears all participating bits same way.
     * @param value true if all bits should be set to 1, false to clear them.
     */
    void setAll( const bool value );
};

#endif