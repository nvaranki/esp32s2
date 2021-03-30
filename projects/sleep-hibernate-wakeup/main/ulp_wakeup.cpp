/* MCU hibernate Example: periodic wakeup of PRO processor by RTC timer.

   This file contains C code to run assembly program on the ULP. To emplloy C++ 
   API "mcu", the file itself is named as C++ file.

   The program prepares RTC timer that periodically wakes up an Xtensa CPU to run code.
   Main program increments a variable stored in persistent register.

   Author © 2021 Nikolai Varankine
*/

#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
#include "soc/soc.h"
#include "soc/uart_reg.h"
#include "esp_sleep.h"
#include "mcu.h"
#include "TimerRTC.hpp"
#include "BinaryImageULP.hpp"
#include "SleepAndWakeupController.hpp"

// static void printf( const PowerManagementUnit::Switch::SlowMemory* sm );
static void uart_tx_wait_idle(uint8_t uart_no);

RTC_FAST_ATTR int bootCount; // using RTC FAST memory, see note below

/**
 * Standard ESP-IDF main entry point, executed on every chip reset.
 */
extern "C" 
void app_main( void )
{
    MicroControllerUnit* const mcu = new MicroControllerUnit();
    PowerManagementUnit* const pmu = mcu->getPowerManagementUnit();
    SleepAndWakeupController* const swc = pmu->getSleepAndWakeupController();
    TimerRTC* tmr = pmu->getTimer();
    CoreLX7* cpu = mcu->getProcessor( 0 );
    
    uint32_t cause = 0;
    if( swc->wakeup.isCause( SleepAndWakeupController::Peripherals::TIMER ) ) 
    {
        printf( "\nRTC TIMER wakeup 0x%08x\n", swc->wakeup.cause->getAll() );
        swc->wakeup.on->off(); // found as 1, reset TODO why does it matter?
        
        // increment wakeup counter
        printf( "Wake count: %d\n", ++bootCount );
        if( bootCount >= 6 )
        {
            // Done. Stop timer reset
            printf("Final exit Xtensa program\n");
            return;
        }
    } 
    else if( ( cause = swc->wakeup.cause->getAll() ) == 0 )
    {
        printf( "\nMCU reset 0x%02x\n", (uint32_t) cpu->getResetCause() );

        // init persistent counter
        printf( "Wake count: %d\n", bootCount = 0 );
        
        // allow wakeup by timer only
        swc->wakeup.enable->setAll( false );
        swc->wakeup.setEnabled( SleepAndWakeupController::Peripherals::TIMER, true );

        // allow auto PD for everything
        pmu->ctrl.digital.sleepDn->on();
        pmu->ctrl.slowMemory.sleepDn->on();
        pmu->ctrl.fastMemory.sleepDn->off(); //TODO rather is works but "E (46) boot: Fast booting is not successful" //on();
        pmu->ctrl.peripherals.sleepDn->on(); // that differs hibernate from deep sleep
        pmu->ctrl.wifi.sleepDn->on();
        //TODO clocks?

        // reset forced power ON
        pmu->ctrl.digital.power->on->off();
        pmu->ctrl.slowMemory.power->on->off();
        pmu->ctrl.fastMemory.power->on->off();
        pmu->ctrl.peripherals.power->on->off();
        pmu->ctrl.wifi.power->on->off();
        
        // manage isolation of outputs
        pmu->ctrl.slowMemory.isolation->off->off();
        //TODO see above pmu->ctrl.fastMemory.isolation->off->off();
    }
    else
    {
        printf( "\nUnexpected wakeup cause %08x\n", cause );
        swc->wakeup.on->off(); // found as 1, reset TODO why does it matter?
    }

    // eveluate alarm timestamp
    tmr->update->on();
    uint64_t tw = 0x000000040000uL; // = 262144 -> 8 s @ 32 kHz
    uint64_t ta = tmr->getTimeStampCurrent() + tw; // 48 bits max
    tmr->setTimeStampAlarm( ta );
    printf( "Wait: %10lld up to 0x%012llx\n", tw, ta );
    
    // final message
    printf( "Entering hibernate...\n\n" );
    fflush(stdout);
    uart_tx_wait_idle(0);
    vTaskDelay( 20 );
    
    // start RTC timer
    tmr->alarm->on();
    
    // enter hibernate
    swc->sleep.start->on(); // normally never returns from there
    
    // Something went wrong
    printf( "Enter to hibernate aborted\n\n" );
    delete mcu;
    printf("Exit Xtensa program\n");
}

/* sleep_modes.c */
static void uart_tx_wait_idle(uint8_t uart_no) {
    uint32_t status;
    do {
        status = READ_PERI_REG(UART_STATUS_REG(uart_no));
        /* either tx count or state is non-zero */
    } while ((status & (UART_ST_UTX_OUT_M | UART_TXFIFO_CNT_M)) != 0);
}
