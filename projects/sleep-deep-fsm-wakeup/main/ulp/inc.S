/* ULP FSM Example: manual (software) start of processor.

   This file contains assembly code which runs on the ULP.

   ULP wakes up to run this code on a request. To make sure it really runa,
   an ASM program increments public variable.

   Author © 2021 Nikolai Varankine
*/

	/* Define variables, which go into .bss section (zero-initialized data) */
	.bss

	/* Total number of signal edges acquired */
	.global edge_count
edge_count:
	.long 0

	/* Code goes into .text section */
	.text
	
	.global entry
entry:
	/* edge_count += 1 Increment edge_count */
	move r3, edge_count // r3 = edge_count
	ld r2, r3, 0        // r2 = r3[0]
	add r2, r2, 1       // r2 += 1
	st r2, r3, 0        // r3[0] = r2

	/* if( ( edge_count & 0xff ) == 0 ) goto wake_up */
	and r2, r2, 0xFF       // r2 &= 0xFF
	jump wake_up, eq      // if r2 == 0 goto wake_up
	
	/* that's all */
	halt
