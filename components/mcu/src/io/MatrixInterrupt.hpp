// Interrupt controller of GPIO Matrix.
//
// © 2021 Nikolai Varankine

#ifndef H_MatrixInterrupt
#define H_MatrixInterrupt

#include <stdint.h>
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/SubValueWO.hpp"
#include "bits/WordRO.hpp"
#include "bits/WordRW.hpp"
#include "bits/WordWO.hpp"

class MatrixInterrupt
{
private:
    WordRO*     const src0;
    SubValueRO* const src1;
    WordRW*     const stw0;
    SubValueRW* const stw1;
    WordWO*     const sts0;
    SubValueWO* const sts1;
    WordWO*     const stc0;
    SubValueWO* const stc1;
    //TODO PRO CPU masked/unmasked
public:
    MatrixInterrupt();
    virtual ~MatrixInterrupt();
public:
    /** @return current interrupt status value of all GPIO pins */
    uint64_t source() const;
    /** @return current interrupt status for all GPIO pins */
    uint64_t status() const;
    /** @param v value to set interrupt status for all GPIO pins */
    void write( uint64_t v );
    /** @param m mask to selectively set GPIO pin interrupt status to 1 */
    void set( uint64_t m );
    /** @param m mask to selectively set GPIO pin interrupt status to 0 */
    void clear( uint64_t m );
};

#endif
