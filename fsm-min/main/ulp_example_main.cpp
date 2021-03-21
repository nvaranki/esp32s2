/* ULP Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include "esp_sleep.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "soc/rtc_periph.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "esp32/ulp.h"
#include "mcu.h"
#include "ulp_main.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "BinaryImageULP.hpp"

extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");

static void init_ulp_program(void);
static void update_pulse_count(void);

extern "C" 
void app_main( void )
{
    MicroControllerUnit* mcu = new MicroControllerUnit();
    CoprocessorULP* ulp = mcu->getCoprocessorULP();
    CoreFSM* fsm = ulp->getCoreFSM();
    BinaryImageULP image( ulp_main_bin_start, ulp_main_bin_end - ulp_main_bin_start );
    esp_err_t e = image.loadAt( RTC_SLOW_MEM );
    if( e != ESP_OK )
    {
        printf("Failed to load ULP program (%x), aborting...\n", e);
        delete mcu;
        return;
    }
    ulp->entry->set( &ulp_entry - RTC_SLOW_MEM );

    ulp_edge_count = 500; // not earlier than program is loaded 

    ulp->setConfig( CoprocessorULP::ConfigCore::CORE, static_cast<bool>( CoprocessorULP::Core::FSM ) );

    printf("MemoryAddressSize: %d\n", fsm->getMemoryAddressSize() );
    printf("MemoryAddressInit: %d\n", fsm->getMemoryAddressInit() );
    // ulp->getTimerULP()->setConfig( TimerULP::Configuration::TIMER, false );
    // ulp->getTimerULP()->setSleepCycles( 20 ); //TODO ?
    // ulp->getTimerULP()->setConfig( TimerULP::Configuration::TIMER, true );
    fsm->setConfig( CoreFSM::Configuration::STARTH, true );
    fsm->setConfig( CoreFSM::Configuration::CLKFO, true );
   
    const TickType_t delay = 1000 / portTICK_PERIOD_MS;
    printf("Delay %d task ticks\n", delay);
    for( int i = 0; i < 6; i++ ) 
    {
        printf("Edge count %3d from ULP: %10d\n", i, ulp_edge_count);
        ulp_edge_count += 1000;
        fsm->start(); // single pass run should finish within delay
        vTaskDelay( delay );
    }

    ulp->getTimerULP()->setConfig( TimerULP::Configuration::TIMER, false );
    delete mcu;
    printf("Exit main program\n");
}

void xxx_app_main( void )
{
    MicroControllerUnit* mcu = new MicroControllerUnit();
    PowerManagementUnit* pmu = mcu->getPowerManagementUnit();
    SleepAndWakeupController* swc = pmu->getSleepAndWakeupController();
 
    if( swc->isSleepToWakeupCause( SleepAndWakeupController::Peripherals::FSM ) ) 
    {
        printf("FSM ULP wakeup, saving pulse count\n");
        update_pulse_count();
    } 
    else if( swc->isSleepToWakeupCause( SleepAndWakeupController::Peripherals::TIMER ) ) 
    {
        printf("TIMER wakeup, do nothing\n");
    } 
    else 
    {
        printf("Not an ULP wakeup, initializing ULP\n");
        init_ulp_program();
    }

    printf("Entering deep sleep\n\n");
    swc->setWakeupEnabled( SleepAndWakeupController::WakeupEnable::FSM, true );
    swc->setWakeupEnabled( SleepAndWakeupController::WakeupEnable::TIMER, true );
//TODO NV    
    // ESP_ERROR_CHECK( esp_sleep_enable_ulp_wakeup() );
    ESP_ERROR_CHECK( esp_sleep_enable_timer_wakeup(0x000000001000000) );
    esp_deep_sleep_start();

    delete swc;
    delete mcu;
}

static void init_ulp_program(void)
{
    esp_err_t err = ulp_load_binary(0, ulp_main_bin_start,
            (ulp_main_bin_end - ulp_main_bin_start) / sizeof(uint32_t));
    ESP_ERROR_CHECK(err);

    /* GPIO used for pulse counting. */
    gpio_num_t gpio_num = GPIO_NUM_0;
    int rtcio_num = rtc_io_number_get(gpio_num);
    assert(rtc_gpio_is_valid_gpio(gpio_num) && "GPIO used for pulse counting must be an RTC IO");

    /* Initialize some variables used by ULP program.
     * Each 'ulp_xyz' variable corresponds to 'xyz' variable in the ULP program.
     * These variables are declared in an auto generated header file,
     * 'ulp_main.h', name of this file is defined in component.mk as ULP_APP_NAME.
     * These variables are located in RTC_SLOW_MEM and can be accessed both by the
     * ULP and the main CPUs.
     *
     * Note that the ULP reads only the lower 16 bits of these variables.
     */
    ulp_debounce_counter = 3;
    ulp_debounce_max_count = 3;
    ulp_next_edge = 0;
    ulp_io_number = rtcio_num; /* map from GPIO# to RTC_IO# */
    ulp_edge_count_to_wake_up = 10;

    /* Initialize selected GPIO as RTC IO, enable input, disable pullup and pulldown */
    rtc_gpio_init(gpio_num);
    rtc_gpio_set_direction(gpio_num, RTC_GPIO_MODE_INPUT_ONLY);
    rtc_gpio_pulldown_dis(gpio_num);
    rtc_gpio_pullup_dis(gpio_num);
    rtc_gpio_hold_en(gpio_num);

    /* Disconnect GPIO12 and GPIO15 to remove current drain through
     * pullup/pulldown resistors.
     * GPIO12 may be pulled high to select flash voltage.
     */
    rtc_gpio_isolate(GPIO_NUM_12);
    rtc_gpio_isolate(GPIO_NUM_15);
 //TODO NV   esp_deep_sleep_disable_rom_logging(); // suppress boot messages

    /* Set ULP wake up period to T = 20ms.
     * Minimum pulse width has to be T * (ulp_debounce_counter + 1) = 80ms.
     */
    ulp_set_wakeup_period(0, 20000);

    /* Start the program */
    err = ulp_run(&ulp_entry - RTC_SLOW_MEM);
    ESP_ERROR_CHECK(err);
}

static void update_pulse_count(void)
{
    const char* const count_name_space = "plusecnt";
    const char* const count_key = "count";

    ESP_ERROR_CHECK( nvs_flash_init() );
    nvs_handle_t handle;
    ESP_ERROR_CHECK( nvs_open(count_name_space, NVS_READWRITE, &handle));
    uint32_t pulse_count = 0;
    esp_err_t err = nvs_get_u32(handle, count_key, &pulse_count);
    assert(err == ESP_OK || err == ESP_ERR_NVS_NOT_FOUND);
    printf("Read pulse count from NVS: %5d\n", pulse_count);

    /* ULP program counts signal edges, convert that to the number of pulses */
    //ulp_edge_count = 99;
    uint32_t pulse_count_from_ulp = (ulp_edge_count & UINT16_MAX) / 2;
    /* In case of an odd number of edges, keep one until next time */
    ulp_edge_count = ulp_edge_count % 2;
    printf("Pulse count from ULP: %5d\n", pulse_count_from_ulp);

    /* Save the new pulse count to NVS */
    pulse_count += pulse_count_from_ulp;
    ESP_ERROR_CHECK(nvs_set_u32(handle, count_key, pulse_count));
    ESP_ERROR_CHECK(nvs_commit(handle));
    nvs_close(handle);
    printf("Wrote updated pulse count to NVS: %5d\n", pulse_count);
}
