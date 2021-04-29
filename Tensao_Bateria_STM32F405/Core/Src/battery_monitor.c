/*
 * battery_monitor.c
 *
 *  Created on: Apr 22, 2021
 *  Author: Germano Henrique
 *  Brief: Declare the functions used in the main file.
 */

#include "battery_monitor.h"
#include "stm32f4xx_hal.h"

#define MAX_RESOLUTION_ADC 4095	// 12 bits resolution
#define RESISTOR1 10000U
#define RESISTOR2 3400U
#define VOLTAGE_DIVIDER_RATIO ((RESISTOR1 + RESISTOR2) / (float)(RESISTOR2)) // ratio between Vin and Vout
#define MAX_OUTPUT_VOLTAGE_ON_ADC 3.3
#define OFFSET 0.1 // offset value used to correct final voltage

static void config_vbat_reader(void);

ADC_HandleTypeDef hadc1;

// starts and configures the peripherals that are to be used
void init_battery_monitor(void)
{
	config_vbat_reader();
	HAL_ADC_Start(&hadc1);
}

// returns the voltage of a battery
float get_battery_voltage (void)
{
	// inicializa a conversão em PC14
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

	uint16_t adc_return_value = HAL_ADC_GetValue(&hadc1);
	float voltage_on_adc_pin = adc_return_value *  MAX_OUTPUT_VOLTAGE_ON_ADC / MAX_RESOLUTION_ADC;
	float battery_voltage = (VOLTAGE_DIVIDER_RATIO * voltage_on_adc_pin) - OFFSET;

	return battery_voltage;
}

/**
  VBAT (PC14) peripheral initialization
  */
static void config_vbat_reader(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    while(1);
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
	  while(1);
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}
