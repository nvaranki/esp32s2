// Command (write only) value within 32 bit register.
//
// © 2021 Nikolai Varankine

#ifndef H_SubValueWO
#define H_SubValueWO

#include <stdint.h>

class SubValueWO
{
protected:
    volatile uint32_t* const addr; //!< address of the register
    const uint32_t view; //!< continuous mask of participating bits
    const uint8_t shift; //!< shift of the mask relative to bit #0
public:
    /**
     * @param a address of the register
     * @param v continuous mask of participating bits
     * @param s shift of the mask relative to bit #0
     */
    SubValueWO( const uint32_t a, const uint32_t v, const uint8_t s );
    virtual ~SubValueWO() {};
public:
    /**
     * Shifts left specified value to align it with bit mask and then inserts masked result into register.
     * @param value a value to insert into register.
     */
    void set( const uint32_t value );
    /**
     * @return maximum allowed value.
     */
    uint32_t max() const { return view >> shift; };
};

#endif
