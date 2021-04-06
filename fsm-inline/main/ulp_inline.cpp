/* ULP FSM Example: manual (software) start of processor.

   This file contains C code to run assembly program on the ULP. To emplloy C++ 
   API "mcu", the file itself is named as C++ file.

   The program wakes up an ULP FSM to run code on a request, while it is on hold. 
   To make sure the FSM really runs, an ASM program increments public variable.
   The program is loaded into slow memory with intentional offset.

   Author © 2021 Nikolai Varankine
   @see https://github.com/boarchuz/s2-ulp-fsm-demo/blob/master/main/main.c Origin
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp32s2/ulp.h"
#include "driver/rtc_io.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/rtc_io_reg.h"
//#include "ulp_main.h"
#include "mcu.h"

// // ASM program placeholders
// extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
// extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");

#define ARRAY_COUNT(x) (sizeof(x) / sizeof((x)[0]))
#define RTC_WORD_OFFSET(x) ((uint16_t)((uint32_t*)&(x) - RTC_SLOW_MEM))

RTC_DATA_ATTR uint32_t ulp_edge_count;

const ulp_insn_t program[] =
{

	/* edge_count += 33 Increment edge_count */
	I_MOVI( R3, RTC_WORD_OFFSET( ulp_edge_count ) ), // R3 = edge_count
	I_LD( R2, R3, 0 ),       // R2 = R3[0]
	I_ADDI( R2, R2, 33 ),     // R2 += 33
	I_ST( R2, R3, 0 ),       // R3[0] = R2
	
	/* that's all */
	I_HALT(),
};

/**
 * Standard ESP-IDF main entry point, executed on every chip reset.
 */
extern "C" 
void app_main( void )
{
    MicroControllerUnit* const mcu = new MicroControllerUnit();
    CoprocessorULP* const ulp = mcu->getCoprocessorULP();
    CoreFSM* const fsm = ulp->getCoreFSM();
    TimerULP* const tmr = ulp->getTimerULP();
    
    // cancel next timer run if any
    tmr->active->set( false ); 
    
    // load code
    size_t program_size = ARRAY_COUNT( program );
    ESP_ERROR_CHECK( ulp_process_macros_and_load( 0, program, &program_size ) );
    // uint32_t offset32 = 128; // 128*4=512=0x00000200
    // esp_err_t e = ulp->loadExecCode( RTC_SLOW_MEM + offset32, ulp_main_bin_start,
    //     ulp_main_bin_end - ulp_main_bin_start );
    // if( e != ESP_OK )
    // {
    //     printf("Failed to load ULP program (%x), aborting...\n", e);
    //     delete mcu;
    //     return;
    // }
    // ulp->entry->set( &ulp_entry - RTC_SLOW_MEM + offset32 );
    ulp->entry->set( 0 );
    
    // select FSM to run
    ulp->setConfig( CoprocessorULP::ConfigCore::CORE, static_cast<bool>( CoprocessorULP::Core::FSM ) );
    fsm->startOn->set( true );
    fsm->clock->on->on();
    fsm->clock->off->off();

    // init shared vars
    ulp_edge_count = 500; // not earlier than the program has been loaded 

    // go!
    fsm->start->set( false );
    fsm->start->set( true ); // 0->1 transition matters
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // single pass run should finish within delay
    printf("Edge count from ULP: %10d\n", ulp_edge_count);

    fsm->start->set( false ); // it possibly can be applied right after "true"
    fsm->clock->on->off();
    delete mcu;
    printf("Exit main program\n");
}
