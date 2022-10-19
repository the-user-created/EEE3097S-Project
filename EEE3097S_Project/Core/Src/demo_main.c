/* USER CODE BEGIN Header */

/*
 * Data collection size can be between 44 bytes and 616 bytes
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

#include "stm32f0xx_hal_i2c.h"
#include "heatshrink/heatshrink_encoder.h"
#include "heatshrink/heatshrink_decoder.h"
#include "blowfish/blowfish.h"
#include "crc32/crc32.h"

#include "sensors/imu.h"
#include "sensors/DEV_Config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t UART_UpdataFlag = 0;
uint8_t IMU_SampleFlag = 0;
uint8_t Press_SampleFlag = 0;
int16_t magn[3] = {0};
int16_t accel[3] = {0}, gyro[3] = {0};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DATASIZE 132
#define KEYSIZE 32
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t compressed_size[4] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Compress(uint8_t *input, uint32_t input_size, uint8_t *output);
void Decompress(uint8_t *input, uint32_t input_size, uint32_t comp_size, uint8_t *output);
uint8_t Encrypt(uint8_t *input, uint32_t input_size, uint8_t *output);
void Decrypt(uint8_t *input, uint32_t input_size, uint8_t padding, uint8_t *output);
void xcrc32(const uint8_t *buf, uint32_t len, uint8_t *output);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  IMU_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  // End sequence used after each transmission of data during non-IMU module tests
  char text[10] = "\rEEEEEEE\r\n";
  // Buffer array for end sequence
  char buf[10] = {0};
  // print the end sequence to the buffer array
  sprintf(buf, text, sizeof(text));
  // Ready to transmit data sequence used by client and STMs
  char ready[3] = "RDY";
  // Ready to receive data from the client
  char gimmeData[3] = "GMD";
  // Skip receiving this set of data from the client
  char skip[3] = "SKP";
  // Buffer used for ready, gimmeData, and skip
  uint8_t buf2[3] = {0};

  // compressed data size
  uint32_t comp_size;
  // padding length for encryption/decryption
  uint8_t padding_len[1] = {0};
  // int time value
  uint32_t time_val;
  // checksum array
  uint8_t *crc = calloc(4, sizeof(uint8_t));

  // Generate encryption key
  uint32_t *key = calloc(KEYSIZE, sizeof(uint32_t));
  key[0] = 0x11;
  key[31] = 0xFF;

  blowfish_init(key, KEYSIZE);
  free(key);

  // count is used for tracking how many insertions to arr1 have occurred (how much data is in arr1)
  uint32_t count = 0;
  // Used for indicating to the client that DATASIZE sensor data has been collected
  char text2[14] = "Data Collected";
  // Buffer used for "Data Collected"
  char buf3[14] = {0};
  // print "Data Collected" to buf3
  sprintf(buf3, text2, sizeof(text2));

  // single byte int time array
  uint8_t *time_arr = calloc(4, sizeof(uint8_t));

  // Temporary data array 1
  uint8_t *arr1 = calloc(DATASIZE, sizeof(uint8_t));
  // Temporary data array 2
  uint8_t *arr2 = calloc(DATASIZE, sizeof(uint8_t));

  while (1)
  {
    IMU_GetQuater();

    if (count == DATASIZE) {
    	// TRANSMIT the DATA COLLECTED sequence
    	HAL_UART_Transmit(&huart1, (uint8_t*)buf3, sizeof(buf3), 1000);
    	// TRANSMIT the END SEQUENCE
		HAL_UART_Transmit(&huart1, (uint8_t*)buf, sizeof(buf), 1000);
		HAL_Delay(100);

    	while (1)
    	{
			// RECEIVE directive from USER
			HAL_UART_Receive(&huart1, buf2, 3, 500);
			if (memcmp(ready, buf2, 3) == 0) {
				// CALCULATE the CRC32 Checksum
				xcrc32(arr1, DATASIZE, crc);
				// TRANSMIT the CRC32 Checksum
				HAL_UART_Transmit(&huart1, crc, 4, 1000);
				// TRANSMIT the END SEQUENCE
				HAL_UART_Transmit(&huart1, (uint8_t*)buf, sizeof(buf), 1000);
				HAL_Delay(100);

				// COMPRESS the data
				Compress(arr1, DATASIZE, arr2);
				// CALCULATE the compressed size
				comp_size = compressed_size[0] + (compressed_size[1] << 8) + (compressed_size[2] << 16) + (compressed_size[3] << 24);
				// ENCRYPT the data
				padding_len[0] = Encrypt(arr2, comp_size, arr1);

				// TRANSMIT the encrypted DATA
				HAL_UART_Transmit(&huart1, arr1, DATASIZE + padding_len[0], 5000);
				// TRANSMIT the END SEQUENCE
				HAL_UART_Transmit(&huart1, (uint8_t*)buf, sizeof(buf), 1000);
				HAL_Delay(100);

				// TRANSMIT the COMPRESSED SIZE data
				HAL_UART_Transmit(&huart1, compressed_size, 4, 1000);
				// TRANSMIT the END SEQUENCE
				HAL_UART_Transmit(&huart1, (uint8_t*)buf, sizeof(buf), 1000);
				HAL_Delay(100);

				// TRANSMIT the PADDING LENGTH data
				HAL_UART_Transmit(&huart1, padding_len, 1, 1000);
				// TRANSMIT the END SEQUENCE
				HAL_UART_Transmit(&huart1, (uint8_t*)buf, sizeof(buf), 1000);

				// Reset all arrays and variables in preparation for the next iteration
				memset(buf2, 0, sizeof(buf2));
				memset(compressed_size, 0, 4);
				memset(padding_len, 0, 1);
				memset(arr1, 0, DATASIZE);
				memset(arr2, 0, DATASIZE);
				memset(crc, 0, 4);
				comp_size = 0;
			} else if (memcmp(gimmeData, buf2, 3) == 0) {
				// RECEIVE the PADDING LENGTH data
				HAL_UART_Receive(&huart1, padding_len, 1, 1000);
				HAL_Delay(100);

				// RECEIVE the COMPRESSED SIZE data
				HAL_UART_Receive(&huart1, compressed_size, 4, 1000);
				HAL_Delay(100);

				// CALCULATE THE COMPRESSED DATA SIZE
				comp_size = compressed_size[0] + (compressed_size[1] << 8) + (compressed_size[2] << 16) + (compressed_size[3] << 24);

				// RECEIVE the encrypted DATA
				HAL_UART_Receive(&huart1, arr1, comp_size + padding_len[0], 5000);

				// DECRYPT the DATA
				Decrypt(arr1, comp_size, padding_len[0], arr2);
				// DECOMPRESS the DATA
				Decompress(arr2, DATASIZE, comp_size, arr1);

				// TRANSMIT the decompressed DATA
				HAL_UART_Transmit(&huart1, arr1, DATASIZE, 5000);
				// TRANSMIT the END SEQUENCE
				HAL_UART_Transmit(&huart1, (uint8_t*)buf, sizeof(buf), 1000);
				HAL_Delay(100);

				// CALCULATE the CRC32 checksum
				xcrc32(arr1, DATASIZE, crc);
				// TRANSMIT the CRC32 checksum
				HAL_UART_Transmit(&huart1, crc, 4, 1000);
				// TRANSMIT the END SEQUENCE
				HAL_UART_Transmit(&huart1, (uint8_t*)buf, sizeof(buf), 1000);
				HAL_Delay(100);
				break;
			} else if (memcmp(skip, buf2, 3) == 0) {
				break;
			}
    	}
    	// Reset all arrays and variables in preparation for the next iteration
    	memset(buf2, 0, sizeof(buf2));
		memset(compressed_size, 0, 4);
		memset(padding_len, 0, 1);
		memset(arr1, 0, DATASIZE);
		memset(arr2, 0, DATASIZE);
		memset(crc, 0, 4);
		comp_size = 0;
		count = 0;
    } else {
    	// READ TIME DATA
		time_val = time(0);
		memcpy(time_arr, &time_val, sizeof(uint32_t));
		arr1[count++] = time_arr[0];
		arr1[count++] = time_arr[1];
		arr1[count++] = time_arr[2];
		arr1[count++] = time_arr[3];
		// READ ACCELERATION DATA
		arr1[count++] = (accel[0] >> 8) & 0x00FF;
		arr1[count++] = accel[0] & 0x00FF;
		arr1[count++] = (accel[1] >> 8) & 0x00FF;
		arr1[count++] = accel[1] & 0x00FF;
		arr1[count++] = (accel[2] >> 8) & 0x00FF;
		arr1[count++] = accel[2] & 0x00FF;
		// READ GYROSCOPE DATA
		arr1[count++] = (gyro[0] >> 8) & 0x00FF;
		arr1[count++] = gyro[0] & 0x00FF;
		arr1[count++] = (gyro[1] >> 8) & 0x00FF;
		arr1[count++] = gyro[1] & 0x00FF;
		arr1[count++] = (gyro[2] >> 8) & 0x00FF;
		arr1[count++] = gyro[2] & 0x00FF;
		// READ MAGNETOMETER DATA
		arr1[count++] = (magn[0] >> 8) & 0x00FF;
		arr1[count++] = magn[0] & 0x00FF;
		arr1[count++] = (magn[1] >> 8) & 0x00FF;
		arr1[count++] = magn[1] & 0x00FF;
		arr1[count++] = (magn[2] >> 8) & 0x00FF;
		arr1[count++] = magn[2] & 0x00FF;

		// DELAY READING FROM THE SENSOR FOR 500ms
		DEV_Delay_ms(500);
    }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Compress(uint8_t *input, uint32_t input_size, uint8_t *output)
{
    static heatshrink_encoder hse;

    heatshrink_encoder_reset(&hse);
    size_t comp_sz = input_size + (input_size/2) + 4;
    uint8_t *comp = calloc(input_size, sizeof(uint8_t));

    size_t count = 0;
    uint32_t sunk = 0;
    uint32_t polled = 0;
    while (sunk < input_size) {
        heatshrink_encoder_sink(&hse, &input[sunk], input_size - sunk, &count);
        sunk += count;
        if (sunk == input_size) {
            heatshrink_encoder_finish(&hse);
        }

        HSE_poll_res pres;
        do {
            pres = heatshrink_encoder_poll(&hse, &comp[polled], comp_sz - polled, &count);
            polled += count;
        } while (pres == HSER_POLL_MORE);
        if (sunk == input_size) {
            heatshrink_encoder_finish(&hse);
        }
    }

    memmove(compressed_size, &polled, sizeof(uint32_t));

    memcpy(output, comp, polled);
    free(comp);
}

void Decompress(uint8_t *input, uint32_t input_size, uint32_t comp_size, uint8_t *output)
{
    static heatshrink_decoder hsd;
    heatshrink_decoder_reset(&hsd);

    size_t decomp_sz = input_size + (input_size/2) + 4;
    uint8_t *decomp = calloc(input_size, sizeof(uint8_t));
    uint32_t sunk = 0;
    uint32_t polled = 0;
    size_t count = 0;

    while (sunk < comp_size) {
        heatshrink_decoder_sink(&hsd, &input[sunk], comp_size - sunk, &count);
        sunk += count;
        if (sunk == comp_size) {
            heatshrink_decoder_finish(&hsd);
        }

        HSD_poll_res pres;
        do {
            pres = heatshrink_decoder_poll(&hsd, &decomp[polled],decomp_sz - polled, &count);
            polled += count;
        } while (pres == HSDR_POLL_MORE);
        if (sunk == comp_size) {
            heatshrink_decoder_finish(&hsd);
        }
    }

    memcpy(output, decomp, polled);
    free(decomp);
}

uint8_t Encrypt(uint8_t *input, uint32_t input_size, uint8_t *output) {
	uint8_t padding = input_size%8;  // Get trailing bytes

	// Determine how many bytes should be added to allow for complete encryption
	if (padding != 0) {
		padding = 8 - padding;
	}

	// Adding padding to the data
	uint8_t *temp_input = calloc(input_size + padding, sizeof(uint8_t));
	memmove(temp_input, input, input_size);

	uint8_t *encrypted = calloc(input_size + padding, sizeof(uint8_t));
	blowfish_encrypt(temp_input, input_size + padding, encrypted);
	free(temp_input);

	memmove(output, encrypted, input_size + padding);
	free(encrypted);

	return padding;
}

void Decrypt(uint8_t *input, uint32_t input_size, uint8_t padding, uint8_t *output) {
	uint8_t *decrypted = calloc(input_size, sizeof(uint8_t));
	blowfish_decrypt(input, input_size, decrypted);
	memmove(output, decrypted, input_size + padding);
	free(decrypted);
}

void xcrc32(const uint8_t *buf, uint32_t len, uint8_t *output)
{
    uint32_t crc = 0;
    while (len--)
    {
        crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ *buf) & 255];
        buf++;
    }

    memmove(output, &crc, sizeof(uint32_t));
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
