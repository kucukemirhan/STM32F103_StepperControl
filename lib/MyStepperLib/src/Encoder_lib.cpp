#include "Encoder_lib.h"

bool EncoderBase::is_init[ENCODER_COUNT] = {false};
ISR<EncoderIT> EncoderIT::ISR_List;

void EncoderIT::PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    for (uint16_t i = 0; i < ISR_List.size(); i++)
    {
        if (ISR_List.get(i)->_htim == htim)
        {
            ISR_List.get(i)->checkOverflow();
        }
    }
}

// Encoder base implementation
EncoderBase::EncoderBase(TIM_HandleTypeDef *htim, uint32_t Channel): 
_htim(htim), _Channel(Channel)
{   
    init();
}

EncoderBase::~EncoderBase(void)
{
    if (_htim->Instance == TIM2)
    {
        is_init[1] = false;
        HAL_TIM_Base_MspDeInit(_htim);
    } 
}

void EncoderBase::init(void) 
{
    if (_htim->Instance == TIM2)
    {
        if (is_init[1])
        {
            return;
        }
        is_init[0] = true;
        void MX_TIM2_Init(void);
        return;
    }
}

HAL_StatusTypeDef EncoderBase::start(void) 
{
    return HAL_TIM_Encoder_Start(_htim, _Channel);
}

HAL_StatusTypeDef EncoderBase::stop(void) 
{
    return HAL_TIM_Encoder_Stop(_htim, _Channel);
}

void EncoderBase::reset(void) 
{
    _htim->Instance->CNT = 0;
}

uint32_t EncoderBase::read(void) 
{
    return __HAL_TIM_GET_COUNTER(_htim);
}

// Encoder interrupt implementation
EncoderIT::EncoderIT(TIM_HandleTypeDef *htim):
EncoderBase(htim, _Channel)
{
#if USE_HAL_TIM_REGISTER_CALLBACKS != 1
#error "USE_HAL_TIM_REGISTER_CALLBACKS must be enabled in stm32f1xx_hal_conf.h"
#endif

    htim->PeriodElapsedCallback = PeriodElapsedCallback;
    ISR_List.add(this);
}

EncoderIT::~EncoderIT(void)
{
    ISR_List.remove(this);
}

HAL_StatusTypeDef EncoderIT::start(void) 
{
    return HAL_TIM_Encoder_Start_IT(_htim, _Channel);
}

HAL_StatusTypeDef EncoderIT::stop(void) 
{
    return HAL_TIM_Encoder_Stop_IT(_htim, _Channel);
}

uint32_t EncoderIT::read(void) 
{
    // Read the raw 16-bit hardware counter
    uint16_t rawCount = __HAL_TIM_GET_COUNTER(_htim);

    // Combine with the overflow (shift by 16 bits)
    int32_t fullCount = (_overflow << 16) | rawCount;
    
    return fullCount;
}

void EncoderIT::checkOverflow(void)
{
    // Check direction bit to see if it was an overflow or underflow
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(_htim))
    {
        _overflow--;
    }
    else
    {
        _overflow++;
    }
}