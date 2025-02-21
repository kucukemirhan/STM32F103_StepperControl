#include "TIM_lib.h"
#include "ISR_obj.h"

bool TimBase::is_init[] = {false};
ISR<TimIT> TimIT::ISR_List;

void TimIT::PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    for (uint16_t i = 0; i < ISR_List.size(); i++)
    {
        if (ISR_List.get(i)->_htim == htim)
        {
            ISR_List.get(i)->tick();
        }
    }
}

void TimPWM::PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

}

// Timer base implementation
TimBase::TimBase(TIM_TypeDef *Instance, TIM_HandleTypeDef *htim): _Instance(Instance), _htim(htim) 
{   
    init();
}

TimBase::~TimBase(void)
{
    if (_Instance == TIM1)
    {
        is_init[0] = false;
        HAL_TIM_Base_MspDeInit(_htim);
    }

    if (_Instance == TIM3)
    {
        is_init[2] = false;
        HAL_TIM_Base_MspDeInit(_htim);
    } 
}

void TimBase::init(void) 
{
    if (_Instance == TIM1)
    {
        if (is_init[0])
        {
            return;
        }
        is_init[0] = true;
        MX_TIM1_Init();
        return;
    }

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

// Timer with interrupt
TimIT::TimIT(TIM_TypeDef *Instance, TIM_HandleTypeDef *htim): 
TimBase(Instance, htim), _tick(0) 
{
#if USE_HAL_TIM_REGISTER_CALLBACKS != 1
#error "USE_HAL_TIM_REGISTER_CALLBACKS must be enabled in stm32f1xx_hal_conf.h"
#endif

    htim->PeriodElapsedCallback = PeriodElapsedCallback;
    ISR_List.add(this);
}

TimIT::~TimIT(void) 
{
    ISR_List.remove(this);
}

HAL_StatusTypeDef TimIT::start(void) 
{
    return HAL_TIM_Base_Start_IT(_htim);
}

HAL_StatusTypeDef TimIT::stop(void) 
{
    return HAL_TIM_Base_Stop_IT(_htim);
}

void TimIT::reset(void) 
{
    _tick = 0;
}

uint64_t TimIT::read(void) 
{
    return _tick;
}

void TimIT::tick(void) 
{
    _tick++;
}

bool TimIT::delay_ms(uint64_t start_time, uint64_t delay_time)
{
    return read() - start_time >= delay_time;
}

// PWM timer
TimPWM::TimPWM(TIM_TypeDef *Instance, TIM_HandleTypeDef *htim): TimBase(Instance, htim) 
{
#if USE_HAL_TIM_REGISTER_CALLBACKS != 1
#error "USE_HAL_TIM_REGISTER_CALLBACKS must be enabled in stm32f1xx_hal_conf.h"
#endif

    htim->PeriodElapsedCallback = PeriodElapsedCallback;
    ISR_List.add(this);
}

TimPWM::~TimPWM(void) 
{
    ISR_List.remove(this);
}

HAL_StatusTypeDef TimPWM::start(void) 
{
    return HAL_TIM_PWM_Start_IT(_htim, TIM_CHANNEL_ALL);
}

HAL_StatusTypeDef TimPWM::stop(void) 
{
    return HAL_TIM_PWM_Stop_IT(_htim, TIM_CHANNEL_ALL);
}

void TimPWM::reset(void) 
{
    _htim->Instance->CNT = 0;
}

uint32_t TimPWM::read(void) 
{
    return _htim->Instance->CNT;
}

void TimPWM::setSpeed(uint16_t value) 
{
    _htim->Instance->ARR = value;
    _htim->Instance->PSC = 0;
    _htim->Instance->CCR1 = value/2;
}

