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
#include "soc/soc.h"
#include "soc/uart_reg.h"
#include "esp_sleep.h"
#include "mcu.h"
#include "TimerRTC.hpp"
#include "BinaryImageULP.hpp"
#include "SleepAndWakeupController.hpp"

// static void printf( const PowerManagementUnit::Switch::SlowMemory* sm );
static void uart_tx_wait_idle(uint8_t uart_no);

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
    WordRW* rr = mcu->getRetentionRegister( 4 );
    CoreLX7* cpu = mcu->getProcessor( 0 );
    
    uint32_t cause = 0;
    if( swc->wakeup.isCause( SleepAndWakeupController::Peripherals::TIMER ) ) 
    {
        tmr->clearInterrupt->on();
        tmr->clearInterrupt->off();
        tmr->alarm->off();
        swc->wakeup.on->off();
        printf( "\nRTC TIMER wakeup 0x%08x\n", swc->wakeup.cause->getAll() );
        uint32_t wc = rr->get();
        printf( "Wake count: %10d (0x%08x)\n", wc, wc );
        // rr->set( ++wc );
        // if( wc >= 6 )
        // {
        //     // Done. Stop timer reset
        //     printf("Final exit Xtensa program\n");
        //     return;
        // }
    } 
    else if( ( cause = swc->wakeup.cause->getAll() ) == 0 )
    {
        printf( "\nMCU reset 0x%02x\n", (uint32_t) cpu->getResetCause() );

        // init persistent counter
        // rr->set( 0 );
        printf( "Wake count: %10d\n", rr->get() );
        
        // allow Xtensa wakeup by timer
        swc->wakeup.enable->setAll( false );
        swc->wakeup.setEnabled( SleepAndWakeupController::Peripherals::TIMER, true );
        //swc->wakeup.on->on();

        // allow auto PD for everything
        pmu->ctrl.digital.sleepDn->on();
        pmu->ctrl.slowMemory.sleepDn->on();
        pmu->ctrl.fastMemory.sleepDn->off(); //TODO "E (46) boot: Fast booting is not successful" //on();
        pmu->ctrl.peripherals.sleepDn->on(); // that differs hibernate from deep sleep
        pmu->ctrl.wifi.sleepDn->on();
        //TODO clocks?

        // reset forced power ON
        pmu->ctrl.digital.power->on->off();
        pmu->ctrl.slowMemory.power->on->off();
        pmu->ctrl.fastMemory.power->on->off();
        pmu->ctrl.peripherals.power->on->off();
        pmu->ctrl.wifi.power->on->off();
        
        // // manage isolation of outputs
        pmu->ctrl.slowMemory.isolation->off->off();
        // pmu->ctrl.fastMemory.isolation->off->off();
    }
    else
    {
        printf( "\nUnexpected wakeup cause %08x\n", cause );
    }

    // eveluate alarm timestamp
    tmr->update->on();
    uint64_t thr = 0x000000040000uL; // = 262144 -> 8 s @ 32 kHz, 48 bits max
    uint64_t ta = tmr->getTimeStampCurrent() + thr;
    tmr->setTimeStampAlarm( ta );
    printf( "Wait: %10lld up to 0x%012llx\n", thr, ta );
    
    // get timer ready
    tmr->interrupt->off();
    tmr->clearInterrupt->on();
    //tmr->alarm->off();
    
    // final message
    printf( "Ready to hibernate\n\n" );
    fflush(stdout);
    uart_tx_wait_idle(0);
    vTaskDelay( 20 );
    
    // start RTC timer
    tmr->alarm->on();
    
    // enter hibernate
    swc->sleep.start->off();
    swc->sleep.start->on(); // never returns from there
    //FYI pmu->ctrl.digital.power->off->on(); // rst:0x1 (POWERON),boot:0x8 (SPI_FAST_FLASH_BOOT) then rst:0x12 (SUPER_WDT_RST),boot:0x8 (SPI_FAST_FLASH_BOOT)
    
    // Something went wrong
    printf( "Enter to hibernate aborted\n\n" );
    delete mcu;
    printf("Exit Xtensa program\n");
}

// static void printf( const PowerManagementUnit::Switch::SlowMemory* sm )
// {
//     printf( "Slow memory:  PU=%1d PD=%1d  RU=%1d RD=%1d  IU=%1d ID=%1d  SLP=%1d  CPU=%1d  \n",
//         sm->power->on->get(),
//         sm->power->off->get(),
//         sm->retain->on->get(),
//         sm->retain->off->get(),
//         sm->isolation->on->get(),
//         sm->isolation->off->get(),
//         sm->sleepDn->get(),
//         sm->followCPU->get()
//         );
// }

/* sleep_modes.c */
static void uart_tx_wait_idle(uint8_t uart_no) {
    uint32_t status;
    do {
        status = READ_PERI_REG(UART_STATUS_REG(uart_no));
        /* either tx count or state is non-zero */
    } while ((status & (UART_ST_UTX_OUT_M | UART_TXFIFO_CNT_M)) != 0);
}
