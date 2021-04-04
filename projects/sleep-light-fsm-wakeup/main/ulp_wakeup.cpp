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
#include "hal/clk_gate_ll.h"
#include "soc/soc.h"
#include "soc/periph_defs.h"
#include "soc/uart_reg.h"
#include "esp_sleep.h"
#include "ulp_main.h"
#include "mcu.h"
#include "BinaryImageULP.hpp"
#include "SleepAndWakeupController.hpp"

static void IRAM_ATTR suspend_uart(int i);
static void IRAM_ATTR resume_uart(int i);

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
        // OOps. Stop both programs
        printf( "\nULP FSM wakeup on system reset\n" );
        printf( "Edge count: %10d     Wake count: %3d\n", ulp_edge_count, ulp_wake_count );
        printf("Cancelling FSM program...\n");
        tmr->active->off();
        fsm->clockOn->off();
        printf("Abnormal exit Xtensa program\n");
        return;
    } 
    else if( ( cause = swc->wakeup.cause->getAll() ) == 0 )
    {
        printf( "\nMCU reset 0x%02x\n", (uint32_t) cpu->getResetCause() );

        // cancel next timer run if any
        tmr->active->off(); 

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
        fsm->startOn->off(); // block software start of ULP
        fsm->clockOn->on();
        fsm->clockOff->off();

        // allow auto PD for clocks only, see Table 195: Predefined Power Modes
        pmu->ctrl.wifi.sleepDn->off();
        pmu->ctrl.digital.sleepDn->off();
        //TODO how clocks OFF?
        pmu->ctrl.slowMemory.sleepDn->off();
        pmu->ctrl.fastMemory.sleepDn->off();
        pmu->ctrl.peripherals.sleepDn->off();

        // ensure power ON for RTC slow memory where FSM program resides
        pmu->ctrl.slowMemory.power->on->on();
        pmu->ctrl.slowMemory.power->off->off();
        pmu->ctrl.slowMemory.isolation->off->on();

        // allow Xtensa wakeup by FSM
        swc->wakeup.enable->setAll( false );
        swc->wakeup.setEnabled( SleepAndWakeupController::Peripherals::FSM, true );

        // start ULP timer
        tmr->sleep->set( 50 ); //TODO 75+: rst:0x8 (TG1WDT_SYS_RST),boot:0x8 (SPI_FAST_FLASH_BOOT)
        tmr->active->on();
    }
    else
    {
        printf( "\nUnexpected wakeup cause %08x\n", cause );
    }
    
    while( ulp_wake_count < 6 )
    {
        // Get ready to sleep
        printf( "Ready to sleep\n\n" );
        fflush(stdout);
        // suspend_uart(CONFIG_ESP_CONSOLE_UART_NUM);
        vTaskDelay( 20 );
        
        // enter light sleep
        swc->sleep.start->off();
        swc->sleep.start->on(); // returns after wakeup from there

        // wait here until wakeup
        // resume_uart(CONFIG_ESP_CONSOLE_UART_NUM);
        while (GET_PERI_REG_MASK(RTC_CNTL_INT_RAW_REG,
                             RTC_CNTL_SLP_REJECT_INT_RAW | RTC_CNTL_SLP_WAKEUP_INT_RAW) == 0) {
            ;
        }

        if( swc->wakeup.isCause( SleepAndWakeupController::Peripherals::FSM ) ) 
        {
            printf( "\nULP FSM wakeup from sleep\n" );
            printf( "Edge count: %10d     Wake count: %3d\n", ulp_edge_count, ulp_wake_count );
        }
        else
        {
            printf( "\nUnexpected wakeup cause %08x\n", cause );
            printf( "Edge count: %10d     Wake count: %3d\n", ulp_edge_count, ulp_wake_count );
        }
    }

    // Done. Stop both programs
    printf("Cancelling FSM program...\n");
    tmr->active->off();
    fsm->clockOn->off();
    printf("Final exit Xtensa program\n");
}

/* sleep_modes.c */
static void IRAM_ATTR suspend_uart(int i)
{
    if (periph_ll_periph_enabled((periph_module_t)((int)PERIPH_UART0_MODULE + i))) {
        REG_CLR_BIT(UART_FLOW_CONF_REG(i), UART_FORCE_XON);
        REG_SET_BIT(UART_FLOW_CONF_REG(i), UART_SW_FLOW_CON_EN | UART_FORCE_XOFF);
        while (REG_GET_FIELD(UART_FSM_STATUS_REG(i), UART_ST_UTX_OUT) != 0) {
            ;
        }
    }
}

static void IRAM_ATTR resume_uart(int i)
{
    if (periph_ll_periph_enabled((periph_module_t)((int)PERIPH_UART0_MODULE + i))) {
        REG_CLR_BIT(UART_FLOW_CONF_REG(i), UART_FORCE_XOFF);
        REG_SET_BIT(UART_FLOW_CONF_REG(i), UART_FORCE_XON);
        REG_CLR_BIT(UART_FLOW_CONF_REG(i), UART_SW_FLOW_CON_EN | UART_FORCE_XON);
    }
}
