// 32 bit command (write only) register.
//
// © 2021 Nikolai Varankine

#ifndef H_BitSetWO
#define H_BitSetWO

#include <stdint.h>

class BitSetWO
{
protected:
    volatile uint32_t* const addr; //!< address of the register
    const uint32_t view; //!< mask of participating bits
public:
    /**
     * @param a address of the register
     * @param v mask of participating bits
     */
    BitSetWO( const uint32_t a, const uint32_t v ) : addr( (volatile uint32_t*) a ), view( v ) {};
    virtual ~BitSetWO() {}
public:
    /**
     * Sets or clears all specified bits same way.
     * @param test one or combination of bits.
     * @param value true if all specified bits should be set to 1, false to clear them.
     */
    void set( const uint32_t mask, const bool value );
};

#endif
