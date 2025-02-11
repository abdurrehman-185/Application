#include "main.h"
#include "stdio.h"
#include <string.h>

UART_HandleTypeDef hlpuart1;
CRC_HandleTypeDef CrcHandle;

#define FLASH_START_ADDRESS 0x08000000
#define APP_START_ADDRESS   0x080FF800
#define END_ADDRESS         0x080FFFFF
#define FLASH_PAGE_NBPERBANK  256
#define FLASH_BANK_NUMBER     2
#define ARRAY_SIZE 512
void SystemClock_Config(void);
void Error_Handler(void);
static void LED_Init(void);
static void UART_Init(void);
void Application_Init(void);
void CheckPassword(void);
void ProcessCommand(void);
void EraseFlash(uint32_t startAddress);
HAL_StatusTypeDef WriteFlash(uint32_t* pData, uint32_t startAddress, uint32_t dataLength);
void ReadFlash(uint32_t* pData, uint32_t startAddress, uint32_t dataLength);
void VerifyFlashWrite(uint32_t* pData, uint32_t startAddress, uint32_t dataLength);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    LED_Init();
    UART_Init();

    uint32_t pData[ARRAY_SIZE] = {
    APP_START_ADDRESS, APP_START_ADDRESS + 1, APP_START_ADDRESS + 2, APP_START_ADDRESS + 3, APP_START_ADDRESS + 4, APP_START_ADDRESS + 5, APP_START_ADDRESS + 6, APP_START_ADDRESS + 7,
    APP_START_ADDRESS + 8, APP_START_ADDRESS + 9, APP_START_ADDRESS + 10, APP_START_ADDRESS + 11, APP_START_ADDRESS + 12, APP_START_ADDRESS + 13, APP_START_ADDRESS + 14, APP_START_ADDRESS + 15,
    APP_START_ADDRESS + 16, APP_START_ADDRESS + 17, APP_START_ADDRESS + 18, APP_START_ADDRESS + 19, APP_START_ADDRESS + 20, APP_START_ADDRESS + 21, APP_START_ADDRESS + 22, APP_START_ADDRESS + 23,
    APP_START_ADDRESS + 24, APP_START_ADDRESS + 25, APP_START_ADDRESS + 26, APP_START_ADDRESS + 27, APP_START_ADDRESS + 28, APP_START_ADDRESS + 29, APP_START_ADDRESS + 30, APP_START_ADDRESS + 31,
    APP_START_ADDRESS + 32, APP_START_ADDRESS + 33, APP_START_ADDRESS + 34, APP_START_ADDRESS + 35, APP_START_ADDRESS + 36, APP_START_ADDRESS + 37, APP_START_ADDRESS + 38, APP_START_ADDRESS + 39,
    APP_START_ADDRESS + 40, APP_START_ADDRESS + 41, APP_START_ADDRESS + 42, APP_START_ADDRESS + 43, APP_START_ADDRESS + 44, APP_START_ADDRESS + 45, APP_START_ADDRESS + 46, APP_START_ADDRESS + 47,
    APP_START_ADDRESS + 48, APP_START_ADDRESS + 49, APP_START_ADDRESS + 50, APP_START_ADDRESS + 51, APP_START_ADDRESS + 52, APP_START_ADDRESS + 53, APP_START_ADDRESS + 54, APP_START_ADDRESS + 55,
    APP_START_ADDRESS + 56, APP_START_ADDRESS + 57, APP_START_ADDRESS + 58, APP_START_ADDRESS + 59, APP_START_ADDRESS + 60, APP_START_ADDRESS + 61, APP_START_ADDRESS + 62, APP_START_ADDRESS + 63,
    APP_START_ADDRESS + 64, APP_START_ADDRESS + 65, APP_START_ADDRESS + 66, APP_START_ADDRESS + 67, APP_START_ADDRESS + 68, APP_START_ADDRESS + 69, APP_START_ADDRESS + 70, APP_START_ADDRESS + 71,
    APP_START_ADDRESS + 72, APP_START_ADDRESS + 73, APP_START_ADDRESS + 74, APP_START_ADDRESS + 75, APP_START_ADDRESS + 76, APP_START_ADDRESS + 77, APP_START_ADDRESS + 78, APP_START_ADDRESS + 79,
    APP_START_ADDRESS + 80, APP_START_ADDRESS + 81, APP_START_ADDRESS + 82, APP_START_ADDRESS + 83, APP_START_ADDRESS + 84, APP_START_ADDRESS + 85, APP_START_ADDRESS + 86, APP_START_ADDRESS + 87,
    APP_START_ADDRESS + 88, APP_START_ADDRESS + 89, APP_START_ADDRESS + 90, APP_START_ADDRESS + 91, APP_START_ADDRESS + 92, APP_START_ADDRESS + 93, APP_START_ADDRESS + 94, APP_START_ADDRESS + 95,
    APP_START_ADDRESS + 96, APP_START_ADDRESS + 97, APP_START_ADDRESS + 98, APP_START_ADDRESS + 99, APP_START_ADDRESS + 100, APP_START_ADDRESS + 101, APP_START_ADDRESS + 102, APP_START_ADDRESS + 103,
    APP_START_ADDRESS + 104, APP_START_ADDRESS + 105, APP_START_ADDRESS + 106, APP_START_ADDRESS + 107, APP_START_ADDRESS + 108, APP_START_ADDRESS + 109, APP_START_ADDRESS + 110, APP_START_ADDRESS + 111,
    APP_START_ADDRESS + 112, APP_START_ADDRESS + 113, APP_START_ADDRESS + 114, APP_START_ADDRESS + 115, APP_START_ADDRESS + 116, APP_START_ADDRESS + 117, APP_START_ADDRESS + 118, APP_START_ADDRESS + 119,
    APP_START_ADDRESS + 120, APP_START_ADDRESS + 121, APP_START_ADDRESS + 122, APP_START_ADDRESS + 123, APP_START_ADDRESS + 124, APP_START_ADDRESS + 125, APP_START_ADDRESS + 126, APP_START_ADDRESS + 127,
    APP_START_ADDRESS + 128, APP_START_ADDRESS + 129, APP_START_ADDRESS + 130, APP_START_ADDRESS + 131, APP_START_ADDRESS + 132, APP_START_ADDRESS + 133, APP_START_ADDRESS + 134, APP_START_ADDRESS + 135,
    APP_START_ADDRESS + 136, APP_START_ADDRESS + 137, APP_START_ADDRESS + 138, APP_START_ADDRESS + 139, APP_START_ADDRESS + 140, APP_START_ADDRESS + 141, APP_START_ADDRESS + 142, APP_START_ADDRESS + 143,
    APP_START_ADDRESS + 144, APP_START_ADDRESS + 145, APP_START_ADDRESS + 146, APP_START_ADDRESS + 147, APP_START_ADDRESS + 148, APP_START_ADDRESS + 149, APP_START_ADDRESS + 150, APP_START_ADDRESS + 151,
    APP_START_ADDRESS + 152, APP_START_ADDRESS + 153, APP_START_ADDRESS + 154, APP_START_ADDRESS + 155, APP_START_ADDRESS + 156, APP_START_ADDRESS + 157, APP_START_ADDRESS + 158, APP_START_ADDRESS + 159,
    APP_START_ADDRESS + 160, APP_START_ADDRESS + 161, APP_START_ADDRESS + 162, APP_START_ADDRESS + 163, APP_START_ADDRESS + 164, APP_START_ADDRESS + 165, APP_START_ADDRESS + 166, APP_START_ADDRESS + 167,
    APP_START_ADDRESS + 168, APP_START_ADDRESS + 169, APP_START_ADDRESS + 170, APP_START_ADDRESS + 171, APP_START_ADDRESS + 172, APP_START_ADDRESS + 173, APP_START_ADDRESS + 174, APP_START_ADDRESS + 175,
    APP_START_ADDRESS + 176, APP_START_ADDRESS + 177, APP_START_ADDRESS + 178, APP_START_ADDRESS + 179, APP_START_ADDRESS + 180, APP_START_ADDRESS + 181, APP_START_ADDRESS + 182, APP_START_ADDRESS + 183,
    APP_START_ADDRESS + 184, APP_START_ADDRESS + 185, APP_START_ADDRESS + 186, APP_START_ADDRESS + 187, APP_START_ADDRESS + 188, APP_START_ADDRESS + 189, APP_START_ADDRESS + 190, APP_START_ADDRESS + 191,
    APP_START_ADDRESS + 192, APP_START_ADDRESS + 193, APP_START_ADDRESS + 194, APP_START_ADDRESS + 195, APP_START_ADDRESS + 196, APP_START_ADDRESS + 197, APP_START_ADDRESS + 198, APP_START_ADDRESS + 199,
    APP_START_ADDRESS + 200, APP_START_ADDRESS + 201, APP_START_ADDRESS + 202, APP_START_ADDRESS + 203, APP_START_ADDRESS + 204, APP_START_ADDRESS + 205, APP_START_ADDRESS + 206, APP_START_ADDRESS + 207,
    APP_START_ADDRESS + 208, APP_START_ADDRESS + 209, APP_START_ADDRESS + 210, APP_START_ADDRESS + 211, APP_START_ADDRESS + 212, APP_START_ADDRESS + 213, APP_START_ADDRESS + 214, APP_START_ADDRESS + 215,
    APP_START_ADDRESS + 216, APP_START_ADDRESS + 217, APP_START_ADDRESS + 218, APP_START_ADDRESS + 219, APP_START_ADDRESS + 220, APP_START_ADDRESS + 221, APP_START_ADDRESS + 222, APP_START_ADDRESS + 223,
    APP_START_ADDRESS + 224, APP_START_ADDRESS + 225, APP_START_ADDRESS + 226, APP_START_ADDRESS + 227, APP_START_ADDRESS + 228, APP_START_ADDRESS + 229, APP_START_ADDRESS + 230, APP_START_ADDRESS + 231,
    APP_START_ADDRESS + 232, APP_START_ADDRESS + 233, APP_START_ADDRESS + 234, APP_START_ADDRESS + 235, APP_START_ADDRESS + 236, APP_START_ADDRESS + 237, APP_START_ADDRESS + 238, APP_START_ADDRESS + 239,
    APP_START_ADDRESS + 240, APP_START_ADDRESS + 241, APP_START_ADDRESS + 242, APP_START_ADDRESS + 243, APP_START_ADDRESS + 244, APP_START_ADDRESS + 245, APP_START_ADDRESS + 246, APP_START_ADDRESS + 247,
    APP_START_ADDRESS + 248, APP_START_ADDRESS + 249, APP_START_ADDRESS + 250, APP_START_ADDRESS + 251, APP_START_ADDRESS + 252, APP_START_ADDRESS + 253, APP_START_ADDRESS + 254, APP_START_ADDRESS + 255,
    APP_START_ADDRESS + 256, APP_START_ADDRESS + 257, APP_START_ADDRESS + 258, APP_START_ADDRESS + 259, APP_START_ADDRESS + 260, APP_START_ADDRESS + 261, APP_START_ADDRESS + 262, APP_START_ADDRESS + 263,
    APP_START_ADDRESS + 264, APP_START_ADDRESS + 265, APP_START_ADDRESS + 266, APP_START_ADDRESS + 267, APP_START_ADDRESS + 268, APP_START_ADDRESS + 269, APP_START_ADDRESS + 270, APP_START_ADDRESS + 271,
    APP_START_ADDRESS + 272, APP_START_ADDRESS + 273, APP_START_ADDRESS + 274, APP_START_ADDRESS + 275, APP_START_ADDRESS + 276, APP_START_ADDRESS + 277, APP_START_ADDRESS + 278, APP_START_ADDRESS + 279,
    APP_START_ADDRESS + 280, APP_START_ADDRESS + 281, APP_START_ADDRESS + 282, APP_START_ADDRESS + 283, APP_START_ADDRESS + 284, APP_START_ADDRESS + 285, APP_START_ADDRESS + 286, APP_START_ADDRESS + 287,
    APP_START_ADDRESS + 288, APP_START_ADDRESS + 289, APP_START_ADDRESS + 290, APP_START_ADDRESS + 291, APP_START_ADDRESS + 292, APP_START_ADDRESS + 293, APP_START_ADDRESS + 294, APP_START_ADDRESS + 295,
    APP_START_ADDRESS + 296, APP_START_ADDRESS + 297, APP_START_ADDRESS + 298, APP_START_ADDRESS + 299, APP_START_ADDRESS + 300, APP_START_ADDRESS + 301, APP_START_ADDRESS + 302, APP_START_ADDRESS + 303,
    APP_START_ADDRESS + 304, APP_START_ADDRESS + 305, APP_START_ADDRESS + 306, APP_START_ADDRESS + 307, APP_START_ADDRESS + 308, APP_START_ADDRESS + 309, APP_START_ADDRESS + 310, APP_START_ADDRESS + 311,
    APP_START_ADDRESS + 312, APP_START_ADDRESS + 313, APP_START_ADDRESS + 314, APP_START_ADDRESS + 315, APP_START_ADDRESS + 316, APP_START_ADDRESS + 317, APP_START_ADDRESS + 318, APP_START_ADDRESS + 319,
    APP_START_ADDRESS + 320, APP_START_ADDRESS + 321, APP_START_ADDRESS + 322, APP_START_ADDRESS + 323, APP_START_ADDRESS + 324, APP_START_ADDRESS + 325, APP_START_ADDRESS + 326, APP_START_ADDRESS + 327,
    APP_START_ADDRESS + 328, APP_START_ADDRESS + 329, APP_START_ADDRESS + 330, APP_START_ADDRESS + 331, APP_START_ADDRESS + 332, APP_START_ADDRESS + 333, APP_START_ADDRESS + 334, APP_START_ADDRESS + 335,
    APP_START_ADDRESS + 336, APP_START_ADDRESS + 337, APP_START_ADDRESS + 338, APP_START_ADDRESS + 339, APP_START_ADDRESS + 340, APP_START_ADDRESS + 341, APP_START_ADDRESS + 342, APP_START_ADDRESS + 343,
    APP_START_ADDRESS + 344, APP_START_ADDRESS + 345, APP_START_ADDRESS + 346, APP_START_ADDRESS + 347, APP_START_ADDRESS + 348, APP_START_ADDRESS + 349, APP_START_ADDRESS + 350, APP_START_ADDRESS + 351,
    APP_START_ADDRESS + 352, APP_START_ADDRESS + 353, APP_START_ADDRESS + 354, APP_START_ADDRESS + 355, APP_START_ADDRESS + 356, APP_START_ADDRESS + 357, APP_START_ADDRESS + 358, APP_START_ADDRESS + 359,
    APP_START_ADDRESS + 360, APP_START_ADDRESS + 361, APP_START_ADDRESS + 362, APP_START_ADDRESS + 363, APP_START_ADDRESS + 364, APP_START_ADDRESS + 365, APP_START_ADDRESS + 366, APP_START_ADDRESS + 367,
    APP_START_ADDRESS + 368, APP_START_ADDRESS + 369, APP_START_ADDRESS + 370, APP_START_ADDRESS + 371, APP_START_ADDRESS + 372, APP_START_ADDRESS + 373, APP_START_ADDRESS + 374, APP_START_ADDRESS + 375,
    APP_START_ADDRESS + 376, APP_START_ADDRESS + 377, APP_START_ADDRESS + 378, APP_START_ADDRESS + 379, APP_START_ADDRESS + 380, APP_START_ADDRESS + 381, APP_START_ADDRESS + 382, APP_START_ADDRESS + 383,
    APP_START_ADDRESS + 384, APP_START_ADDRESS + 385, APP_START_ADDRESS + 386, APP_START_ADDRESS + 387, APP_START_ADDRESS + 388, APP_START_ADDRESS + 389, APP_START_ADDRESS + 390, APP_START_ADDRESS + 391,
    APP_START_ADDRESS + 392, APP_START_ADDRESS + 393, APP_START_ADDRESS + 394, APP_START_ADDRESS + 395, APP_START_ADDRESS + 396, APP_START_ADDRESS + 397, APP_START_ADDRESS + 398, APP_START_ADDRESS + 399,
    APP_START_ADDRESS + 400, APP_START_ADDRESS + 401, APP_START_ADDRESS + 402, APP_START_ADDRESS + 403, APP_START_ADDRESS + 404, APP_START_ADDRESS + 405, APP_START_ADDRESS + 406, APP_START_ADDRESS + 407,
    APP_START_ADDRESS + 408, APP_START_ADDRESS + 409, APP_START_ADDRESS + 410, APP_START_ADDRESS + 411, APP_START_ADDRESS + 412, APP_START_ADDRESS + 413, APP_START_ADDRESS + 414, APP_START_ADDRESS + 415,
    APP_START_ADDRESS + 416, APP_START_ADDRESS + 417, APP_START_ADDRESS + 418, APP_START_ADDRESS + 419, APP_START_ADDRESS + 420, APP_START_ADDRESS + 421, APP_START_ADDRESS + 422, APP_START_ADDRESS + 423,
    APP_START_ADDRESS + 424, APP_START_ADDRESS + 425, APP_START_ADDRESS + 426, APP_START_ADDRESS + 427, APP_START_ADDRESS + 428, APP_START_ADDRESS + 429, APP_START_ADDRESS + 430, APP_START_ADDRESS + 431,
    APP_START_ADDRESS + 432, APP_START_ADDRESS + 433, APP_START_ADDRESS + 434, APP_START_ADDRESS + 435, APP_START_ADDRESS + 436, APP_START_ADDRESS + 437, APP_START_ADDRESS + 438, APP_START_ADDRESS + 439,
    APP_START_ADDRESS + 440, APP_START_ADDRESS + 441, APP_START_ADDRESS + 442, APP_START_ADDRESS + 443, APP_START_ADDRESS + 444, APP_START_ADDRESS + 445, APP_START_ADDRESS + 446, APP_START_ADDRESS + 447,
    APP_START_ADDRESS + 448, APP_START_ADDRESS + 449, APP_START_ADDRESS + 450, APP_START_ADDRESS + 451, APP_START_ADDRESS + 452, APP_START_ADDRESS + 453, APP_START_ADDRESS + 454, APP_START_ADDRESS + 455,
    APP_START_ADDRESS + 456, APP_START_ADDRESS + 457, APP_START_ADDRESS + 458, APP_START_ADDRESS + 459, APP_START_ADDRESS + 460, APP_START_ADDRESS + 461, APP_START_ADDRESS + 462, APP_START_ADDRESS + 463,
    APP_START_ADDRESS + 464, APP_START_ADDRESS + 465, APP_START_ADDRESS + 466, APP_START_ADDRESS + 467, APP_START_ADDRESS + 468, APP_START_ADDRESS + 469, APP_START_ADDRESS + 470, APP_START_ADDRESS + 471,
    APP_START_ADDRESS + 472, APP_START_ADDRESS + 473, APP_START_ADDRESS + 474, APP_START_ADDRESS + 475, APP_START_ADDRESS + 476, APP_START_ADDRESS + 477, APP_START_ADDRESS + 478, APP_START_ADDRESS + 479,
    APP_START_ADDRESS + 480, APP_START_ADDRESS + 481, APP_START_ADDRESS + 482, APP_START_ADDRESS + 483, APP_START_ADDRESS + 484, APP_START_ADDRESS + 485, APP_START_ADDRESS + 486, APP_START_ADDRESS + 487,
    APP_START_ADDRESS + 488, APP_START_ADDRESS + 489, APP_START_ADDRESS + 490, APP_START_ADDRESS + 491, APP_START_ADDRESS + 492, APP_START_ADDRESS + 493, APP_START_ADDRESS + 494, APP_START_ADDRESS + 495,
    APP_START_ADDRESS + 496, APP_START_ADDRESS + 497, APP_START_ADDRESS + 498, APP_START_ADDRESS + 499, APP_START_ADDRESS + 500, APP_START_ADDRESS + 501, APP_START_ADDRESS + 502, APP_START_ADDRESS + 503,
    APP_START_ADDRESS + 504, APP_START_ADDRESS + 505, APP_START_ADDRESS + 506, APP_START_ADDRESS + 507, APP_START_ADDRESS + 508, APP_START_ADDRESS + 509, APP_START_ADDRESS + 510, APP_START_ADDRESS + 511
};

    EraseFlash(APP_START_ADDRESS);
    WriteFlash(pData, APP_START_ADDRESS, 512);
    VerifyFlashWrite(pData, APP_START_ADDRESS, 512);

    Application_Init();

    while (1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
        HAL_Delay(500);
    }
}

void EraseFlash(uint32_t startAddress)
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef eraseInitStruct;
    uint32_t pageError = 0;

    eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInitStruct.Banks = FLASH_BANK_1;
    eraseInitStruct.Page = (startAddress - FLASH_START_ADDRESS) / FLASH_PAGE_SIZE;
    eraseInitStruct.NbPages = (END_ADDRESS - startAddress) / FLASH_PAGE_SIZE + 1;

    if (HAL_FLASHEx_Erase(&eraseInitStruct, &pageError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        Error_Handler();
    }

    HAL_FLASH_Lock();
}

HAL_StatusTypeDef WriteFlash(uint32_t* pData, uint32_t startAddress, uint32_t dataLength)
{
    HAL_StatusTypeDef status = HAL_OK;
    HAL_FLASH_Unlock();

    for (uint32_t i = 0; i < dataLength; i += 2)
    {
        uint64_t data64 = ((uint64_t)pData[i + 1] << 32) | pData[i];

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, startAddress + (i * 4), data64) != HAL_OK)
        {
            status = HAL_ERROR;
            break;
        }
    }

    HAL_FLASH_Lock();
    return status;
}
void ReadFlash(uint32_t* pData, uint32_t startAddress, uint32_t dataLength)
{
    memcpy(pData, (uint32_t*)startAddress, dataLength * sizeof(uint32_t));
}

void VerifyFlashWrite(uint32_t* pData, uint32_t startAddress, uint32_t dataLength)
{
    uint32_t readData[512];

    ReadFlash(readData, startAddress, dataLength);

    if (memcmp(pData, readData, dataLength) == 0)
    {
        char *msg = "Flash erased successfully.\r\n";
        HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    }
    else
    {
        char *msg = "Flash erase failed.\r\n";
        HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    }
}

void Application_Init(void)
{
    char *msg = "Application Started\r\n";
    HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    CheckPassword();
}

void CheckPassword(void)
{
    char password[] = "1234";
    char input[5];
    char *prompt = "Enter password:\r\n";
    HAL_UART_Transmit(&hlpuart1, (uint8_t*)prompt, strlen(prompt), HAL_MAX_DELAY);

    HAL_UART_Receive(&hlpuart1, (uint8_t*)input, sizeof(input), HAL_MAX_DELAY);
    input[strcspn(input, "\r\n")] = 0;

    if (strcmp(input, password) == 0)
    {
        char *success = "Password correct. Enter command:\r\n";
        HAL_UART_Transmit(&hlpuart1, (uint8_t*)success, strlen(success), HAL_MAX_DELAY);
        ProcessCommand();
    }
    else
    {
        char *fail = "Password incorrect. Try again:\r\n";
        HAL_UART_Transmit(&hlpuart1, (uint8_t*)fail, strlen(fail), HAL_MAX_DELAY);
        CheckPassword();
    }
}

void ProcessCommand(void)
{
    char input[5];
    HAL_UART_Receive(&hlpuart1, (uint8_t*)input, sizeof(input), HAL_MAX_DELAY);
    input[strcspn(input, "\r\n")] = 0;

    if (strcmp(input, "led on") == 0)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
        char *msg = "LED turned on.\r\n";
        HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    }
    else if (strcmp(input, "led off") == 0)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
        char *msg = "LED turned off.\r\n";
        HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    }
    else if (strcmp(input, "boot") == 0)
    {
        char *msg = "Updating bootloader...\r\n";
        HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        HAL_Delay(100);
        // Update the application start address here, if needed
    }
    else
    {
        char *msg = "Unknown command.\r\n";
        HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}

static void LED_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}
static void UART_Init(void)
{
    hlpuart1.Instance = LPUART1;
    hlpuart1.Init.BaudRate = 115200;
    hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
    hlpuart1.Init.StopBits = UART_STOPBITS_1;
    hlpuart1.Init.Parity = UART_PARITY_NONE;
    hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    hlpuart1.Init.Mode = UART_MODE_RX | UART_MODE_TX;
    hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
    hlpuart1.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;

    if (HAL_UART_Init(&hlpuart1) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_RCC_CRC_CLK_ENABLE();

    CrcHandle.Instance = CRC;
    CrcHandle.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
    CrcHandle.Init.GeneratingPolynomial = 0x1021;
    CrcHandle.Init.CRCLength = CRC_POLYLENGTH_16B;
    CrcHandle.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
    CrcHandle.Init.InitValue = 0;
    CrcHandle.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
    CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
    CrcHandle.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;

    if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
    {
        while (1) {}
    }
}

void Error_Handler(void)
{
    while (1)
    {
    }
}
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    while (1)
    {
    }
}
#endif /* USE_FULL_ASSERT */
