// Controller of IO with GPIO pins.
//
// © 2021 Nikolai Varankine

#include "soc/io_mux_reg.h"
#include "ControllerIO.hpp"

ControllerIO::ControllerIO() :
    buffer( new MatrixBuffer() ),
    interrupt( new MatrixInterrupt() ),
    clock( new FlagRW( GPIO_CLOCK_GATE_REG, GPIO_CLK_EN_S ) ),
    powerSwitchDelay( new SubValueRW( PIN_CTRL, PAD_POWER_SWITCH_DELAY_M, PAD_POWER_SWITCH_DELAY_S ) ),
    power18( new FlagRW( PIN_CTRL, PAD_POWER_SEL_S ) )
{
    for( int i = 0; i < MAX_INPUT;  i++ ) input [i] = nullptr;
    for( int i = 0; i < MAX_OUTPUT; i++ ) output[i] = nullptr;
    for( int i = 0; i < MAX_PAD;    i++ ) pins  [i] = nullptr;
}
    
ControllerIO::~ControllerIO()
{
    delete buffer;
    delete interrupt;
    delete clock;
    delete powerSwitchDelay;
    delete power18;
    for( int i = 0; i < MAX_INPUT;  i++ ) if( input [i] != nullptr ) delete input [i];
    for( int i = 0; i < MAX_OUTPUT; i++ ) if( output[i] != nullptr ) delete output[i];
    for( int i = 0; i < MAX_PAD;    i++ ) if( pins  [i] != nullptr ) delete pins  [i];
}

MatrixInput* ControllerIO::getMatrixInput( const size_t i )
{
    if( i >= MAX_INPUT ) return nullptr;
    if( input[i] == nullptr ) input[i] = new MatrixInput( i );
    return input[i];
}
    
MatrixOutput* ControllerIO::getMatrixOutput( const size_t i )
{
    if( i >= MAX_OUTPUT ) return nullptr;
    if( output[i] == nullptr ) output[i] = new MatrixOutput( i );
    return output[i];
}
    
ExternalPin* ControllerIO::getExternalPin( const size_t i )
{
    if( i >= MAX_PAD ) return nullptr;
    if( pins[i] == nullptr ) pins[i] = new ExternalPin( i );
    return pins[i];
}
