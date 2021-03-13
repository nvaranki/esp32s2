// 32 bit read only register.
//
// © 2021 Nikolai Varankine

#ifndef H_BitSetRO
#define H_BitSetRO

#include <stdint.h>

class BitSetRO
{
protected:
    const uint32_t addr; //!< address of the register
    const uint32_t view; //!< mask of participating bits
public:
    /**
     * @param a address of the register
     * @param v mask of participating bits
     */
    BitSetRO( const uint32_t a, const uint32_t v ) : addr( a ), view( v ) {};
    virtual ~BitSetRO() {};
public:
    /**
     * Returns presence of all specified test bits.
     * @param test one or combination of bits.
     * @return true if all specified bits were set to 1.
     */
    bool get( const uint32_t test ) const;
    /**
     * Returns presence of all declared bits.
     * @return all bits of the bit set view.
     */
    uint32_t getAll() const;
};

#endif