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
    /**
     * Sets current value to true.
     */
    inline void on() { set( true ); };
    /**
     * Sets current value to true.
     */
    inline void up() { set( true ); };
    /**
     * Sets current value to true.
     */
    inline void high() { set( true ); };
    /**
     * Sets current value to false.
     */
    inline void off() { set( false ); };
    /**
     * Sets current value to false.
     */
    inline void down() { set( false ); };
    /**
     * Sets current value to false.
     */
    inline void low() { set( false ); };
};

#endif
