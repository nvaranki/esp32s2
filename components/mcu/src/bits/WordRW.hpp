// 32 bit read and write register.
//
// © 2021 Nikolai Varankine

#ifndef H_WordRW
#define H_WordRW

#include "WordRO.hpp"

class WordRW : public WordRO
{
public:
    /**
     * @param a address of the register
     */
    WordRW( const uint32_t a ) : WordRO( a ) {}
    virtual ~WordRW() {}
public:
    /**
     * @param value to be set
     */
    void set( const uint32_t value );
};

#endif
