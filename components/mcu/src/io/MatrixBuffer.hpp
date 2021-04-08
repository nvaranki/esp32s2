#ifndef H_MatrixBuffer
#define H_MatrixBuffer

#include <stdint.h>
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/SubValueWO.hpp"
#include "bits/WordRO.hpp"
#include "bits/WordRW.hpp"
#include "bits/WordWO.hpp"

class MatrixBuffer
{
private:
    WordRO*     const inp0;
    SubValueRO* const inp1;
    WordWO*     const out0;
    SubValueWO* const out1;
    WordWO*     const set0;
    SubValueWO* const set1;
    WordWO*     const clr0;
    SubValueWO* const clr1;
    WordRW*     const enw0;
    SubValueRW* const enw1;
    WordWO*     const ens0;
    SubValueWO* const ens1;
    WordWO*     const enc0;
    SubValueWO* const enc1;
public:
    MatrixBuffer();
    virtual ~MatrixBuffer();
public:
    /** @return current value of all GPIO pins */
    uint64_t read() const;
    /** @param v value to set for all GPIO pins */
    void write( uint64_t v );
    /** @param m mask to selectively set GPIO pins to 1 */
    void set( uint64_t m );
    /** @param m mask to selectively set GPIO pins to 0 */
    void clear( uint64_t m );
    /** @return currently enabled all GPIO pins */
    uint64_t enabled() const;
    /** @param v value to enable write for all GPIO pins */
    void enable( uint64_t v );
    /** @param m mask to selectively enable write for GPIO pins */
    void setEnabled( uint64_t m );
    /** @param m mask to selectively disable write for GPIO pins */
    void clearEnabled( uint64_t m );
};

#endif
