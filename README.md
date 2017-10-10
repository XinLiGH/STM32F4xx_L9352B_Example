# STM32F4xx_L9352B_Example

STM32F4的L9352B(电磁阀驱动器)驱动。

## 开发环境

* 适用芯片：STM32F4全部芯片
* 固件库：STM32F4xx_DSP_StdPeriph_Lib_V1.8.0
* IDE：MDK517

## API

* void L9352B_Init(float frequency, float amplitude)
* void L9352B_DeInit(void)
* void L9352B_SetDither(float frequency, float amplitude)
* void L9352B_SetCurrent(float ch1, float ch2)

## 注意

修改接口需要注意，定时器的分频值和重载值。
