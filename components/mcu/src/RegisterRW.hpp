// 32 bit read and write register.
//
// © 2021 Nikolai Varankine

#ifndef H_RegisterRW
#define H_RegisterRW

#include "RegisterRO.hpp"

class RegisterRW : public RegisterRO
{
public:
    /**
     * @param a address of the register
     * @param v mask of participating bits
     */
    RegisterRW( const uint32_t a, const uint32_t v ) : RegisterRO( a, v ) {}
    virtual ~RegisterRW() {}
public:
    /**
     * Sets or clears all specified bits same way.
     * @param test one or combination of bits.
     * @param value true if all specified bits should be set to 1, false to clear them.
     */
    void set( const uint32_t mask, const bool value );
};

#endif