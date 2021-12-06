#ifndef H_ControllerRegisters
#define H_ControllerRegisters

// User-defined control registers
#define SPI_CMD_REG_A       0x0000 /* Command control register R/W */
#define SPI_ADDR_REG_A      0x0004 /* Address value R/W */
#define SPI_USER0_REG_A     0x0018 /* SPI USER control register R/W */
#define SPI_USER1_REG_A     0x001C /* SPI USER control register 1 R/W */
#define SPI_USER2_REG_A     0x0020 /* SPI USER control register 2 R/W */
#define SPI_MOSI_DLEN_REG_A 0x0024 /* MOSI length R/W */
#define SPI_MISO_DLEN_REG_A 0x0028 /* MISO length R/W */

// Control and configuration registers
#define SPI_CTRL0_REG_A 0x0008 /* SPI control register R/W */
#define SPI_CTRL1_REG_A 0x000C /* SPI control register 1 R/W */
#define SPI_CTRL2_REG_A 0x0010 /* SPI control register 2 R/W */
#define SPI_CLOCK_REG_A 0x0014 /* SPI clock control register R/W */
#define SPI_MISC_REG_A  0x002C /* SPI MISC register R/W */
#define SPI_FSM_REG_A   0x0044 /* SPI master status and DMA read byte control register varies */
#define SPI_HOLD_REG_A  0x0048 /* SPI hold register R/W */

// Slave mode configuration registers
#define SPI_SLAVE0_REG_A 0x0030 /* SPI slave control register varies */
#define SPI_SLAVE1_REG_A 0x0034 /* SPI slave control register 1 varies */
#define SPI_SLV_WRBUF_DLEN_REG_A 0x0038 /* SPI slave Wr_BUF interrupt and CONF control register R/W */
#define SPI_SLV_RDBUF_DLEN_REG_A 0x003C /* SPI magic error and slave control register R/W */
#define SPI_SLV_RD_BYTE_REG_A    0x0040 /* SPI interrupt control register R/W */
/*
DMA configuration registers
SPI_DMA_CONF_REG SPI DMA control register 0x004C R/W
SPI_DMA_OUT_LINK_REG SPI DMA TX link configuration 0x0050 R/W
SPI_IN_ERR_EOF_DES_ADDR_REG The latest SPI DMA RX descriptor address receiving error 0x0068 RO
SPI_IN_SUC_EOF_DES_ADDR_REG The latest SPI DMA EOF RX descriptor address 0x006C RO
SPI_INLINK_DSCR_REG Current SPI DMA RX descriptor pointer 0x0070 RO
SPI_INLINK_DSCR_BF0_REG Next SPI DMA RX descriptor pointer 0x0074 RO
SPI_OUT_EOF_BFR_DES_ADDR_REG The latest SPI DMA EOF TX buffer address 0x007C RO
SPI_OUT_EOF_DES_ADDR_REG The latest SPI DMA EOF TX descriptor address 0x0080 RO
SPI_OUTLINK_DSCR_REG Current SPI DMA TX descriptor pointer 0x0084 RO
SPI_OUTLINK_DSCR_BF0_REG Next SPI DMA TX descriptor pointer 0x0088 RO
SPI_DMA_OUTSTATUS_REG SPI DMA TX status 0x0090 RO
SPI_DMA_INSTATUS_REG SPI DMA RX status 0x0094 RO

DMA interrupt registers
SPI_DMA_IN_LINK_REG SPI DMA RX link configuration 0x0054 R/W
SPI_DMA_INT_ENA_REG SPI DMA interrupt enable register 0x0058 R/W
SPI_DMA_INT_RAW_REG SPI DMA interrupt raw register 0x005C varies
SPI_DMA_INT_ST_REG SPI DMA interrupt status register 0x0060 varies
SPI_DMA_INT_CLR_REG SPI DMA interrupt clear register 0x0064 R/W
*/
//CPU controlled data buffer
#define SPI_W0_REG_A 0x0098 /* Data buffer 0 R/W */
/*
SPI_W1_REG Data buffer 1 0x009C R/W
SPI_W2_REG Data buffer 2 0x00A0 R/W
SPI_W3_REG Data buffer 3 0x00A4 R/W
SPI_W4_REG Data buffer 4 0x00A8 R/W
SPI_W5_REG Data buffer 5 0x00AC R/W
SPI_W6_REG Data buffer 6 0x00B0 R/W
SPI_W7_REG Data buffer 7 0x00B4 R/W
SPI_W8_REG Data buffer 8 0x00B8 R/W
SPI_W9_REG Data buffer 9 0x00BC R/W
SPI_W10_REG Data buffer 10 0x00C0 R/W
SPI_W11_REG Data buffer 11 0x00C4 R/W
SPI_W12_REG Data buffer 12 0x00C8 R/W
SPI_W13_REG Data buffer 13 0x00CC R/W
SPI_W14_REG Data buffer 14 0x00D0 R/W
SPI_W15_REG Data buffer 15 0x00D4 R/W
SPI_W16_REG Data buffer 16 0x00D8 R/W
SPI_W17_REG Data buffer 17 0x00DC R/W
*/
// Timing registers
#define SPI_DIN_MODE_REG_A  0x00E0 /* SPI input delay mode configuration R/W */
#define SPI_DIN_NUM_REG_A   0x00E4 /* SPI input delay number configuration R/W */
#define SPI_DOUT_MODE_REG_A 0x00E8 /* SPI output delay mode configuration R/W */
#define SPI_DOUT_NUM_REG_A  0x00EC /* SPI output delay number configuration R/W */

// LCD control registers
#define SPI_LCD_CTRL0_REG_A  0x00F0 /* LCD frame control register 0 R/W */
#define SPI_LCD_CTRL1_REG_A  0x00F4 /* LCD frame control register 1 R/W */
#define SPI_LCD_CTRL2_REG_A  0x00F8 /* LCD frame control register 2 R/W */
#define SPI_LCD_D_MODE_REG_A 0x00FC /* LCD delay number R/W */
#define SPI_LCD_D_NUM_REG_A  0x0100 /* LCD delay mode R/W */

// Version register
#define SPI_DATE_REG_A 0x03FC /* Version control register R/W */


#endif
