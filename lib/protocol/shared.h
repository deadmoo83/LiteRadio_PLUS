#ifndef __SHARED_H_
#define __SHARED_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#define FIRST_FLASH_MARK_ADDR           0x8007A00

#define LITE_RADIO_HARDWARE_TYPE_ADDR   0x8007A02
#define INTERNAL_RADIO_TYPE_ADDR        0x8007A04
#define THROTTLE_ROCKER_POSITION_ADDR   0x8007A06
#define HARDWARE_MAJOR_VERSION_ADDR     0x8007A08
#define HARDWARE_MINOR_VERSION_ADDR     0x8007A10
#define HARDWARE_PATCH_VERSION_ADDR     0x8007A12

#define FIRMWARE_MAJOR_VERSION (0x02U)
#define FIRMWARE_MINOR_VERSION (0x00U)
#define FIRMWARE_PITCH_VERSION (0x02U)
typedef struct
{
    uint8_t MAJOR_VERSION;
    uint8_t MINOR_VERSION;
    uint8_t PITCH_VERSIO;
}Hardware_Version_t;

#define INTERNAL_ELRS_CONFIGER_INFO_ADDR           0x08007068
#define INTERNAL_ELRS_CONFIGER_INFO_POWER_ADDR     0x08007068
#define INTERNAL_ELRS_CONFIGER_INFO_Rate_ADDR      0x0800706A
#define INTERNAL_ELRS_CONFIGER_INFO_TLM_ADDR       0x0800706C

typedef struct {
    uint32_t    version;
    uint32_t    lastRate;
    uint32_t    rate;
    uint32_t    lastTLM;
    uint32_t    tlm;
    uint32_t    lastPower;
    uint32_t    power;
    uint8_t     modify;
} tx_config_t;

extern tx_config_t tx_config;

typedef enum
{
    FREQ_2400_RATE_500HZ = 0,
    FREQ_2400_RATE_250HZ = 1,
    FREQ_2400_RATE_150HZ = 2,
    FREQ_2400_RATE_50HZ = 3,
} expresslrs_RF_FREQ_2400_rates_e; 

typedef enum
{
    FREQ_900_RATE_200HZ = 0,
    FREQ_900_RATE_100HZ = 1,
    FREQ_900_RATE_50HZ = 2,
    FREQ_900_RATE_25HZ = 3,
} expresslrs_RF_FREQ_900_rates_e; 

#endif