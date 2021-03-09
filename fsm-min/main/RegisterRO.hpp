// 32 bit read only register.
//
// © 2021 Nikolai Varankine

#ifndef H_RegisterRO
#define H_RegisterRO

#include <stdint.h>

class RegisterRO
{
protected:
    const uint32_t addr; //!< address of the register
    const uint32_t view; //!< mask of participating bits
public:
    /**
     * @param a address of the register
     * @param v mask of participating bits
     */
    RegisterRO( const uint32_t a, const uint32_t v ) : addr( a ), view( v ) {};
    virtual ~RegisterRO() {};
public:
    /**
     * Returns presence of all specified bits.
     * @param test one or combination of bits.
     * @return true if all specified bits were set to 1.
     */
    bool get( const uint32_t test ) const;
};

#endif