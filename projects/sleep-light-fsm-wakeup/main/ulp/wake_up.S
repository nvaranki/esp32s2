/* ULP assembly files are passed through C preprocessor first, so include directives
   and C macros may be used in these files 
 */
#include "soc/rtc_cntl_reg.h"
#include "soc/soc_ulp.h"

	/* Define variables, which go into .bss section (zero-initialized data) */
	.bss

	/* Total number of signal edges acquired */
	.global wake_count
wake_count:
	.long 0

	/* Code goes into .text section */
	.text
	
	.global wake_up
wake_up:
	/* Check if the system can be woken up */
	READ_RTC_FIELD(RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_RDY_FOR_WAKEUP)
	and r0, r0, 1
	jump wake_up, eq

	/* wake_count += 1 Increment wake_count */
	move r3, wake_count // r3 = wake_count
	ld r2, r3, 0        // r2 = r3[0]
	add r2, r2, 1       // r2 += 1
	st r2, r3, 0        // r3[0] = r2
	
	/* Wake up the SoC, end program */
	wake
	halt
