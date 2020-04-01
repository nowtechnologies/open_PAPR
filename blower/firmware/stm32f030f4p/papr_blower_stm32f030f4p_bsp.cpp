#include "main.h"

extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim14;

namespace {
  constexpr uint32_t ADC_CHANNEL_COUNT = ((uint32_t)  4);
  constexpr uint32_t ADC_SAMPLE_COUNT = ((uint32_t)  4);
  constexpr uint32_t ADC_CONVERTED_DATA_BUFFER_SIZE =   ((uint32_t)  ADC_SAMPLE_COUNT * ADC_CHANNEL_COUNT);
  constexpr uint32_t cIndexAdcBattery =  0u;
  constexpr uint32_t cIndexAdcPotmeter = 1u;
  constexpr uint32_t cIndexAdcTemp =     2u;
  constexpr uint32_t cIndexAdcVref =     3u;
}

volatile uint16_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
volatile  uint32_t aADCxData[ADC_CHANNEL_COUNT];

void calculateAndCopyAdcData(uint32_t const start, uint32_t const end) {
  for (unsigned int i = start; i < end / 2; i++) {
    aADCxData[i % ADC_CHANNEL_COUNT] += aADCxConvertedData[i];
    aADCxData[i % ADC_CHANNEL_COUNT] /= 2;
  }
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  calculateAndCopyAdcData(ADC_CONVERTED_DATA_BUFFER_SIZE / 2, ADC_CONVERTED_DATA_BUFFER_SIZE);
}

extern "C" void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc) {
  calculateAndCopyAdcData(0, ADC_CONVERTED_DATA_BUFFER_SIZE / 2);
}

extern "C" void setup() {
  // Calibrate ADC
  if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK) {
    Error_Handler();
  }
  // Start ADC DMA
  if (HAL_ADC_Start_DMA(&hadc, (uint32_t *)aADCxConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK) {
    Error_Handler();
  }
  // PWM Timer setup
  __HAL_TIM_SET_PRESCALER(&htim14, 3); //0 = 5.8 kHz, 1 = 2 kHz, 3 = 1.5 kHz (48 MHz CPU & 0xFFF max)
  HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
}

// System
uint32_t getTick() {
  return HAL_GetTick();
}

void delay(uint32_t const aDelayMs) {
  HAL_Delay(aDelayMs);
}

// Power LED
void turnOnPowerLed() {
  HAL_GPIO_WritePin(POWER_LED_GPIO_Port, POWER_LED_Pin, GPIO_PIN_SET);
}

void turnOffPowerLed() {
  HAL_GPIO_WritePin(POWER_LED_GPIO_Port, POWER_LED_Pin, GPIO_PIN_RESET);
}

void togglePowerLed() {
  HAL_GPIO_TogglePin(POWER_LED_GPIO_Port, POWER_LED_Pin);
}

bool isPowerLedOn() {
  return HAL_GPIO_ReadPin(POWER_LED_GPIO_Port, POWER_LED_Pin) == GPIO_PIN_SET;
}

// Mode LED
void turnOnModeLed() {
  HAL_GPIO_WritePin(MODE_LED_GPIO_Port, MODE_LED_Pin, GPIO_PIN_SET);
}

void turnOffModeLed() {
  HAL_GPIO_WritePin(MODE_LED_GPIO_Port, MODE_LED_Pin, GPIO_PIN_RESET);
}

bool isModeLedOn() {
  return HAL_GPIO_ReadPin(MODE_LED_GPIO_Port, MODE_LED_Pin) == GPIO_PIN_SET;
}

// Buzzer
void turnOnBuzzer() {
  
}

void turnOffBuzzer() {
  
}

bool isBuzzerOn() {
  return HAL_GPIO_ReadPin(BUZZER_GPIO_Port, BUZZER_Pin) == GPIO_PIN_SET;
}

// ADC
uint32_t getPotmeterValue() {
  return aADCxData[cIndexAdcPotmeter];
}

extern uint32_t getBatteryValue() {
  return aADCxData[cIndexAdcBattery];
}

// Motor
void setMotorPwm(uint32_t const aMotorPwm) {
  __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, aMotorPwm);
}