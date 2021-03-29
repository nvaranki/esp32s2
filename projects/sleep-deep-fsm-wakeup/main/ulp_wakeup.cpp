/* ULP FSM Example: start of PRO processor from deep sleep.

   This file contains C code to run assembly program on the ULP. To emplloy C++ 
   API "mcu", the file itself is named as C++ file.

   The program prepares ULP timer that periodically wakes up an ULP FSM to run code.
   Main program stays on hold to let FSM to do some work few times. 
   To make sure the FSM really runs, an ASM program increments public variable.

   Author © 2021 Nikolai Varankine
*/

#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc.h"
#include "soc/uart_reg.h"
#include "esp_sleep.h"
#include "ulp_main.h"
#include "mcu.h"
#include "BinaryImageULP.hpp"
#include "SleepAndWakeupController.hpp"

static void printf( const PowerManagementUnit::Switch::SlowMemory* sm );
static void uart_tx_wait_idle(uint8_t uart_no);

// ASM program placeholders
extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");

/**
 * Standard ESP-IDF main entry point, executed on every chip reset.
 */
extern "C" 
void app_main( void )
{
    MicroControllerUnit* const mcu = new MicroControllerUnit();
    PowerManagementUnit* const pmu = mcu->getPowerManagementUnit();
    SleepAndWakeupController* const swc = pmu->getSleepAndWakeupController();
    CoprocessorULP* const ulp = mcu->getCoprocessorULP();
    CoreFSM* const fsm = ulp->getCoreFSM();
    TimerULP* const tmr = ulp->getTimerULP();
    CoreLX7* cpu = mcu->getProcessor( 0 );
    
    uint32_t cause = 0;
    if( swc->wakeup.isCause( SleepAndWakeupController::Peripherals::FSM ) ) 
    {
        printf( "\nULP FSM wakeup\n" );
        printf( "Edge count: %10d     Wake count: %3d\n", ulp_edge_count, ulp_wake_count );
        if( ulp_wake_count >= 6 )
        {
            // Done. Stop both programs
            printf("Cancelling FSM program...\n");
            tmr->active->set( false );
            fsm->clockOn->set( false );
            printf("Final exit Xtensa program\n");
            return;
        }
    } 
    else if( ( cause = swc->wakeup.cause->getAll() ) == 0 )
    {
        printf( "\nMCU reset 0x%02x\n", (uint32_t) cpu->getResetCause() );

        // cancel next timer run if any
        tmr->active->set( false ); 

        // load code
        BinaryImageULP image( ulp_main_bin_start, ulp_main_bin_end - ulp_main_bin_start );
        esp_err_t e = image.loadAt( RTC_SLOW_MEM );
        if( e != ESP_OK )
        {
            printf( "Failed to load ULP program (%x), aborting...\n", e );
            delete mcu;
            return;
        }
        ulp->entry->set( &ulp_entry - RTC_SLOW_MEM );

        // init shared vars, not earlier than the program has been loaded 
        ulp_edge_count = 0;
        ulp_wake_count = 0;
        printf( "Edge count: %10d\n", ulp_edge_count );
        
        // select FSM to run
        fsm->selectForExec();
        fsm->selectForDone();
        fsm->startOn->set( false ); // block software start of ULP
        fsm->clockOn->set( true );
        fsm->clockOff->set( false );

        // ensure power ON for RTC slow memory
        printf( &pmu->ctrl.slowMemory );
        pmu->ctrl.slowMemory.sleepDn->set( false ); // found false, but anyway...

        // allow Xtensa wakeup by FSM
        swc->wakeup.setEnabled( SleepAndWakeupController::Peripherals::FSM, true );

        // start timer
        tmr->sleep->set( 500 );
        tmr->active->set( true );
    }
    else
    {
        printf( "\nUnexpected wakeup cause %08x\n", cause );
    }

    // Get ready to sleep
    printf( "Ready to sleep\n\n" );
    fflush(stdout);
    uart_tx_wait_idle(0);
    vTaskDelay( 20 );
    pmu->ctrl.digital.sleepDn->set( true ); // go deep in sleep
    // pmu->digiNoDeepSleep->set( true );
    swc->sleep.start->set( false );
    swc->sleep.start->set( true ); // never returns from there

    // Something went wrong
    printf( "Enter to sleep aborted\n\n" );
    delete mcu;
    printf("Exit Xtensa program\n");
}

static void printf( const PowerManagementUnit::Switch::SlowMemory* sm )
{
    printf( "Slow memory:  PU=%1d PD=%1d  RU=%1d RD=%1d  IU=%1d ID=%1d  SLP=%1d  CPU=%1d  \n",
        sm->power->on->get(),
        sm->power->off->get(),
        sm->retain->on->get(),
        sm->retain->off->get(),
        sm->isolation->on->get(),
        sm->isolation->off->get(),
        sm->sleepDn->get(),
        sm->followCPU->get()
        );
}

/* sleep_modes.c */
static void uart_tx_wait_idle(uint8_t uart_no) {
    uint32_t status;
    do {
        status = READ_PERI_REG(UART_STATUS_REG(uart_no));
        /* either tx count or state is non-zero */
    } while ((status & (UART_ST_UTX_OUT_M | UART_TXFIFO_CNT_M)) != 0);
}
