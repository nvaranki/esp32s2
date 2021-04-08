// Controller of IO with GPIO pins.
//
// © 2021 Nikolai Varankine

#include "ControllerIO.hpp"

ControllerIO::ControllerIO() :
    buffer( new MatrixBuffer() )
{
    for( int i = 0; i < MAX_INPUT;  i++ ) input [i] = nullptr;
    for( int i = 0; i < MAX_OUTPUT; i++ ) output[i] = nullptr;
}
    
ControllerIO::~ControllerIO()
{
    delete buffer;
    for( int i = 0; i < MAX_INPUT;  i++ ) if( input [i] != nullptr ) delete input [i];
    for( int i = 0; i < MAX_OUTPUT; i++ ) if( output[i] != nullptr ) delete output[i];
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
