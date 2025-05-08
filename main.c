/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "pitches.h"
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIM_FREQ 60000000
#define ARR 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t uart_rx;
uint8_t play_music = 0;
uint8_t song_index = 0; // 0 = Mario, 1 = Zelda



// TEMA DE MARIO
int melody[] = {
659, 659, 659, 0, 523, 659, 784, 392, 523, 0, 392, 0, 330, 0, 440, 0, 494, 0, 466, 440, 392, 659, 784, 880, 698, 784, 0, 659, 0, 523, 587, 494, 0, 523, 0, 392, 0, 330, 0, 440, 0, 494, 0, 466, 440, 392, 659, 784, 880, 698, 784, 0, 659, 0, 523, 587, 494, 0, 0, 784, 740, 698, 622, 659, 0, 415, 440, 523, 0, 440, 523, 587, 0, 784, 740, 698, 622, 659, 0, 698, 1047, 0, 698, 1047, 698, 1047, 0, 784, 740, 698, 622, 659, 0, 415, 440, 523, 0, 440, 523, 587, 0, 622, 0, 587, 0, 523, 0, 0, 784, 740, 698, 622, 659, 0, 415, 440, 523, 0, 440, 523, 587, 0, 784, 740, 698, 622, 659, 0, 698, 1047, 0, 698, 1047, 698, 1047, 0, 784, 740, 698, 622, 659, 0, 415, 440, 523, 0, 440, 523, 587, 0, 622, 0, 587, 0, 523, 0, 523, 523, 523, 0, 523, 587, 659, 523, 440, 392, 523, 523, 523, 0, 523, 587, 659, 0, 523, 523, 523, 0, 523, 587, 659, 523, 440, 392, 659, 659, 659, 0, 523, 659, 784, 392, 523, 0, 392, 0, 330, 0, 440, 0, 494, 0, 466, 440, 392, 659, 784, 880, 698, 784, 0, 659, 0, 523, 587, 494, 0, 523, 0, 392, 0, 330, 0, 440, 0, 494, 0, 466, 440, 392, 659, 784, 880, 698, 784, 0, 659, 0, 523, 587, 494, 0, 659, 523, 392, 0, 415, 440, 698, 698, 440, 494, 880, 880, 880, 784, 698, 659, 523, 440, 392, 659, 523, 392, 0, 415, 440, 698, 698, 440, 494, 698, 698, 698, 659, 587, 523, 392, 392, 262, 523, 523, 523, 0, 523, 587, 659, 523, 440, 392, 523, 523, 523, 0, 523, 587, 659, 0, 523, 523, 523, 0, 523, 587, 659, 523, 440, 392, 659, 659, 659, 0, 523, 659, 784, 392, 659, 523, 392, 0, 415, 440, 698, 698, 440, 494, 880, 880, 880, 784, 698, 659, 523, 440, 392, 659, 523, 392, 0, 415, 440, 698, 698, 440, 494, 698, 698, 698, 659, 587, 523, 392, 392, 262, 0
};
int noteDurations[] = {
126, 252, 126, 126, 126, 252, 504, 504, 252, 126, 126, 252, 252, 126, 126, 126, 126, 126, 126, 252, 168, 168, 168, 252, 126, 126, 126, 126, 126, 126, 126, 126, 252, 252, 126, 126, 252, 252, 126, 126, 126, 126, 126, 126, 252, 168, 168, 168, 252, 126, 126, 126, 126, 126, 126, 126, 126, 252, 252, 126, 126, 126, 252, 126, 126, 126, 126, 126, 126, 126, 126, 126, 252, 126, 126, 126, 252, 126, 126, 126, 126, 126, 126, 126, 504, 504, 252, 126, 126, 126, 252, 126, 126, 126, 126, 126, 126, 126, 126, 126, 252, 252, 126, 126, 252, 504, 504, 252, 126, 126, 126, 252, 126, 126, 126, 126, 126, 126, 126, 126, 126, 252, 126, 126, 126, 252, 126, 126, 126, 126, 126, 126, 126, 504, 504, 252, 126, 126, 126, 252, 126, 126, 126, 126, 126, 126, 126, 126, 126, 252, 252, 126, 126, 252, 504, 504, 126, 252, 126, 126, 126, 252, 126, 252, 126, 504, 126, 252, 126, 126, 126, 126, 126, 1008, 126, 252, 126, 126, 126, 252, 126, 252, 126, 504, 126, 252, 126, 126, 126, 252, 504, 504, 252, 126, 126, 252, 252, 126, 126, 126, 126, 126, 126, 252, 168, 168, 168, 252, 126, 126, 126, 126, 126, 126, 126, 126, 252, 252, 126, 126, 252, 252, 126, 126, 126, 126, 126, 126, 252, 168, 168, 168, 252, 126, 126, 126, 126, 126, 126, 126, 126, 252, 126, 252, 126, 252, 252, 126, 252, 126, 504, 168, 168, 168, 168, 168, 168, 126, 252, 126, 504, 126, 252, 126, 252, 252, 126, 252, 126, 504, 126, 252, 126, 168, 168, 168, 126, 252, 126, 504, 126, 252, 126, 126, 126, 252, 126, 252, 126, 504, 126, 252, 126, 126, 126, 126, 126, 1008, 126, 252, 126, 126, 126, 252, 126, 252, 126, 504, 126, 252, 126, 126, 126, 252, 504, 504, 126, 252, 126, 252, 252, 126, 252, 126, 504, 168, 168, 168, 168, 168, 168, 126, 252, 126, 504, 126, 252, 126, 252, 252, 126, 252, 126, 504, 126, 252, 126, 168, 168, 168, 126, 252, 126, 504, 0
};


int melody_ZELDA[] = {
  // Main phrase (repeated twice)
  784, 740, 659, 659, 0, 659, 988, 784,
  740, 659, 740, 659, 587, 0, 587, 659,
  587, 523, 0, 523, 587, 523, 494, 0,

  784, 740, 659, 659, 0, 659, 988, 784,
  740, 659, 740, 659, 587, 0, 587, 659,
  587, 523, 0, 523, 587, 523, 494, 0,

  // Second section
  659, 659, 659, 0, 659, 659, 659, 0,
  587, 523, 494, 440, 0, 440, 494, 523,
  587, 0, 587, 659, 587, 523, 0, 523,
  587, 523, 494, 0,

  // Repeat main phrase
  784, 740, 659, 659, 0, 659, 988, 784,
  740, 659, 740, 659, 587, 0, 587, 659,
  587, 523, 0, 523, 587, 523, 494, 0,

  // Main phrase (repeated twice)
    784, 740, 659, 659, 0, 659, 988, 784,
    740, 659, 740, 659, 587, 0, 587, 659,
    587, 523, 0, 523, 587, 523, 494, 0,

    784, 740, 659, 659, 0, 659, 988, 784,
    740, 659, 740, 659, 587, 0, 587, 659,
    587, 523, 0, 523, 587, 523, 494, 0,

    // Second section
    659, 659, 659, 0, 659, 659, 659, 0,
    587, 523, 494, 440, 0, 440, 494, 523,
    587, 0, 587, 659, 587, 523, 0, 523,
    587, 523, 494, 0,

    // Repeat main phrase
    784, 740, 659, 659, 0, 659, 988, 784,
    740, 659, 740, 659, 587, 0, 587, 659,
    587, 523, 0, 523, 587, 523, 494, 0,

	// Main phrase (repeated twice)
	  784, 740, 659, 659, 0, 659, 988, 784,
	  740, 659, 740, 659, 587, 0, 587, 659,
	  587, 523, 0, 523, 587, 523, 494, 0,

	  784, 740, 659, 659, 0, 659, 988, 784,
	  740, 659, 740, 659, 587, 0, 587, 659,
	  587, 523, 0, 523, 587, 523, 494, 0,

	  // Second section
	  659, 659, 659, 0, 659, 659, 659, 0,
	  587, 523, 494, 440, 0, 440, 494, 523,
	  587, 0, 587, 659, 587, 523, 0, 523,
	  587, 523, 494, 0,

	  // Repeat main phrase
	  784, 740, 659, 659, 0, 659, 988, 784,
	  740, 659, 740, 659, 587, 0, 587, 659,
	  587, 523, 0, 523, 587, 523, 494, 0,

	  // Main phrase (repeated twice)
	    784, 740, 659, 659, 0, 659, 988, 784,
	    740, 659, 740, 659, 587, 0, 587, 659,
	    587, 523, 0, 523, 587, 523, 494, 0,

	    784, 740, 659, 659, 0, 659, 988, 784,
	    740, 659, 740, 659, 587, 0, 587, 659,
	    587, 523, 0, 523, 587, 523, 494, 0,

	    // Second section
	    659, 659, 659, 0, 659, 659, 659, 0,
	    587, 523, 494, 440, 0, 440, 494, 523,
	    587, 0, 587, 659, 587, 523, 0, 523,
	    587, 523, 494, 0,

	    // Repeat main phrase
	    784, 740, 659, 659, 0, 659, 988, 784,
	    740, 659, 740, 659, 587, 0, 587, 659,
	    587, 523, 0, 523, 587, 523, 494, 0,

	    // Main phrase (repeated twice)
	      784, 740, 659, 659, 0, 659, 988, 784,
	      740, 659, 740, 659, 587, 0, 587, 659,
	      587, 523, 0, 523, 587, 523, 494, 0,

	      784, 740, 659, 659, 0, 659, 988, 784,
	      740, 659, 740, 659, 587, 0, 587, 659,
	      587, 523, 0, 523, 587, 523, 494, 0,

	      // Second section
	      659, 659, 659, 0, 659, 659, 659, 0,
	      587, 523, 494, 440, 0, 440, 494, 523,
	      587, 0, 587, 659, 587, 523, 0, 523,
	      587, 523, 494, 0,

	      // Repeat main phrase
	      784, 740, 659, 659, 0, 659, 988, 784,
	      740, 659, 740, 659, 587, 0, 587, 659,
	      587, 523, 0, 523, 587, 523, 494, 0,

	  	// Main phrase (repeated twice)
	  	  784, 740, 659, 659, 0, 659, 988, 784,
	  	  740, 659, 740, 659, 587, 0, 587, 659,
	  	  587, 523, 0, 523, 587, 523, 494, 0,

	  	  784, 740, 659, 659, 0, 659, 988, 784,
	  	  740, 659, 740, 659, 587, 0, 587, 659,
	  	  587, 523, 0, 523, 587, 523, 494, 0,

	  	  // Second section
	  	  659, 659, 659, 0, 659, 659, 659, 0,
	  	  587, 523, 494, 440, 0, 440, 494, 523,
	  	  587, 0, 587, 659, 587, 523, 0, 523,
	  	  587, 523, 494, 0,

	  	  // Repeat main phrase
	  	  784, 740, 659, 659, 0, 659, 988, 784,
	  	  740, 659, 740, 659, 587, 0, 587, 659,
	  	  587, 523, 0, 523, 587, 523, 494, 0,

		  // Main phrase (repeated twice)
		    784, 740, 659, 659, 0, 659, 988, 784,
		    740, 659, 740, 659, 587, 0, 587, 659,
		    587, 523, 0, 523, 587, 523, 494, 0,

		    784, 740, 659, 659, 0, 659, 988, 784,
		    740, 659, 740, 659, 587, 0, 587, 659,
		    587, 523, 0, 523, 587, 523, 494, 0,

		    // Second section
		    659, 659, 659, 0, 659, 659, 659, 0,
		    587, 523, 494, 440, 0, 440, 494, 523,
		    587, 0, 587, 659, 587, 523, 0, 523,
		    587, 523, 494, 0,

		    // Repeat main phrase
		    784, 740, 659, 659, 0, 659, 988, 784,
		    740, 659, 740, 659, 587, 0, 587, 659,
		    587, 523, 0, 523, 587, 523, 494, 0,

		    // Main phrase (repeated twice)
		      784, 740, 659, 659, 0, 659, 988, 784,
		      740, 659, 740, 659, 587, 0, 587, 659,
		      587, 523, 0, 523, 587, 523, 494, 0,

		      784, 740, 659, 659, 0, 659, 988, 784,
		      740, 659, 740, 659, 587, 0, 587, 659,
		      587, 523, 0, 523, 587, 523, 494, 0,

		      // Second section
		      659, 659, 659, 0, 659, 659, 659, 0,
		      587, 523, 494, 440, 0, 440, 494, 523,
		      587, 0, 587, 659, 587, 523, 0, 523,
		      587, 523, 494, 0,

		      // Repeat main phrase
		      784, 740, 659, 659, 0, 659, 988, 784,
		      740, 659, 740, 659, 587, 0, 587, 659,
		      587, 523, 0, 523, 587, 523, 494, 0,

		  	// Main phrase (repeated twice)
		  	  784, 740, 659, 659, 0, 659, 988, 784,
		  	  740, 659, 740, 659, 587, 0, 587, 659,
		  	  587, 523, 0, 523, 587, 523, 494, 0,

		  	  784, 740, 659, 659, 0, 659, 988, 784,
		  	  740, 659, 740, 659, 587, 0, 587, 659,
		  	  587, 523, 0, 523, 587, 523, 494, 0,

		  	  // Second section
		  	  659, 659, 659, 0, 659, 659, 659, 0,
		  	  587, 523, 494, 440, 0, 440, 494, 523,
		  	  587, 0, 587, 659, 587, 523, 0, 523,
		  	  587, 523, 494, 0,

		  	  // Repeat main phrase
		  	  784, 740, 659, 659, 0, 659, 988, 784,
		  	  740, 659, 740, 659, 587, 0, 587, 659,
		  	  587, 523, 0, 523, 587, 523, 494, 0
};

int noteDurations_ZELDA[] = {
  // Main phrase durations (eighth notes, quarter notes, etc.)
  250, 250, 250, 250, 250, 250, 500, 500,
  250, 250, 250, 250, 500, 250, 250, 250,
  250, 500, 250, 250, 250, 250, 500, 250,

  // Repeat
  250, 250, 250, 250, 250, 250, 500, 500,
  250, 250, 250, 250, 500, 250, 250, 250,
  250, 500, 250, 250, 250, 250, 500, 250,

  // Second section
  250, 250, 250, 250, 250, 250, 250, 250,
  250, 250, 250, 250, 250, 250, 250, 250,
  500, 250, 250, 250, 250, 500, 250, 250,
  250, 250, 500, 250,

  // Repeat main phrase
  250, 250, 250, 250, 250, 250, 500, 500,
  250, 250, 250, 250, 500, 250, 250, 250,
  250, 500, 250, 250, 250, 250, 500, 250,

  // Main phrase durations (eighth notes, quarter notes, etc.)
    250, 250, 250, 250, 250, 250, 500, 500,
    250, 250, 250, 250, 500, 250, 250, 250,
    250, 500, 250, 250, 250, 250, 500, 250,

    // Repeat
    250, 250, 250, 250, 250, 250, 500, 500,
    250, 250, 250, 250, 500, 250, 250, 250,
    250, 500, 250, 250, 250, 250, 500, 250,

    // Second section
    250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250, 250,
    500, 250, 250, 250, 250, 500, 250, 250,
    250, 250, 500, 250,

    // Repeat main phrase
    250, 250, 250, 250, 250, 250, 500, 500,
    250, 250, 250, 250, 500, 250, 250, 250,
    250, 500, 250, 250, 250, 250, 500, 250,

	// Main phrase durations (eighth notes, quarter notes, etc.)
	  250, 250, 250, 250, 250, 250, 500, 500,
	  250, 250, 250, 250, 500, 250, 250, 250,
	  250, 500, 250, 250, 250, 250, 500, 250,

	  // Repeat
	  250, 250, 250, 250, 250, 250, 500, 500,
	  250, 250, 250, 250, 500, 250, 250, 250,
	  250, 500, 250, 250, 250, 250, 500, 250,

	  // Second section
	  250, 250, 250, 250, 250, 250, 250, 250,
	  250, 250, 250, 250, 250, 250, 250, 250,
	  500, 250, 250, 250, 250, 500, 250, 250,
	  250, 250, 500, 250,

	  // Repeat main phrase
	  250, 250, 250, 250, 250, 250, 500, 500,
	  250, 250, 250, 250, 500, 250, 250, 250,
	  250, 500, 250, 250, 250, 250, 500, 250,

	  // Main phrase durations (eighth notes, quarter notes, etc.)
	    250, 250, 250, 250, 250, 250, 500, 500,
	    250, 250, 250, 250, 500, 250, 250, 250,
	    250, 500, 250, 250, 250, 250, 500, 250,

	    // Repeat
	    250, 250, 250, 250, 250, 250, 500, 500,
	    250, 250, 250, 250, 500, 250, 250, 250,
	    250, 500, 250, 250, 250, 250, 500, 250,

	    // Second section
	    250, 250, 250, 250, 250, 250, 250, 250,
	    250, 250, 250, 250, 250, 250, 250, 250,
	    500, 250, 250, 250, 250, 500, 250, 250,
	    250, 250, 500, 250,

	    // Repeat main phrase
	    250, 250, 250, 250, 250, 250, 500, 500,
	    250, 250, 250, 250, 500, 250, 250, 250,
	    250, 500, 250, 250, 250, 250, 500, 250,

	    // Main phrase durations (eighth notes, quarter notes, etc.)
	      250, 250, 250, 250, 250, 250, 500, 500,
	      250, 250, 250, 250, 500, 250, 250, 250,
	      250, 500, 250, 250, 250, 250, 500, 250,

	      // Repeat
	      250, 250, 250, 250, 250, 250, 500, 500,
	      250, 250, 250, 250, 500, 250, 250, 250,
	      250, 500, 250, 250, 250, 250, 500, 250,

	      // Second section
	      250, 250, 250, 250, 250, 250, 250, 250,
	      250, 250, 250, 250, 250, 250, 250, 250,
	      500, 250, 250, 250, 250, 500, 250, 250,
	      250, 250, 500, 250,

	      // Repeat main phrase
	      250, 250, 250, 250, 250, 250, 500, 500,
	      250, 250, 250, 250, 500, 250, 250, 250,
	      250, 500, 250, 250, 250, 250, 500, 250,

	  	// Main phrase durations (eighth notes, quarter notes, etc.)
	  	  250, 250, 250, 250, 250, 250, 500, 500,
	  	  250, 250, 250, 250, 500, 250, 250, 250,
	  	  250, 500, 250, 250, 250, 250, 500, 250,

	  	  // Repeat
	  	  250, 250, 250, 250, 250, 250, 500, 500,
	  	  250, 250, 250, 250, 500, 250, 250, 250,
	  	  250, 500, 250, 250, 250, 250, 500, 250,

	  	  // Second section
	  	  250, 250, 250, 250, 250, 250, 250, 250,
	  	  250, 250, 250, 250, 250, 250, 250, 250,
	  	  500, 250, 250, 250, 250, 500, 250, 250,
	  	  250, 250, 500, 250,

	  	  // Repeat main phrase
	  	  250, 250, 250, 250, 250, 250, 500, 500,
	  	  250, 250, 250, 250, 500, 250, 250, 250,
	  	  250, 500, 250, 250, 250, 250, 500, 250,

		  // Main phrase durations (eighth notes, quarter notes, etc.)
		    250, 250, 250, 250, 250, 250, 500, 500,
		    250, 250, 250, 250, 500, 250, 250, 250,
		    250, 500, 250, 250, 250, 250, 500, 250,

		    // Repeat
		    250, 250, 250, 250, 250, 250, 500, 500,
		    250, 250, 250, 250, 500, 250, 250, 250,
		    250, 500, 250, 250, 250, 250, 500, 250,

		    // Second section
		    250, 250, 250, 250, 250, 250, 250, 250,
		    250, 250, 250, 250, 250, 250, 250, 250,
		    500, 250, 250, 250, 250, 500, 250, 250,
		    250, 250, 500, 250,

		    // Repeat main phrase
		    250, 250, 250, 250, 250, 250, 500, 500,
		    250, 250, 250, 250, 500, 250, 250, 250,
		    250, 500, 250, 250, 250, 250, 500, 250,

		    // Main phrase durations (eighth notes, quarter notes, etc.)
		      250, 250, 250, 250, 250, 250, 500, 500,
		      250, 250, 250, 250, 500, 250, 250, 250,
		      250, 500, 250, 250, 250, 250, 500, 250,

		      // Repeat
		      250, 250, 250, 250, 250, 250, 500, 500,
		      250, 250, 250, 250, 500, 250, 250, 250,
		      250, 500, 250, 250, 250, 250, 500, 250,

		      // Second section
		      250, 250, 250, 250, 250, 250, 250, 250,
		      250, 250, 250, 250, 250, 250, 250, 250,
		      500, 250, 250, 250, 250, 500, 250, 250,
		      250, 250, 500, 250,

		      // Repeat main phrase
		      250, 250, 250, 250, 250, 250, 500, 500,
		      250, 250, 250, 250, 500, 250, 250, 250,
		      250, 500, 250, 250, 250, 250, 500, 250,

		  	// Main phrase durations (eighth notes, quarter notes, etc.)
		  	  250, 250, 250, 250, 250, 250, 500, 500,
		  	  250, 250, 250, 250, 500, 250, 250, 250,
		  	  250, 500, 250, 250, 250, 250, 500, 250,

		  	  // Repeat
		  	  250, 250, 250, 250, 250, 250, 500, 500,
		  	  250, 250, 250, 250, 500, 250, 250, 250,
		  	  250, 500, 250, 250, 250, 250, 500, 250,

		  	  // Second section
		  	  250, 250, 250, 250, 250, 250, 250, 250,
		  	  250, 250, 250, 250, 250, 250, 250, 250,
		  	  500, 250, 250, 250, 250, 500, 250, 250,
		  	  250, 250, 500, 250,

		  	  // Repeat main phrase
		  	  250, 250, 250, 250, 250, 250, 500, 500,
		  	  250, 250, 250, 250, 500, 250, 250, 250,
		  	  250, 500, 250, 250, 250, 250, 500, 250
};

int melody_MII[] = {
  415, 494, 622, 0, 494, 0, 415, 330, 330, 330, 0, 311, 330, 415, 494, 622, 0, 494, 0, 415, 740, 0, 698, 659, 0, 466, 0, 622, 415, 0, 622, 0, 466, 0, 622, 0, 440, 415, 0, 370, 0, 294, 294, 294, 0, 0, 294, 294, 294, 0, 0, 330, 311, 0, 415, 494, 622, 0, 494, 0, 415, 370, 370, 370, 0, 740, 740, 740, 0, 0, 415, 494, 622, 0, 494, 0, 415, 622, 554, 0, 554, 440, 330, 311, 554, 440, 311, 494, 415, 294, 278, 494, 415, 278, 311, 311, 311, 0, 0, 262, 278, 311, 330, 415, 494, 0, 247, 0, 262, 0, 278, 262, 278, 0, 278, 262, 278, 415, 0, 311, 278, 262, 278, 278, 0, 278, 0, 294, 0, 311, 294, 311, 0, 311, 294, 311, 466, 0, 349, 311, 349, 278, 278, 466, 466, 466, 0, 0
  };
  int noteDurations_MII[] = {
  500, 250, 250, 250, 250, 250, 250, 250, 250, 250, 1000, 250, 250, 250, 250, 250, 250, 250, 250, 250, 500, 250, 250, 750, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 500, 250, 250, 250, 250, 250, 500, 500, 250, 250, 500, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 1000, 500, 500, 250, 250, 250, 500, 250, 250, 250, 250, 250, 250, 500, 250, 250, 250, 250, 250, 500, 500, 250, 250, 250, 250, 250, 250, 1000, 1000, 250, 250, 250, 250, 750, 250, 1000, 500, 250, 250, 250, 250, 250, 250, 750, 250, 1000, 500, 500, 250, 250, 250, 250, 750, 250, 1000, 500, 250, 250, 250, 250, 250, 250, 750, 250, 1000, 1000, 250, 250, 250, 250, 0
  };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

int presForFrequency(int frequency);
void playTone(int *tone, int *duration, int *pause, int size);
void noTone(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void printUART(char *msg)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}


int presForFrequency(int frequency){
	if (frequency == 0){
		return 0;
	}
	return ((TIM_FREQ / (ARR * frequency)) - 1);

}
/*void playTone(int *tone, int *duration, int *pause, int size){
	for (int i = 0; i < size; i++){
		int prescaler = presForFrequency(tone[i]);  // calcular el prescaler
		int dur = duration[i];  // obtener la duracion
		int pauseBetweenTones = 0;
		if (pause != NULL)
			pauseBetweenTones = pause[i] - duration[i];

		__HAL_TIM_SET_PRESCALER(&htim1, prescaler);
		HAL_Delay(dur);  // duracion nota
		noTone();  // pausa
		HAL_Delay(pauseBetweenTones); // duracion sin tono
	}
}*/
void playTone(int *tone, int *duration, int *pause, int size){
	for (int i = 0; i < size; i++){
		if (!play_music) break;  // ← interrupción inmediata de la música

		int prescaler = presForFrequency(tone[i]);
		int dur = duration[i];
		int pauseBetweenTones = 0;

		if (pause != NULL)
			pauseBetweenTones = pause[i] - dur;

		__HAL_TIM_SET_PRESCALER(&htim1, prescaler);
		HAL_Delay(dur);
		noTone();
		HAL_Delay(pauseBetweenTones);
	}
}
void playToneOnce(int *tone, int *duration, int *pause, int size){
	for (int i = 0; i < size; i++){
		int prescaler = presForFrequency(tone[i]);
		int dur = duration[i];
		int pauseBetweenTones = 0;

		if (pause != NULL)
			pauseBetweenTones = pause[i] - dur;

		__HAL_TIM_SET_PRESCALER(&htim1, prescaler);
		HAL_Delay(dur);
		noTone();
		HAL_Delay(pauseBetweenTones);
	}
}


void noTone(void){
	__HAL_TIM_SET_PRESCALER(&htim1, 0);  // prescaler 0

}
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
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2, &uart_rx, 1);  // USART2 (con terminal)
  HAL_UART_Receive_IT(&huart3, &uart_rx, 1);  // USART3 (solo comandos)

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
//  playTone(melody, noteDurations, NULL, (sizeof(melody)/sizeof(melody[0])));
//  noTone();
  while (1)
  {
	  while (play_music == 1)
	  {
	      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

	      while (play_music == 1)  // ← Loop infinito hasta que play_music sea 0
	      {
	          switch (song_index)
	          {
	              case 0:
	                  playTone(melody, noteDurations, NULL, sizeof(melody) / sizeof(melody[0]));
	                  break;
	              case 1:
	                  playTone(melody_ZELDA, noteDurations_ZELDA, NULL, sizeof(melody_ZELDA) / sizeof(melody_ZELDA[0]));
	                  break;
	              case 2:
	                  playTone(melody_MII, noteDurations_MII, NULL, sizeof(melody_MII) / sizeof(melody_MII[0]));
	                  break;
	          }
	          noTone();  // Pausa entre repeticiones
	          //HAL_Delay(200);  // Pequeño delay antes de repetir
	      }
	  }
    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 100-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 50;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART3)  // Solo si proviene de USART3
  {
    char msg[64];

    switch (uart_rx)
    {
      case 'H':
        play_music = 1;
        song_index = 0;
        sprintf(msg, "H -> Mario\r\n");
        break;

      case 'J':
        play_music = 1;
        song_index = 1;
        sprintf(msg, "J -> Zelda\r\n");
        break;

      case 'K':
        play_music = 1;
        song_index = 2;
        sprintf(msg, "K -> Mii\r\n");
        break;

      case 'h':
        play_music = 0;
        noTone();
        sprintf(msg, "h -> Música detenida\r\n");
        break;

      default:
        sprintf(msg, "recibi ->*%c\r\n", uart_rx);
        break;
    }

    // Mostrar lo que se recibió por UART3 en la terminal (USART2)
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    HAL_UART_Receive_IT(&huart3, &uart_rx, 1); // Reactiva solo USART3
  }
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
