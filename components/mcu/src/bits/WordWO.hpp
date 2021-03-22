// 32 bit command (write only) register.
//
// © 2021 Nikolai Varankine

#ifndef H_WordWO
#define H_WordWO

#include <stdint.h>

class WordWO
{
protected:
    volatile uint32_t* const addr; //!< address of the register
public:
    /**
     * @param a address of the register
     */
    WordWO( const uint32_t a );
    virtual ~WordWO() {}
public:
    /**
     * @param value to be set
     */
    void set( const uint32_t value );
};

#endif
