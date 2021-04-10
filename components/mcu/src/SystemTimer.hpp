// Main 64 bit system timer.
//
// © 2021 Nikolai Varankine

#ifndef H_SystemTimer
#define H_SystemTimer

#include <stdint.h>
#include "bits/FlagRO.hpp"
#include "bits/FlagWO.hpp"
#include "bits/WordRO.hpp"

class SystemTimer
{
public:
    static constexpr uint64_t INVALID = 0u;
private:
    WordRO* const valueLO;
    WordRO* const valueHI;
public:
    /** Used to update system timer value to registers */
    FlagWO* const update;
    /** Check if it is valid to read out timer value from registers after update */
    FlagRO* const valid;
public:
    SystemTimer();
    virtual ~SystemTimer();
public:
    /** @return current value from the timer */
    uint64_t getValue() const;
    /** 
     * @param attempts number of attempts to repeat a check for updated value is ready
     * @return current value from the timer or INVALID value if all attempts were spent with no success
     * */
    uint64_t getValidValue( uint32_t attempts );
};

#endif
