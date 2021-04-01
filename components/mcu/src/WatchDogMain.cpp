#include "soc/rtc_cntl_reg.h"
#include "WatchDogMain.hpp"

WatchDogMain::WatchDogMain( const uint8_t tg ) :
    stage0( new Stage( TIMG_WDTCONFIG2_REG( tg ), TIMG_WDTCONFIG0_REG( tg ), TIMG_WDT_STG0_M, TIMG_WDT_STG0_S, 0 ) ),
    stage1( new Stage( TIMG_WDTCONFIG3_REG( tg ), TIMG_WDTCONFIG0_REG( tg ), TIMG_WDT_STG1_M, TIMG_WDT_STG1_S, 1 ) ),
    stage2( new Stage( TIMG_WDTCONFIG4_REG( tg ), TIMG_WDTCONFIG0_REG( tg ), TIMG_WDT_STG2_M, TIMG_WDT_STG2_S, 2 ) ),
    stage3( new Stage( TIMG_WDTCONFIG5_REG( tg ), TIMG_WDTCONFIG0_REG( tg ), TIMG_WDT_STG3_M, TIMG_WDT_STG3_S, 3 ) ),
    interruptLevel( new FlagRW( TIMG_WDTCONFIG0_REG( tg ), TIMG_WDT_LEVEL_INT_EN_S ) ),
    interruptEdge ( new FlagRW( TIMG_WDTCONFIG0_REG( tg ), TIMG_WDT_EDGE_INT_EN_S ) ),
    enable( new FlagRW( TIMG_WDTCONFIG0_REG( tg ), TIMG_WDT_EN_S ) ),
    protectFlashBoot( new FlagRW( TIMG_WDTCONFIG0_REG( tg ), TIMG_WDT_FLASHBOOT_MOD_EN_S ) ),
    resetCPU0( new FlagRW( TIMG_WDTCONFIG0_REG( tg ), TIMG_WDT_PROCPU_RESET_EN_S ) ),
//  resetCPU1( new FlagRW( TIMG_WDTCONFIG0_REG( tg ), TIMG_WDT_APPCPU_RESET_EN_S ) ),
    scale( new SubValueRW( TIMG_WDTCONFIG1_REG( tg ), TIMG_WDT_CLK_PRESCALE_M, TIMG_WDT_CLK_PRESCALE_S ) ),
    feed( new WordWO( TIMG_WDTFEED_REG( tg ) ) ),
    durationResetCPU   ( new SubValueRW( TIMG_WDTCONFIG1_REG( tg ), TIMG_WDT_CPU_RESET_LENGTH_M, TIMG_WDT_CPU_RESET_LENGTH_S ) ),
    durationResetSystem( new SubValueRW( TIMG_WDTCONFIG1_REG( tg ), TIMG_WDT_SYS_RESET_LENGTH_M, TIMG_WDT_SYS_RESET_LENGTH_S ) ),
    lock( new WordWO( TIMG_WDTWPROTECT_REG( tg ) ) ),

    interruptClear( new FlagWO( RTC_CNTL_INT_CLR_REG, RTC_CNTL_WDT_INT_CLR_S ) ),
    interruptState( new FlagRO( RTC_CNTL_INT_ST_REG, RTC_CNTL_WDT_INT_ST_S ) ),
    interruptRaw( new FlagRO( RTC_CNTL_INT_RAW_REG, RTC_CNTL_WDT_INT_RAW_S ) ),
    interruptEnable( new FlagRW( RTC_CNTL_INT_ENA_REG, RTC_CNTL_WDT_INT_ENA_S ) )
{
}
    
WatchDogMain::~WatchDogMain()
{
    delete stage0;
    delete stage1;
    delete stage2;
    delete stage3;
    delete interruptLevel;
    delete interruptEdge;
    delete enable;
    delete protectFlashBoot;
    delete resetCPU0;
//  delete resetCPU1;
    delete scale;
    delete feed;
    delete durationResetCPU;
    delete durationResetSystem;
    delete lock;
    
    delete interruptClear;
    delete interruptState;
    delete interruptRaw;
    delete interruptEnable;
}

WatchDogMain::Duration WatchDogMain::getResetDuration( Target t ) const 
{ 
    return (Duration) ( t == Target::CPU ? durationResetCPU : durationResetSystem )->get(); 
}
    
void WatchDogMain::setResetDuration( Target t, Duration d ) 
{ 
    ( t == Target::CPU ? durationResetCPU : durationResetSystem )->set( static_cast<uint32_t>( d ) ); 
}

void WatchDogMain::protect( const bool p ) 
{ 
    lock->set( p ? ~TIMG_WDT_WKEY_VALUE : TIMG_WDT_WKEY_VALUE );
}

WatchDogMain::Stage::Stage( const uint32_t timeout_r, const uint32_t action_r, 
        const uint32_t action_m, const uint8_t action_s, const uint8_t id ) :
    action( new SubValueRW( action_r, action_m, action_s ) ),
    timeout( new WordRW( timeout_r ) ),
    id( id )
{
}

WatchDogMain::Stage::~Stage()
{
    delete action;
    delete timeout;
}

// WatchDogMain::Stage::Action WatchDogMain::Stage::getAction()
// {
//     Action rc;
//     switch( action->get() ) //TODO map?
//     {
//     case static_cast<uint32_t>( Action::OFF          ) : rc = Action::OFF;          break;
//     case static_cast<uint32_t>( Action::INTERRUPT    ) : rc = Action::INTERRUPT;    break;
//     case static_cast<uint32_t>( Action::RESET_CPU    ) : rc = Action::RESET_CPU;    break;
//     case static_cast<uint32_t>( Action::RESET_SYSTEM ) : rc = Action::RESET_SYSTEM; break;
//     default: rc = *nullptr; break;
//     }
//     return static_cast<Action>( 2  );
// }
