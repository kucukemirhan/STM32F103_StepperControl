#ifndef INC_CAN_LIB_H_
#define INC_CAN_LIB_H_

#include "can.h"

#define CAN_COUNT 1

class CanBase {
public:
    CanBase(CAN_TypeDef *Instance, CAN_HandleTypeDef *hcan);

    void init();

    HAL_StatusTypeDef write(uint32_t StdId, uint8_t *pData, uint8_t Size);
    HAL_StatusTypeDef read(uint8_t *pData, uint8_t *Size);

protected:
    CAN_TypeDef *_Instance;
    CAN_HandleTypeDef *_hcan;

private:
    static bool is_init[CAN_COUNT];
};

#endif /* INC_CAN_LIB_H_ */
