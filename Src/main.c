
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "EPD_2in13.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
  
uint8_t flag = 1;
uint8_t buffer[20] = {0};
int main(void)
{

	
	/* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  

  HAL_UART_Transmit(&huart3,"ssssssssss",10,0xffff);
  HAL_UART_Transmit(&huart3,"ssssssssss",10,0xffff);
  HAL_UART_Receive_IT(&huart3,buffer , 10);
  
  /* USER CODE BEGIN 2 */	
		printf("2.13inch e-Paper demo\r\n");
		if(EPD_Init(lut_full_update) != 0) {
			printf("e-Paper init failed\r\n");
		}
    EPD_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1)) * EPD_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 90, WHITE);
    Paint_SelectImage(BlackImage);
//    Paint_SetMirroring(MIRROR_HORIZONTAL); //
    Paint_Clear(WHITE);
		
#if 1	//show image for array 显示图片  
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_2in13);
    
    EPD_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image

    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    /*
	Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);

    Paint_DrawLine(20, 10, 70, 60, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
    Paint_DrawLine(70, 10, 20, 60, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
    Paint_DrawRectangle(20, 10, 70, 60, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawRectangle(85, 10, 135, 60, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    
    Paint_DrawLine(45, 15, 45, 55, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);
    Paint_DrawLine(25, 35, 70, 35, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);
    Paint_DrawCircle(45, 35, 20, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);    
    Paint_DrawCircle(110, 35, 20, WHITE, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    
    Paint_DrawString_EN(140, 15, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawNum(140, 40, 123456789, &Font16, BLACK, WHITE);
	*/
	//Paint_Clear(BLACK);

    Paint_DrawString_CN(40, 40, "你好", &Font24CN, BLACK, WHITE);
	Paint_DrawString_EN(120, 50, "UESTC", &Font24, WHITE, BLACK);
    EPD_Display(BlackImage);
	DEV_Delay_ms(2000);
	Paint_Clear(WHITE);
	EPD_Display(BlackImage);


	Paint_DrawString_EN(68, 80, "designed by", &Font16, WHITE, BLACK);
	Paint_DrawString_CN(30, 25, "墨水屏手机壳", &Font24CN, BLACK, WHITE);
	Paint_DrawString_CN(55, 95, "詹越扬", &Font12CN, WHITE, BLACK);
	Paint_DrawString_CN(105, 95, "吴海洋", &Font12CN, WHITE, BLACK);
	Paint_DrawString_CN(155, 95, "莫雅洁", &Font12CN, WHITE, BLACK);
	EPD_Display(BlackImage);
	DEV_Delay_ms(10000);
	
	Paint_Clear(WHITE);
	EPD_Display(BlackImage);
	DEV_Delay_ms(2000);
    //EPD_Display(BlackImage);

#endif

#if 1   //Partial refresh, example shows time 显示时间   
    if(EPD_Init(lut_partial_update) != 0) {
        printf("e-Paper init failed\r\n");
    }
	
	Paint_SelectImage(BlackImage);
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 0;
    sPaint_time.Sec = 0;
    for (;;) {
        sPaint_time.Sec = sPaint_time.Sec + 1;
        if (sPaint_time.Sec == 60) {
            sPaint_time.Min = sPaint_time.Min + 1;
            sPaint_time.Sec = 0;
            if (sPaint_time.Min == 60) {
                sPaint_time.Hour =  sPaint_time.Hour + 1;
                sPaint_time.Min = 0;
                if (sPaint_time.Hour == 24) {
                    sPaint_time.Hour = 0;
                    sPaint_time.Min = 0;
                    sPaint_time.Sec = 0;
                }
            }
        }
        Paint_ClearWindows(80, 100, 80 + Font20.Width * 7, 100 + Font20.Height, WHITE);
        Paint_DrawTime(80, 100, &sPaint_time, &Font20, WHITE, BLACK);

				EPD_Display(BlackImage);
        DEV_Delay_ms(500);//Analog clock 1s
    }

#endif
	printf("Goto Sleep mode...\r\n");
    EPD_Sleep();	
    if(NULL != BlackImage){
        free(BlackImage);
        BlackImage = NULL;
    }
  while (1)
  {


  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1)) * EPD_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for memory...\r\n");
        return ;
    }
	Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 90, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
	UNUSED(huart);
	HAL_UART_Transmit(&huart3, (uint8_t *)buffer, 10,0xFFFF);
	HAL_UART_Transmit(&huart3, "4444444444", 10,0xFFFF);
	//EPD_Clear();
	HAL_UART_Transmit(&huart3, "6666666666", 10,0xFFFF);
    //DEV_Delay_ms(500);
	
	Paint_DrawString_EN(20, 20, (uint8_t *)buffer, &Font24, WHITE, BLACK);
	HAL_UART_Transmit(&huart3, "7777777777", 10,0xFFFF);
	Paint_DrawString_CN(60, 90, "指导教师李晓宁", &Font12CN, BLACK, WHITE);
	HAL_UART_Transmit(&huart3, "1111111111", 10,0xFFFF);
    EPD_Display(BlackImage);
	HAL_UART_Transmit(&huart3, "2222222222", 10,0xFFFF);
	DEV_Delay_ms(1000);
	HAL_UART_Transmit(&huart3, "3333333333", 10,0xFFFF);

	

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_10);
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_11);
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
