#ifndef __STATUS_H_
#define __STATUS_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

enum{
    SX1280_ELRS,
    SX1280_CRSF,
};

enum{
    CC2500_FCC_D16,
    CC2500_LBT_D16,
    CC2500_D8,
    CC2500_FHSS,
    CC2500_CRSF,
};

#if defined(LiteRadio_Plus_CC2500) 
#define PROTOCOL_INDEX_LIMIT 4
#define CRSF_PROTOCOL_INDEX 4
#elif defined(LiteRadio_Plus_SX1280)||(LiteRadio_Plus_SX1276)
#define PROTOCOL_INDEX_LIMIT 1
#define CRSF_PROTOCOL_INDEX 1
#endif

typedef enum
{
    RC_INIT              = 0,
    RC_SHUTDOWN             ,
    RC_RADIOLINK 	        ,    
    RC_CHRG_AND_JOYSTICK    ,
}RCstatus_e;

typedef enum
{
    RF_DATA           = 0,
    RF_CALIBARATION      ,   
    RF_BIND              ,
}RFstatus_e;

extern uint8_t requestType1;
extern uint8_t requestType2;
extern uint8_t configFlag;
uint32_t Get_ProtocolDelayTime(void);
void Status_Init(void);
void Status_Update(void);
void statusTask(void* param);
uint8_t Get_ProtocolIndex(void);
#endif  
