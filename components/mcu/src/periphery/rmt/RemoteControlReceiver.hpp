// Receiver of signal/modulation and decoder of it to the signal.
// Includes carrier remover. Trasmits data to the RAM.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlReceiver
#define H_RemoteControlReceiver

#include "bits/FlagRW.hpp"
#include "bits/FlagRO.hpp"
#include "bits/FlagWO.hpp"
#include "bits/SubValueRO.hpp"
#include "bits/SubValueRW.hpp"

class RemoteControlReceiver
{
private:
    /* data */
public:
    /** allow to receive data */
    FlagRW* const enable;
    /** enable memory loop read mode when carrier modulation is enabled */
    FlagRW* const read;
    /** number 0f continuous clock cycles to detect idle state (no change on input) */
    SubValueRW* const idle;
    /** RAM address to write received signal */
    class Address
    {
    public:
        /** memory address offset when channel writes RAM via APB bus */
        SubValueRW* const offset;
        /** current memory address offset when channel writes RAM via APB bus */
        SubValueRO* const current;
        /** reset RAM write address */
        FlagWO* const reset;
        /** offset address is out of memory size */
        FlagRO* const error;
        /** more data received than the memory allows */
        FlagRO* const full;
    public:
        Address( const size_t channel );
        virtual ~Address();
    }
    const address;
    class Filter
    {
    public:
        /** filter received data */
        FlagRW* const enable;
        /** ignore the input pulse when its width is less than this value, in APB clock cycles */
        SubValueRW* const threshold;
    public:
        Filter( const size_t channel );
        virtual ~Filter();
    }
    const filter;
    /** Carrier remover configuration */
    class Carrier
    {
    public:
        /** Max number (low +1) of clock cycles in carrier modulation to recognize input as low signal TODO ?*/
        SubValueRW* const low;
        /** Min number (high +1) of clock cycles in carrier modulation to recognize input as high signal TODO ?*/
        SubValueRW* const high;
    public:
        Carrier( const size_t channel );
        virtual ~Carrier();
    }
    const carrier;
public:
    RemoteControlReceiver( const size_t channel );
    virtual ~RemoteControlReceiver();
};

#endif
