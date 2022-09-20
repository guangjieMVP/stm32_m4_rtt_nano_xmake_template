#include "bsp_led.h"
#include "stm32f4xx_hal.h"

#define BOARD_LED_PORT              GPIOC
#define BOARD_LED_PIN               GPIO_PIN_13

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    HAL_GPIO_WritePin(BOARD_LED_PORT, BOARD_LED_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = BOARD_LED_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BOARD_LED_PORT, &GPIO_InitStruct);
}

void BSP_LED_Init(void)
{
    MX_GPIO_Init();
}

void BSP_LED_Ctrl(int onoff)
{
	if (onoff)
	{
		HAL_GPIO_WritePin(BOARD_LED_PORT, BOARD_LED_PIN, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(BOARD_LED_PORT, BOARD_LED_PIN, GPIO_PIN_SET);
	}
}
