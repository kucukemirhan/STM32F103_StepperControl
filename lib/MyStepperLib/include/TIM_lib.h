#ifndef INC_TIM_LIB_H_
#define INC_TIM_LIB_H_

#include "tim.h"

#define TIM_COUNT (4)

class TimBase {
public:
    TimBase(TIM_TypeDef *Instance, TIM_HandleTypeDef *htim);
    ~TimBase(void);

    virtual HAL_StatusTypeDef start(void);
    virtual HAL_StatusTypeDef stop(void);
    virtual void reset(void);
    virtual uint32_t read(void);

protected:
    TIM_TypeDef *_Instance;
    TIM_HandleTypeDef *_htim;

private:
    static bool is_init[TIM_COUNT];
    void init(void);
};

#endif /* INC_TIM_LIB_H_ */