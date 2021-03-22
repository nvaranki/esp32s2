// 32 bit read only register.
//
// © 2021 Nikolai Varankine

#ifndef H_WordRO
#define H_WordRO

#include <stdint.h>

class WordRO
{
protected:
    volatile uint32_t* const addr; //!< address of the register
public:
    /**
     * @param a address of the register
     */
    WordRO( const uint32_t a );
    virtual ~WordRO() {};
public:
    /**
     * @return current value.
     */
    uint32_t get() const;
};

#endif
