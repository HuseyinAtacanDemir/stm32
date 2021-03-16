/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

//morse code timing rules
//a dot is one time unit
//a dash is three time units
//time between two symbols is one time unit
//time between two letters is three time units
//time between two words is seven time units

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>

uint8_t count = 0;
const uint8_t dotDuration = 50;

char *msg = "angular js sucks";
size_t len = 0;

struct node {
    char data;
    struct node* left;
    struct node* right;
};

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

struct node* newNode(char data){
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

struct node* createMorseTree(){
	struct node* root = newNode(0);
	root->left = newNode(69);
	root->left->left = newNode(73);
	root->left->right = newNode(65);
	root->left->left->left = newNode(83);
	root->left->left->right = newNode(85);
	root->left->right->left = newNode(82);
	root->left->right->right = newNode(87);
	root->left->left->left->left = newNode(72);
	root->left->left->left->right = newNode(86);
	root->left->left->right->left = newNode(70);
	root->left->right->left->left = newNode(76);
	root->left->right->right->left = newNode(80);
	root->left->right->right->right = newNode(74);
	root->right = newNode(84);
	root->right->right = newNode(77);
	root->right->left = newNode(78);
	root->right->right->right = newNode(79);
	root->right->right->left = newNode(71);
	root->right->left->right = newNode(75);
	root->right->left->left = newNode(68);
	root->right->right->left->right = newNode(81);
	root->right->right->left->left = newNode(90);
	root->right->left->right->right = newNode(89);
	root->right->left->right->left = newNode(67);
	root->right->left->left->right = newNode(88);
	root->right->left->left->left = newNode(66);

	return root;
}
void deleteMorseTree(struct node* n){
	if(!(n->left) && !(n->right)){
		free(n);
		return;
	}
	deleteMorseTree(n->left);
	deleteMorseTree(n->right);
	return;
}
void flash(char duration){

	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
	if(duration == 1){
		HAL_Delay(dotDuration*3);
	}else{
		HAL_Delay(dotDuration);
	}

	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
	HAL_Delay(dotDuration);
}
char decode(char* c, struct node* n){
	if(n->data == *c){ return 1; }
	if(!(n->left) && !(n->right)){ return 0; }
	if(n->right){
		char r = decode(c, n->right);
		if(r == 1){
			flash(1);
			return 1;
		}
	}
	if(n->left){
		char l = decode(c, n->left);
		if(l == 1){
			flash(0);
			return 1;
		}
	}
	return 0;
}
void morse(char* msg, struct node* root){
	for(size_t i = 0; i < len; i++){
		char current = *(msg+i);
		if(*(msg+i) > 91){
				current -= 32;
		}
		char space = decode(&current, root);
		if(space == 0){
			HAL_Delay(dotDuration*7);
		}else{
			HAL_Delay(dotDuration*3);
		}
	}
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void){

	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	struct node* root = createMorseTree();
	len = strlen(msg);

	while(count < 8){
		morse(msg, root);
		HAL_Delay(dotDuration*7);
		count++;
	}

	deleteMorseTree(root);

	return 0;

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
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
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
