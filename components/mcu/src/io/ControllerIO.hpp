// Controller of IO with GPIO pins.
//
// © 2021 Nikolai Varankine

#ifndef H_ControllerIO
#define H_ControllerIO

#include "MatrixBuffer.hpp"
#include "MatrixInterrupt.hpp"
#include "MatrixInput.hpp"
#include "MatrixOutput.hpp"
#include "bits/FlagRW.hpp"

class ControllerIO
{
private:
    static constexpr size_t MAX_INPUT = 256;
    static constexpr size_t MAX_OUTPUT = 54;
    MatrixInput* input[MAX_INPUT];
    MatrixOutput* output[MAX_OUTPUT];
public:
    /** Internal buffer for simple (direct) I/O */
    MatrixBuffer* const buffer;
    /** Interrupt controller for GPIO Matrix */
    MatrixInterrupt* const interrupt;
    /** Clock control. 0: gating, 1: running */
    FlagRW* const clock;
    //TODO GPIO_BT_SELECT_REG GPIO bit selection register - Reserved (R/W)
    //TODO GPIO_SDIO_SELECT_REG GPIO SDIO selection register - Reserved (R/W)
    //TODO GPIO_STRAP_REG Bootstrap pin value register (RO)
public:
    ControllerIO();
    virtual ~ControllerIO();
public:
    /**
     * @param i index of input channel of GPIO Matrix, [0;MAX_INPUT).
     * @return pointer to input channel (GPIOn-->MCU) controller or nullptr for wrong index.
     */
    MatrixInput* getMatrixInput( const size_t i );
    /**
     * @param i index of output channel of GPIO Matrix, [0;MAX_OUTPUT).
     * @return pointer to output channel (MCU-->GPIOn) controller or nullptr for wrong index.
     */
    MatrixOutput* getMatrixOutput( const size_t i );
};

#endif
