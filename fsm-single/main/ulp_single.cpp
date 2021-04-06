/* ULP FSM Example: single manual (software) start of processor with program 
   loaded at shifted location.

   This file contains C code to run assembly program on the ULP. To emplloy C++ 
   API "mcu", the file itself is named as C++ file.

   The program wakes up an ULP FSM to run code on a request, while it is on hold. 
   To make sure the FSM really runs, an ASM program increments public variable.
   The program is loaded into slow memory with intentional offset.

    Expected output:
    
FSM: SLEEP=0 WORK=0 RUNS=0 DONE=1
FSM: SLEEP=0 WORK=0 RUNS=0 DONE=0
FSM: SLEEP=0 WORK=0 RUNS=0 DONE=0
FSM: SLEEP=0 WORK=0 RUNS=0 DONE=0
FSM: SLEEP=0 WORK=0 RUNS=0 DONE=1
Edge count from ULP:        533
Stop FSM clock
Exit main program

   Author © 2021 Nikolai Varankine
*/

#include <stdio.h>
#include "ulp_main.h"
#include "mcu.h"
#include "BinaryImageULP.hpp"
#include "MicroControllerUnit.hpp"
#include "CoprocessorULP.hpp"
#include "CoreFSM.hpp"
#include "PowerManagementUnit.hpp"

void printf( const PowerManagementUnit* pmu );

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
    CoprocessorULP* const ulp = mcu->getCoprocessorULP();
    CoreFSM* const fsm = ulp->getCoreFSM();
    
    // load code with intentional offset
    uint32_t offset32 = 128; // 128*4=512=0x00000200
    BinaryImageULP image( ulp_main_bin_start, ulp_main_bin_end - ulp_main_bin_start );
    esp_err_t e = image.loadAt( RTC_SLOW_MEM + offset32 );
    if( e != ESP_OK )
    {
        printf("Failed to load ULP program (%x), aborting...\n", e);
        delete mcu;
        return;
    }
    ulp->entry->set( &ulp_entry - RTC_SLOW_MEM + offset32 );

    // init shared vars
    ulp_edge_count = 500; // not earlier than the program has been loaded 
    
    // select FSM to run
    fsm->selectForExec();
    fsm->startOn->on();
    fsm->clock->on->on();
    fsm->clock->off->off();

    // go!
    printf( pmu );
    fsm->start->off();
    fsm->start->on(); // 0->1 transition matters for software start
    while( !pmu->cocpuDone->get() )
        printf( pmu ); // it may stand on long wait, so WORK=0 RUNS=0 is expected
    printf( pmu );
    printf( "Edge count from ULP: %10d\n", ulp_edge_count );

    // done
    printf("Stop FSM clock\n");
    fsm->start->off(); // it possibly can be applied right after "true"
    fsm->clock->on->off();
    delete mcu;
    printf("Exit main program\n");
}

void printf( const PowerManagementUnit* pmu )
{
    printf( "FSM: SLEEP=%1d WORK=%1d RUNS=%1d DONE=%1d\n",
        pmu->cocpuSleep->get(),
        pmu->cocpuWork->get(),
        pmu->cocpuRuns->get(),
        pmu->cocpuDone->get()
        );
}
