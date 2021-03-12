// Read only value within 32 bit register.
//
// © 2021 Nikolai Varankine

#ifndef H_SubValueRO
#define H_SubValueRO

#include <stdint.h>

class SubValueRO
{
protected:
    const uint32_t addr; //!< address of the register
    const uint32_t view; //!< continuous mask of participating bits
    const uint8_t shift; //!< shift of the mask relative to bit #0
public:
    /**
     * @param a address of the register
     * @param v continuous mask of participating bits
     * @param s shift of the mask relative to bit #0
     */
    SubValueRO( const uint32_t a, const uint32_t v, const uint8_t s );
    virtual ~SubValueRO() {};
public:
    /**
     * Extracts masked value from register and then shifts it right to align with bit #0.
     * @return current value.
     */
    uint32_t get() const;
};

#endif