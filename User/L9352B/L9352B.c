/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    L9352B.c
 * Date:         2016.08.31
 * Description:  L9352B driver.
*****************************************************************/

/****************************************************************
 *                        Header include
*****************************************************************/
#include "L9352B.h"
#include "arm_math.h"
#include <stdbool.h>

/****************************************************************
 *                       Global variables
*****************************************************************/
static __IO float ditherFrequency = 0.0f;
static __IO float ditherAmplitude = 0.0f;

static __IO float ch1Current = 0.0f;
static __IO float ch2Current = 0.0f;

static __IO bool initFlag = false;

/****************************************************************
 *                     Function declaration
*****************************************************************/
static void L9352B_EN_Init(void);
static void L9352B_EN_Low(void);
static void L9352B_EN_High(void);
static void L9352B_CLK_Init(void);
static void L9352B_IN_Init(void);
static void L9352B_IN3_SetPwm(float DutyCycle);
static void L9352B_IN4_SetPwm(float DutyCycle);

/****************************************************************
 *                     Function definition
*****************************************************************/

/****************************************************************
 * Function:    L9352B_EN_Init
 * Description: Initialization EN pin.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void L9352B_EN_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure = {0};
  
  RCC_AHB1PeriphClockCmd(L9352B_EN_RCC_AHB1Periph_GPIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin   = L9352B_EN_GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(L9352B_EN_GPIO, &GPIO_InitStructure);
}

/****************************************************************
 * Function:    L9352B_EN_Low
 * Description: Pull low EN pin.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void L9352B_EN_Low(void)
{
  GPIO_ResetBits(L9352B_EN_GPIO, L9352B_EN_GPIO_Pin);
}

/****************************************************************
 * Function:    L9352B_EN_High
 * Description: Pull high EN pin.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void L9352B_EN_High(void)
{
  GPIO_SetBits(L9352B_EN_GPIO, L9352B_EN_GPIO_Pin);
}

/****************************************************************
 * Function:    L9352B_CLK_Init
 * Description: Initialization CLK pin.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void L9352B_CLK_Init(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure    = {0};
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
  TIM_OCInitTypeDef       TIM_OCInitStructure   = {0};
  
  RCC_AHB1PeriphClockCmd(L9352B_CLK_RCC_AHB1Periph_GPIO, ENABLE);
  RCC_APB1PeriphClockCmd(L9352B_CLK_RCC_APB1Periph_TIM, ENABLE);
  
  GPIO_PinAFConfig(L9352B_CLK_GPIO, L9352B_CLK_GPIO_PinSource, L9352B_CLK_GPIO_AF_TIM);
  
  GPIO_InitStructure.GPIO_Pin   = L9352B_CLK_GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(L9352B_CLK_GPIO, &GPIO_InitStructure);
  
  TIM_DeInit(L9352B_CLK_TIM);
  
  TIM_TimeBaseStructure.TIM_Prescaler     = L9352B_CLK_TIM_Prescaler;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period        = L9352B_CLK_TIM_Period;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(L9352B_CLK_TIM, &TIM_TimeBaseStructure);
  
  TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse       = (L9352B_CLK_TIM_Period + 1) / 2;
  TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
  L9352B_CLK_TIM_OCInit(L9352B_CLK_TIM, &TIM_OCInitStructure);
  
  L9352B_CLK_TIM_OCPreloadConfig(L9352B_CLK_TIM, TIM_OCPreload_Enable);
  
  TIM_ARRPreloadConfig(L9352B_CLK_TIM, ENABLE);
	
  TIM_Cmd(L9352B_CLK_TIM, ENABLE);
}

/****************************************************************
 * Function:    L9352B_IN_Init
 * Description: Initialization IN pin.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void L9352B_IN_Init(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure    = {0};
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
  TIM_OCInitTypeDef       TIM_OCInitStructure   = {0};
  NVIC_InitTypeDef        NVIC_InitStructure    = {0};
  
  RCC_AHB1PeriphClockCmd(L9352B_IN3_RCC_AHB1Periph_GPIO, ENABLE);
  RCC_AHB1PeriphClockCmd(L9352B_IN4_RCC_AHB1Periph_GPIO, ENABLE);
  RCC_APB1PeriphClockCmd(L9352B_IN_RCC_APB1Periph_TIM, ENABLE);
  
  GPIO_PinAFConfig(L9352B_IN3_GPIO, L9352B_IN3_GPIO_PinSource, L9352B_IN3_GPIO_AF_TIM);
  GPIO_PinAFConfig(L9352B_IN4_GPIO, L9352B_IN4_GPIO_PinSource, L9352B_IN4_GPIO_AF_TIM);
  
  GPIO_InitStructure.GPIO_Pin   = L9352B_IN3_GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(L9352B_IN3_GPIO, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin   = L9352B_IN4_GPIO_Pin;
  GPIO_Init(L9352B_IN4_GPIO, &GPIO_InitStructure);
  
  TIM_DeInit(L9352B_IN_TIM);
  
  TIM_TimeBaseStructure.TIM_Prescaler     = L9352B_IN_TIM_Prescaler;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period        = L9352B_IN_TIM_Period;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(L9352B_IN_TIM, &TIM_TimeBaseStructure);
  
  TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse       = 0;
  TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
  L9352B_IN3_TIM_OCInit(L9352B_IN_TIM, &TIM_OCInitStructure);
  L9352B_IN4_TIM_OCInit(L9352B_IN_TIM, &TIM_OCInitStructure);
  
  L9352B_IN3_TIM_OCPreloadConfig(L9352B_IN_TIM, TIM_OCPreload_Enable);
  L9352B_IN4_TIM_OCPreloadConfig(L9352B_IN_TIM, TIM_OCPreload_Enable);
  
  TIM_ARRPreloadConfig(L9352B_IN_TIM, ENABLE);
	
  TIM_ClearFlag(L9352B_IN_TIM, TIM_FLAG_Update);
  TIM_ITConfig(L9352B_IN_TIM, TIM_IT_Update, ENABLE);
  TIM_Cmd(L9352B_IN_TIM, ENABLE);
  
  NVIC_InitStructure.NVIC_IRQChannel                   = L9352B_IN_TIM_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = L9352B_IN_TIM_IRQ_PreemptionPriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = L9352B_IN_TIM_IRQ_SubPriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************
 * Function:    L9352B_IN3_SetPwm
 * Description: L9352B Set Pwm Duty Cycle To IN3.
 * Input:       DutyCycle: 0 - 100 corresponds to 0% - 100%.
 * Output:
 * Return:
*****************************************************************/
static void L9352B_IN3_SetPwm(float DutyCycle)
{
  uint16_t compare = 0;
  
  if(DutyCycle > 100.0f)
  {
    DutyCycle = 100.0f;
  }
  else if(DutyCycle < 0.0f)
  {
    DutyCycle = 0.0f;
  }
  
  compare = (uint16_t)((float)(L9352B_IN_TIM_Period + 1) * DutyCycle * 0.01f);
  
  L9352B_IN3_TIM_SetCompare(L9352B_IN_TIM, compare);
}

/****************************************************************
 * Function:    L9352B_IN4_SetPwm
 * Description: L9352B Set Pwm Duty Cycle To IN4.
 * Input:       DutyCycle: 0 - 100 corresponds to 0% - 100%.
 * Output:
 * Return:
*****************************************************************/
static void L9352B_IN4_SetPwm(float DutyCycle)
{
  uint16_t compare = 0;
  
  if(DutyCycle > 100.0f)
  {
    DutyCycle = 100.0f;
  }
  else if(DutyCycle < 0.0f)
  {
    DutyCycle = 0.0f;
  }
  
  compare = (uint16_t)((float)(L9352B_IN_TIM_Period + 1) * DutyCycle * 0.01f);
  
  L9352B_IN4_TIM_SetCompare(L9352B_IN_TIM, compare);
}

/****************************************************************
 * Function:    L9352B_Init
 * Description: Initialization L9352B.
 * Input:       frequency: Dither frequency 0 - 1000 unit: Hz.
 *              amplitude: Dither amplitude 0 - 2.5 unit: A.
 * Output:
 * Return:
*****************************************************************/
void L9352B_Init(float frequency, float amplitude)
{
  ch1Current = 0.0f;
  ch2Current = 0.0f;
  
  L9352B_SetDither(frequency, amplitude);
  
  if(initFlag == false)
  {
    initFlag = true;
    
    L9352B_EN_Init();
    L9352B_EN_Low();
    L9352B_CLK_Init();
    L9352B_IN_Init();
    L9352B_EN_High();
  }
}

/****************************************************************
 * Function:    L9352B_DeInit
 * Description: Deinitializes L9352B.
 * Input:
 * Output:
 * Return:
*****************************************************************/
void L9352B_DeInit(void)
{
  ditherFrequency = 0.0f;
  ditherAmplitude = 0.0f;
  
  ch1Current = 0.0f;
  ch2Current = 0.0f;
  
  if(initFlag == true)
  {
    initFlag = false;
    
    L9352B_EN_Low();
    RCC_APB1PeriphClockCmd(L9352B_CLK_RCC_APB1Periph_TIM, DISABLE);
    RCC_APB1PeriphClockCmd(L9352B_IN_RCC_APB1Periph_TIM, DISABLE);
    TIM_DeInit(L9352B_CLK_TIM);
    TIM_DeInit(L9352B_IN_TIM);
  }
}

/****************************************************************
 * Function:    L9352B_SetDither
 * Description: Set dither.
 * Input:       frequency: Dither frequency 0 - 1000 unit: Hz.
 *              amplitude: Dither amplitude 0 - 2.5 unit: A.
 * Output:
 * Return:
*****************************************************************/
void L9352B_SetDither(float frequency, float amplitude)
{
  if(frequency < 0.0f)
  {
    ditherFrequency = 0.0f;
  }
  else
  {
    ditherFrequency = frequency;
  }
  
  if(amplitude < 0.0f)
  {
    ditherAmplitude = 0.0f;
  }
  else
  {
    ditherAmplitude = amplitude;
  }
}

/****************************************************************
 * Function:    L9352B_SetCurrent
 * Description: Set current.
 * Input:       ch1: Channel 1 current 0 - 2.5 unit: A.
 *              ch2: Channel 2 current 0 - 2.5 unit: A.
 * Output:
 * Return:
*****************************************************************/
void L9352B_SetCurrent(float ch1, float ch2)
{
  if(ch1 < 0.0f)
  {
    ch1Current = 0.0f;
  }
  else
  {
    ch1Current = ch1;
  }
  
  if(ch2 < 0.0f)
  {
    ch2Current = 0.0f;
  }
  else
  {
    ch2Current = ch2;
  }
}

/****************************************************************
 * Function:    L9352B_IN_TIM_IRQHandler
 * Description: This function handles TIM handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
void L9352B_IN_TIM_IRQHandler(void)
{
  static uint32_t count = 0;
  static float    time  = 0.0f;
  
  float dither = 0.0f;
  float ch1    = 0.0f;
  float ch2    = 0.0f;
  
  if(TIM_GetITStatus(L9352B_IN_TIM, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(L9352B_IN_TIM, TIM_IT_Update);
    
    if((ditherFrequency > 0.0f) && (ditherAmplitude > 0.0f))
    {
      time  += 0.001f * PI * ditherFrequency;
      dither = ditherAmplitude * arm_sin_f32(time);
      
      count++;
      
      if(count >= (uint32_t)(2000.0f / ditherFrequency))
      {
        count = 0;
        time  = 0.0f;
      }
    }
    
    if(ch1Current > 0.0f)
    {
      ch1 = ch1Current + dither;
    }
    
    if(ch2Current > 0.0f)
    {
      ch2 = ch2Current + dither;
    }
    
    L9352B_IN3_SetPwm(ch1 * 40.0f);
    L9352B_IN4_SetPwm(ch2 * 40.0f);
  }
}
