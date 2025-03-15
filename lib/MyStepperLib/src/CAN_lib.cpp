#include "CAN_lib.h"

CanBase::CanBase(CAN_TypeDef *Instance, CAN_HandleTypeDef *hcan): 
_Instance(Instance), _hcan(hcan) 
{        
    init();
}

void CanBase::init() 
{
    if (_Instance == CAN1) {
        if(is_init[0]) {
            return;
        }
        is_init[0] = true;
        MX_CAN_Init();        
    }    
}

HAL_StatusTypeDef CanBase::write(uint32_t StdId, uint8_t *pData, uint8_t Size) 
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;

    TxHeader.StdId = StdId;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = Size;

    return HAL_CAN_AddTxMessage(_hcan, &TxHeader, pData, &TxMailbox);
}

HAL_StatusTypeDef CanBase::read(uint8_t *pData, uint8_t *Size) 
{    CAN_RxHeaderTypeDef RxHeader;
    if (HAL_CAN_GetRxMessage(_hcan, CAN_RX_FIFO0, &RxHeader, pData) == HAL_OK) {
        *Size = RxHeader.DLC;
        return HAL_OK;
    }
    return HAL_ERROR;
}
