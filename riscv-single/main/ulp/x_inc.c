#include <stdint.h>
// #include "ulp_riscv/ulp_riscv.h"
// #include "ulp_riscv/ulp_riscv_utils.h"

/* this variable will be exported as a public symbol, visible from main CPU: */
// volatile uint32_t edge_count;
volatile unsigned int edge_count;

int main (void) //void entry()
{
    edge_count += 33;
    /* ulp_riscv_shutdown() is called automatically when main exits */
    return 0;
}
