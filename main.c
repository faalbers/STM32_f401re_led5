#include "stm32f4xx_hal.h"
//#include "stm32f4xx_nucleo.h"

static void SystemClock_Config(void);
static void Error_Handler(void);

// Disable when using RTOSes, since they have their own handler.
#if 1

// This is a sample SysTick handler, use it if you need HAL timings.
void __attribute__ ((section(".after_vectors")))
SysTick_Handler(void)
{
#if defined(USE_HAL_DRIVER)
	HAL_IncTick();
#endif
}

#endif

int main(void){
    HAL_Init();

    SystemClock_Config();

    // At this stage the system clock should have already been configured
    // at high speed.
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable GPIO Peripheral clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Configure pin in output push/pull mode
    GPIO_InitStructure.Pin = GPIO_PIN_5;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Set timer frequency to 1000 Hz. 1 tick = 1 ms
    SysTick_Config (SystemCoreClock / 1000u);

    // Infinite loop
    while (1)
    {
        // Turn GPIOA PIN_5 ON
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

        /* Insert delay 1000 ms */
        HAL_Delay(100);

        // Turn GPIOA PIN_5 ON
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

        /* Insert delay 1000 ms */
        HAL_Delay(900);
    }
}

static void SystemClock_Config(void)
{
    /* Set Oscillator and clock structures */
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* The voltage scaling allows optimizing the power consumption when the device is 
       clocked below the maximum system frequency, to update the voltage scaling value 
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
    
    /* HSE = 8 Mhz , HSI = 16 Mhz */
    
    /* Enable HSE Oscillator 8 Mhz */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    /* No idea what calibration does */
    RCC_OscInitStruct.HSICalibrationValue = 0x10;

    /* Turn PLL on and select oscillator source */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

    /* HSE devider. Result needs to stay between 1 and 2 Mhz */
    /* PLLM output = 8 Mhz / PLLM = 1 Mhz */
    RCC_OscInitStruct.PLL.PLLM = 8;

    /* PLLMout multiplier. Result needs to stay between 192 and 432 Mhz */
    /* The actual PLLN number can aslo only be between 192 and 432 */
    /* PLLN0ut = PLLMout * PLLN = 336 Mhz */
    RCC_OscInitStruct.PLL.PLLN = 336;

    /* PLLNout devider for SysCLK. Devider can only be 2, 4, 6 or 8 */
    /* SysCLK = PLLNout / 4 = 84 Mhz */
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;

    /* PLLNout devider for USB OTG FS. Between 4 and 15 */
    /* USB = PLLNout / 7 = 48 Mhz */
    RCC_OscInitStruct.PLL.PLLQ = 7;
    
    /* Check if oscilators are OK when setting */
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers*/
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

    /* AHB = SysCLK / 1 = 84 Mhz */
    /* This can be DIV1, 2, 4, 8, 16, 64, 128, 256, 512 */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    /* APB1 = SysCLK / 2 = 42 Mhz */
    /* This can be DIV1, 2, 4, 8, 16 */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
    /* APB2 = SysCLK / 1 = 84 Mhz */
    /* This can be DIV1, 2, 4, 8, 16 */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  

    /* wait state for Vdd = 2.7 to 3.6 SYSCLK = 84Mhz is 2 (see datasheet) */
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }

}

static void Error_Handler(void)
{
  while(1) {
  }
}

static void stopat(uint32_t val)
{
}

