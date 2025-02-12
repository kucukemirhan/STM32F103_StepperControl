#include "TIM_lib.h"

bool TimBase::is_init[] = {false};

TimBase::TimBase(TIM_TypeDef *Instance, TIM_HandleTypeDef *htim): _Instance(Instance), _htim(htim) 
{   
    init();
}

TimBase::~TimBase(void) 
{
    if (_Instance == TIM3)
    {
        is_init[2] = false;
        HAL_TIM_Base_MspDeInit(_htim);
    }
}

void TimBase::init(void) 
{
    if (_Instance == TIM3)
    {
        if (is_init[2])
        {
            return;
        }
        is_init[2] = true;
        MX_TIM3_Init();
        return;
    }
}

HAL_StatusTypeDef TimBase::start(void) 
{
    return HAL_TIM_Base_Start(_htim);
}

HAL_StatusTypeDef TimBase::stop(void) 
{
    return HAL_TIM_Base_Stop(_htim);
}

void TimBase::reset(void) 
{
    _htim->Instance->CNT = 0;
}

uint32_t TimBase::read(void) 
{
    return _htim->Instance->CNT;
}