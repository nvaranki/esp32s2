/* PMU Example: Turn on and off controllable components.

   The program ...

   Author © 2021 Nikolai Varankine
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mcu.h"

void reg( const PowerManagementUnit* pmu )
{
    printf( "\n" );
    printf( "reg.rtc: PU=%1d PD=%1d BS=%1d BW=%1d\n", 
        pmu->reg.rtc.powerUp->get(), pmu->reg.rtc.powerDn->get(),
        pmu->reg.rtc.biasSleep->get(), pmu->reg.rtc.biasWakeup->get() );
}

void printf( const PowerManagementUnit::Switch::Digital* digi )
{
    printf( "digi: ISO=%1d ISO_ON=%1d ISO_OFF=%1d  P=%1d FPU=%1d FPD=%1d  SLP=%1d  MUS=%1d MDS=%1d\n", 
        digi->isolation->state->get(), digi->isolation->on->get(), digi->isolation->off->get(),
        digi->power->state->get(), digi->power->on->get(), digi->power->off->get(),
        digi->sleepDn->get(), 
        digi->memory->on->get(), digi->memory->off->get() );
}
void digi( const PowerManagementUnit::Switch::Digital* digi )
{
    printf( "\n" );
    printf( digi );

    // // commented out because terminates the test
    // digi->isolation->off->set( false );
    // digi->isolation->on->set( true );
    // printf( digi );

    digi->isolation->on->set( false );
    digi->isolation->off->set( true );
    printf( digi );

    // // commented out because leads to rst:0x1 (POWERON)
    // fflush( stdout );
    // vTaskDelay( 10 ); // to prevent corruption of printf()
    // digi->power->on->set( false );
    // digi->power->off->set( true );
    // printf( digi );

    digi->power->off->set( false );
    digi->power->on->set( true );
    printf( digi );

    fflush( stdout );
    vTaskDelay( 10 ); // to prevent corruption of printf()
    digi->memory->on->set( false );
    digi->memory->off->set( true );
    vTaskDelay( 10 );
    printf( digi ); // no print happens

    fflush( stdout );
    vTaskDelay( 10 ); // to prevent corruption of printf()
    digi->memory->off->set( false );
    digi->memory->on->set( true );
    vTaskDelay( 10 );
    printf( digi );
}

void printf( const PowerManagementUnit::Switch::FastMemory* fmem )
{
    printf( "fmem: ISO_ON=%1d ISO_OFF=%1d  FPU=%1d FPD=%1d  SLP=%1d  RU=%1d RD=%1d  FLW=%1d\n", 
        fmem->isolation->on->get(), fmem->isolation->off->get(),
        fmem->power->on->get(), fmem->power->off->get(),
        fmem->sleepDn->get(), 
        fmem->retain->on->get(), fmem->retain->off->get(), 
        fmem->followCPU->get() );
}
void fmem( const PowerManagementUnit::Switch::FastMemory* fmem )
{
    printf( "\n" );
    printf( fmem );

    fmem->isolation->off->set( false );
    fmem->isolation->on->set( true );
    printf( fmem );

    fmem->isolation->on->set( false );
    fmem->isolation->off->set( true );
    printf( fmem );

    fmem->power->on->set( false );
    fmem->power->off->set( true );
    printf( fmem );

    fmem->power->off->set( false );
    fmem->power->on->set( true );
    printf( fmem );

    fmem->retain->on->set( false );
    fmem->retain->off->set( true );
    printf( fmem );

    fmem->retain->off->set( false );
    fmem->retain->on->set( true );
    printf( fmem );
}

void printf( const PowerManagementUnit::Switch::SlowMemory* smem )
{
    printf( "smem: ISO_ON=%1d ISO_OFF=%1d  FPU=%1d FPD=%1d  SLP=%1d  RU=%1d RD=%1d  FLW=%1d\n", 
        smem->isolation->on->get(), smem->isolation->off->get(),
        smem->power->on->get(), smem->power->off->get(),
        smem->sleepDn->get(), 
        smem->retain->on->get(), smem->retain->off->get(), 
        smem->followCPU->get() );
}
void smem( const PowerManagementUnit::Switch::SlowMemory* smem )
{
    printf( "\n" );
    printf( smem );

    smem->isolation->off->set( false );
    smem->isolation->on->set( true );
    printf( smem );

    smem->isolation->on->set( false );
    smem->isolation->off->set( true );
    printf( smem );

    smem->power->on->set( false );
    smem->power->off->set( true );
    printf( smem );

    smem->power->off->set( false );
    smem->power->on->set( true );
    printf( smem );

    smem->retain->on->set( false );
    smem->retain->off->set( true );
    printf( smem );

    smem->retain->off->set( false );
    smem->retain->on->set( true );
    printf( smem );
}

void printf( const PowerManagementUnit::Switch::Peripherals* peri )
{
    printf( "peri: ISO=%1d ISO_ON=%1d ISO_OFF=%1d  P=%1d FPU=%1d FPD=%1d  SLP=%1d  HOLD=%1d\n", 
        peri->isolation->state->get(), peri->isolation->on->get(), peri->isolation->off->get(),
        peri->power->state->get(), peri->power->on->get(), peri->power->off->get(),
        peri->sleepDn->get(), peri->holdGPIO->get() );
}
void peri( const PowerManagementUnit::Switch::Peripherals* peri )
{
    printf( "\n" );
    printf( peri );

    // commented out because terminates USB link to ESP-IDF
    // peri->isolation->off->set( false );
    // peri->isolation->on->set( true );
    // printf( peri );

    peri->isolation->on->set( false );
    peri->isolation->off->set( true );
    printf( peri );

    // commented out because terminates USB link to ESP-IDF
    // peri->power->on->set( false );
    // peri->power->off->set( true );
    // printf( peri );

    peri->power->off->set( false );
    peri->power->on->set( true );
    printf( peri );
}

void printf( const PowerManagementUnit::Switch::DigitalGPIO* gpio, bool had )
{
    printf( "gpio: ISO_ON=%1d ISO_OFF=%1d  HU=%1d HD=%1d  HA=%1d HAU=%1d HAD=%1d\n", 
        gpio->isolation->on->get(), gpio->isolation->off->get(),
        gpio->hold->on->get(), gpio->hold->off->get(),
        gpio->holdAuto->get(), gpio->holdAutoOn->get(), had );
}
void gpio( const PowerManagementUnit::Switch::DigitalGPIO* gpio )
{
    printf( "\n" );
    printf( gpio, false );

    // commented out because test terminates
    // gpio->isolation->off->set( false );
    // gpio->isolation->on->set( true );
    // printf( gpio, false );

    gpio->isolation->on->set( false );
    gpio->isolation->off->set( true );
    printf( gpio, false );

    gpio->hold->on->set( false );
    gpio->hold->off->set( true );
    printf( gpio, false );

    // commented out because rst:0x8 (TG1WDT_SYS_RST)
    // gpio->hold->off->set( false );
    // gpio->hold->on->set( true );
    // printf( gpio, false );

    gpio->holdAutoOff->set( false );
    gpio->holdAutoOff->set( true );
    printf( gpio, true );

    gpio->holdAutoOff->set( false );
    printf( gpio, false );
}

void printf( const PowerManagementUnit::Switch::WiFi* wifi )
{
    printf( "wifi: ISO=%1d ISO_ON=%1d ISO_OFF=%1d  P=%1d FPU=%1d FPD=%1d  SLP=%1d\n", 
        wifi->isolation->state->get(), wifi->isolation->on->get(), wifi->isolation->off->get(),
        wifi->power->state->get(), wifi->power->on->get(), wifi->power->off->get(),
        wifi->sleepDn->get() );
}
void wifi( const PowerManagementUnit::Switch::WiFi* wifi )
{
    printf( "\n" );
    printf( wifi );

    wifi->isolation->off->set( false );
    wifi->isolation->on->set( true );
    printf( wifi );

    wifi->isolation->on->set( false );
    wifi->isolation->off->set( true );
    printf( wifi );

    wifi->power->on->set( false );
    wifi->power->off->set( true );
    printf( wifi );

    wifi->power->off->set( false );
    wifi->power->on->set( true );
    printf( wifi );
}

/**
 * Standard ESP-IDF main entry point, executed on every chip reset.
 */
extern "C" 
void app_main( void )
{
    MicroControllerUnit* const mcu = new MicroControllerUnit();
    PowerManagementUnit* const pmu = mcu->getPowerManagementUnit();

    reg( pmu );
    digi( &pmu->ctrl.digital );
    fmem( &pmu->ctrl.fastMemory );
    smem( &pmu->ctrl.slowMemory );
    peri( &pmu->ctrl.peripherals );
    gpio( &pmu->ctrl.digitalGPIO );
    wifi( &pmu->ctrl.wifi );

    delete mcu;
}
