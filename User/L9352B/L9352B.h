/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    L9352B.h
 * Date:         2016.08.31
 * Description:  L9352B driver.
*****************************************************************/

#ifndef __L9352B_H
#define __L9352B_H

/****************************************************************
 *                        Header include
*****************************************************************/
#include "stm32f4xx.h"

/****************************************************************
 *                       Macro definition
*****************************************************************/

/******************** Interface configuration *******************/

#define L9352B_EN_RCC_AHB1Periph_GPIO           RCC_AHB1Periph_GPIOC
#define L9352B_EN_GPIO                          GPIOC
#define L9352B_EN_GPIO_Pin                      GPIO_Pin_1

#define L9352B_CLK_RCC_AHB1Periph_GPIO          RCC_AHB1Periph_GPIOC
#define L9352B_CLK_GPIO                         GPIOC
#define L9352B_CLK_GPIO_Pin                     GPIO_Pin_8

#define L9352B_CLK_GPIO_PinSource               GPIO_PinSource8
#define L9352B_CLK_GPIO_AF_TIM                  GPIO_AF_TIM3

#define L9352B_CLK_RCC_APB1Periph_TIM           RCC_APB1Periph_TIM3
#define L9352B_CLK_TIM                          TIM3

#define L9352B_CLK_TIM_Prescaler                (83)                  /* Configuration frequency 250 KHz. */
#define L9352B_CLK_TIM_Period                   (3)

#define L9352B_CLK_TIM_OCInit                   TIM_OC3Init
#define L9352B_CLK_TIM_OCPreloadConfig          TIM_OC3PreloadConfig

#define L9352B_IN3_RCC_AHB1Periph_GPIO          RCC_AHB1Periph_GPIOB
#define L9352B_IN3_GPIO                         GPIOB
#define L9352B_IN3_GPIO_Pin                     GPIO_Pin_6

#define L9352B_IN3_GPIO_PinSource               GPIO_PinSource6
#define L9352B_IN3_GPIO_AF_TIM                  GPIO_AF_TIM4

#define L9352B_IN4_RCC_AHB1Periph_GPIO          RCC_AHB1Periph_GPIOB
#define L9352B_IN4_GPIO                         GPIOB
#define L9352B_IN4_GPIO_Pin                     GPIO_Pin_7

#define L9352B_IN4_GPIO_PinSource               GPIO_PinSource7
#define L9352B_IN4_GPIO_AF_TIM                  GPIO_AF_TIM4

#define L9352B_IN_RCC_APB1Periph_TIM            RCC_APB1Periph_TIM4
#define L9352B_IN_TIM                           TIM4

#define L9352B_IN_TIM_Prescaler                 (0)                   /* Configuration frequency 2 KHz. */
#define L9352B_IN_TIM_Period                    (41999)

#define L9352B_IN3_TIM_OCInit                   TIM_OC1Init
#define L9352B_IN3_TIM_OCPreloadConfig          TIM_OC1PreloadConfig

#define L9352B_IN4_TIM_OCInit                   TIM_OC2Init
#define L9352B_IN4_TIM_OCPreloadConfig          TIM_OC2PreloadConfig

#define L9352B_IN3_TIM_SetCompare               TIM_SetCompare1
#define L9352B_IN4_TIM_SetCompare               TIM_SetCompare2

#define L9352B_IN_TIM_IRQn                      TIM4_IRQn
#define L9352B_IN_TIM_IRQHandler                TIM4_IRQHandler
#define L9352B_IN_TIM_IRQ_PreemptionPriority    (0)
#define L9352B_IN_TIM_IRQ_SubPriority           (0)

/****************************************************************/

/****************************************************************
 *                       Type definition
*****************************************************************/


/****************************************************************
 *                     Structure definition
*****************************************************************/



#ifdef __cplusplus
 extern "C" {
#endif  /* __cplusplus */

/****************************************************************
 *                     Variable declaration
*****************************************************************/


/****************************************************************
 *                     Function declaration
*****************************************************************/
void L9352B_Init(float frequency, float amplitude);
void L9352B_DeInit(void);
void L9352B_SetDither(float frequency, float amplitude);
void L9352B_SetCurrent(float ch1, float ch2);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif	/* __L9352B_H */
