// Carrier demodulator management, part of remote control receiver.
//
// © 2021 Nikolai Varankine

#ifndef H_RemoteControlReceiverCarrier
#define H_RemoteControlReceiverCarrier

#include <stdint.h>
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class RemoteControlReceiverCarrier
{
public:
    /** remove carrier modulation 
     * TODO no effect on gen
     * TODO from API:
     * This function has different meaning for TX and RX
     * TX: enable to modulate carrier (NV: no effect)
     * RX: enable to demodulate carrier (NV: suppusedly true)
     */
    FlagRW* const enable;
    /** remove modulation from input TODO wrong desc --> ?! enable memory loop read mode when carrier modulation is enabled */
    FlagRW* const remove;
    /** 
     * Max number (low +1) of clock cycles in carrier modulation to recognize input as low signal TODO ?
     * TODO API: there could be residual in timing the carrier pulse, so double enlarge the theoretical value
     * */
    SubValueRW* const low;
    /** 
     * Min number (high +1) of clock cycles in carrier modulation to recognize input as high signal TODO ?
     * TODO API: there could be residual in timing the carrier pulse, so double enlarge the theoretical value
     * */
    SubValueRW* const high;
public:
    RemoteControlReceiverCarrier( const uint32_t channel );
    virtual ~RemoteControlReceiverCarrier();
};

#endif
