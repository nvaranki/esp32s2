#ifndef H_MatrixBuffer
#define H_MatrixBuffer

#include <stdint.h>
#include "soc/soc.h"
#include "soc/io_mux_reg.h"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"
#include "bits/SubValueWO.hpp"
#include "bits/WordRO.hpp"
#include "bits/WordRW.hpp"
#include "bits/WordWO.hpp"

class MatrixBuffer
{
private:
    static uint64_t constexpr MASK_L = 0xFFFFFFFFuL; // pins #[0.31]
    static uint64_t constexpr MASK_H = 0x003FFFFFuL; // pins #[32,53]
    static size_t   constexpr MAX_CHANNEL = MAX_GPIO_NUM + 1;
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
    /** @return current value of GPIO pins #[0.31] */
    inline uint32_t readL() const { return inp0->get(); }
    /** @return current value of GPIO pins #[32,53] */
    inline uint32_t readH() const { return inp1->get(); }
    /** @param v value to set for all GPIO pins */
    void write( uint64_t v );
    /** @param v value to set for GPIO pins #[0.31] */
    inline void writeL( uint32_t v ) { out0->set( v ); }
    /** @param v value to set for GPIO pins #[32,53] */
    inline void writeH( uint32_t v ) { out1->set( v & 0x003FFFFFuL ); }
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
public:
    class Channel
    {
    private:
        MatrixBuffer* const mb;
        uint32_t const mask;
    public:
        size_t const id;
    public:
        Channel( MatrixBuffer* mb, const size_t id ) : mb( mb ), mask( BIT( id % 32 ) ), id( id ) {}
        virtual ~Channel() {}
    public:
        void enable( const bool v );
        void write( const bool v );
        bool read() const;
    }
    * channel[MAX_CHANNEL];
    Channel* getChannel( const size_t i );
};

#endif
