// Common set of controls for single interrupt.
//
// © 2021 Nikolai Varankine

#include "InterruptController.hpp"

InterruptController::InterruptController( FlagRO* const r, FlagRO* const m, FlagRW* const e, FlagWO* const c ) :
    raw( r ), masked( m ), enable( e ), clear( c )
{
}
