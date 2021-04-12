// Controller of IO with GPIO pins.
//
// © 2021 Nikolai Varankine

#ifndef H_ControllerIO
#define H_ControllerIO

#include "soc/io_mux_reg.h"
#include "ExternalPin.hpp"
#include "MatrixBuffer.hpp"
#include "MatrixInterrupt.hpp"
#include "MatrixInput.hpp"
#include "MatrixOutput.hpp"
#include "bits/FlagRW.hpp"
#include "bits/SubValueRW.hpp"

class ControllerIO
{
private:
    static constexpr size_t MAX_INPUT = 256;
    static constexpr size_t MAX_OUTPUT = MAX_GPIO_NUM + 1;
    static constexpr size_t MAX_PAD = MAX_PAD_GPIO_NUM;
    const size_t RESERVED_PAD[4] { 22, 23, 24, 25 }; //TODO ld undef ref static constexpr
    MatrixInput* input[MAX_INPUT];
    MatrixOutput* output[MAX_OUTPUT];
    ExternalPin* pins[MAX_PAD];
public:
    /** Internal buffer for simple (direct) I/O */
    MatrixBuffer* const buffer;
    /** Interrupt controller for GPIO Matrix */
    MatrixInterrupt* const interrupt;
    /** Clock control. 0: gating, 1: running */
    FlagRW* const clock;
    /** IO pad power switch delay, delay unit is one APB clock */
    SubValueRW* const powerSwitchDelay;
    /** Power voltage for GPIO#[33,37]. 0: 3.3 V VDD3P3_CPU, 1: 1.8 V VDD_SPI */
    FlagRW* const power18;
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
    /**
     * @param i index of external GPIO pin, [0;MAX_PAD).
     * @return pointer to pin controller or nullptr for wrong index.
     */
    ExternalPin* getExternalPin( const size_t i );
};

#endif
