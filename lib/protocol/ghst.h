#ifndef __GHST_H_
#define __GHST_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#define PPM_CENTER                     1500
#define PPM_CH_CENTER(ch)            (PPM_CENTER)

// Device (destination) address
#define GHST_ADDR_RADIO                 0x80    // phase 1
#define GHST_ADDR_MODULE_SYM            0x81    // symmetrical, 400k pulses, 400k telemetry
#define GHST_ADDR_MODULE_ASYM           0x88    // asymmetrical, 400k pulses, 115k telemetry
#define GHST_ADDR_FC                    0x82
#define GHST_ADDR_GOGGLES               0x83    // phase 2
#define GHST_ADDR_5G_TXCTRL             0x84    // phase 3
#define GHST_ADDR_5G_TWRSCAN            0x85
#define GHST_ADDR_5G_RLY                0x86

#define GHST_UL_RC_CHANS_HS4_5TO8       0x10  // High Speed 4 channel (12 bit legacy), plus CH5-8 (8 bits)
#define GHST_UL_RC_CHANS_HS4_9TO12      0x11  // High Speed 4 channel (12 bit legacy), plus CH9-12 (8 bits)
#define GHST_UL_RC_CHANS_HS4_13TO16     0x12  // High Speed 4 channel (12 bit legacy), plus CH13-16 (8 bits)
#define GHST_UL_RC_CHANS_SIZE           12    // 1 (type) + 10 (data) + 1 (crc)
#define GHST_UL_MENU_CTRL               0x13

#define GHST_UL_RC_CHANS_HS4_12_5TO8    0x30  // High Speed 4 channel (12 bit raw), plus CH5-8 (8 bit raw)
#define GHST_UL_RC_CHANS_HS4_12_9TO12   0x31  // High Speed 4 channel (12 bit raw), plus CH9-12 (8 bit raw)
#define GHST_UL_RC_CHANS_HS4_12_13TO16  0x32  // High Speed 4 channel (12 bit raw), plus CH13-16 (8 bit raw)

#define GHST_DL_OPENTX_SYNC             0x20
#define GHST_DL_LINK_STAT               0x21
#define GHST_DL_VTX_STAT                0x22
#define GHST_DL_PACK_STAT               0x23
#define GHST_DL_MENU_DESC               0x24
#define GHST_DL_GPS_PRIMARY             0x25
#define GHST_DL_GPS_SECONDARY           0x26
#define GHST_DL_MAGBARO                 0x27
#define GHST_DL_MSP_RESP                0x28

#define GHST_RC_CTR_VAL_12BIT           0x7C0   // 0x3e0 << 1
#define GHST_RC_CTR_VAL_8BIT            0x7C

#define GHST_CH_BITS_12                 12
#define GHST_CH_BITS_8                  8

void GHST_SetBind(void);
void GHST_Init(uint8_t protocolIndex);
uint16_t GHST_Process(uint16_t *controlData);
#endif
