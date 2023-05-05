#include "ghst.h"
#include "usart.h"
#include "function.h"
#include "buzzer.h"
#include "mixes.h"
#include "status.h"
#include "common.h"

uint8_t ghstPacket[14] = {0,             // frame
                          0,             // addr
                          0,             // len
                          0, 0, 0, 0, 0, // data
                          0, 0, 0, 0, 0,
                          0};            // crc

void GHST_SetBind() { HAL_Delay(1); }

void GHST_Init(uint8_t protocolIndex) { HAL_Delay(1); }

uint32_t min32(uint32_t a, uint32_t b)
{
  return a < b ? a : b;
}

uint32_t max32(uint32_t a, uint32_t b)
{
  return a > b ? a : b;
}

uint32_t limit32(uint32_t vmin, uint32_t x, uint32_t vmax)
{
  return min32(max32(vmin, x), vmax);
}

uint8_t min8(uint8_t a, uint8_t b)
{
  return a < b ? a : b;
}

uint8_t max8(uint8_t a, uint8_t b)
{
  return a > b ? a : b;
}

uint8_t limit8(uint8_t vmin, uint8_t x, uint8_t vmax)
{
  return min8(max8(vmin, x), vmax);
}

static uint8_t getGhostModuleAddr()
{
  return GHST_ADDR_MODULE_SYM;
}

// Range for pulses (channels output) is [-1024:+1024]
static uint8_t createGhostChannelsFrame(uint8_t * frame, int16_t * pulses, uint8_t raw12bits)
{
  static uint8_t lastGhostFrameId = 0;
  uint8_t ghostUpper4Offset = 0;

  switch (lastGhostFrameId) {
    case GHST_UL_RC_CHANS_HS4_5TO8:
    case GHST_UL_RC_CHANS_HS4_12_5TO8:
      lastGhostFrameId = raw12bits ? GHST_UL_RC_CHANS_HS4_12_9TO12 : GHST_UL_RC_CHANS_HS4_9TO12;
      ghostUpper4Offset = 4;
    break;

    case GHST_UL_RC_CHANS_HS4_9TO12:
    case GHST_UL_RC_CHANS_HS4_12_9TO12:
      lastGhostFrameId = raw12bits ? GHST_UL_RC_CHANS_HS4_12_13TO16 : GHST_UL_RC_CHANS_HS4_13TO16;
      ghostUpper4Offset = 8;
      break;

    case GHST_UL_RC_CHANS_HS4_13TO16:
    case GHST_UL_RC_CHANS_HS4_12_13TO16:
      lastGhostFrameId = raw12bits ? GHST_UL_RC_CHANS_HS4_12_5TO8 : GHST_UL_RC_CHANS_HS4_5TO8;
      ghostUpper4Offset = 0;
      break;

    default:  // We don't have known previous state so init
      lastGhostFrameId = raw12bits ? GHST_UL_RC_CHANS_HS4_12_5TO8 : GHST_UL_RC_CHANS_HS4_5TO8;
      ghostUpper4Offset = 0;
    break;
  }

  uint8_t * buf = frame;
  *buf++ = getGhostModuleAddr();            // addr
  *buf++ = GHST_UL_RC_CHANS_SIZE;           // len
  uint8_t * crc_start = buf;
  *buf++ = lastGhostFrameId;                // type

  // payload
  // first 4 high speed, 12 bit channels (11 relevant bits with openTx)
  uint32_t bits = 0;
  uint8_t bitsavailable = 0;
  for (int i = 0; i < 4; i++) {
    uint32_t value;
    if (raw12bits) {
    value = limit32(
        0, (1024 + (pulses[i] + 2 * PPM_CH_CENTER(i) - 2 * PPM_CENTER)) << 1,
        0xFFF);      
    } else {
    value = limit32(
        0,
        GHST_RC_CTR_VAL_12BIT +
            (((pulses[i] + 2 * PPM_CH_CENTER(i) - 2 * PPM_CENTER) << 3) / 5),
        2 * GHST_RC_CTR_VAL_12BIT);
    }
    bits |= value << bitsavailable;
    bitsavailable += GHST_CH_BITS_12;
    while (bitsavailable >= 8) {
      *buf++ = bits;
      bits >>= 8;
      bitsavailable -= 8;
    }
  }

  // second 4 lower speed, 8 bit channels
  for (int i = 4; i < 8; ++i) {
    uint8_t channelIndex = i + ghostUpper4Offset;
    uint8_t value;
    if (raw12bits) {
      value = limit8(0,
                    128 + ((pulses[channelIndex] +
                            2 * PPM_CH_CENTER(channelIndex) - 2 * PPM_CENTER) >>
                           3),
                    0xFF);
    } else {
      value = limit8(0,
                    GHST_RC_CTR_VAL_8BIT +
                        (((pulses[channelIndex] +
                           2 * PPM_CH_CENTER(channelIndex) - 2 * PPM_CENTER) >>
                          1) /
                         5),
                    2 * GHST_RC_CTR_VAL_8BIT);
    }
    *buf++ = value;
  }

  // crc
  *buf++ = crc8(crc_start, GHST_UL_RC_CHANS_SIZE - 1);

  return buf - frame;
}

uint16_t GHST_Process(uint16_t *ghstcontrol_data) {
  (void)createGhostChannelsFrame(&ghstPacket[0], (int16_t *)ghstcontrol_data, 0);

  HAL_UART_Transmit_DMA(&huart1, ghstPacket, 14);
  
  return 0;
}
