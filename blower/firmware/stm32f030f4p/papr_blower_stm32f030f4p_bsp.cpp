#include "main.h"
#include "bspInterface.h"
#include "applicationInterface.h"

extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim14;

namespace {
  constexpr uint32_t cAdcChannelCount = ((uint32_t)  4);
  constexpr uint32_t cAdcSampleCount = ((uint32_t)  64);
  constexpr uint32_t cAdcBufferSize =   ((uint32_t)  cAdcSampleCount * cAdcChannelCount);
  constexpr uint32_t cIndexAdcBattery =  0u;
  constexpr uint32_t cIndexAdcPotmeter = 1u;
  constexpr uint32_t cIndexAdcTemp =     2u;
  constexpr uint32_t cIndexAdcVref =     3u;
  
  volatile bool adcReady = false;
  
  volatile uint16_t adcBuffer[cAdcBufferSize];
  volatile uint32_t adcData[cAdcChannelCount];
}

void calculateAndCopyAdcData(uint32_t const start, uint32_t const end) {
  for (unsigned int i = start; i < end; i++) {
    adcData[i % cAdcChannelCount] += adcBuffer[i];
    adcData[i % cAdcChannelCount] /= 2;
  }
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  calculateAndCopyAdcData(cAdcBufferSize / 2, cAdcBufferSize);
  adcReady = true;
}

extern "C" void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc) {
  calculateAndCopyAdcData(0, cAdcBufferSize / 2);
}

extern "C" void applicationSetup() {
  // Calibrate ADC
  if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK) {
    Error_Handler();
  }
  // Start ADC DMA
  if (HAL_ADC_Start_DMA(&hadc, (uint32_t *)adcBuffer, cAdcBufferSize) != HAL_OK) {
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
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
}

void turnOffBuzzer() {
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
}

bool isBuzzerOn() {
  return HAL_GPIO_ReadPin(BUZZER_GPIO_Port, BUZZER_Pin) == GPIO_PIN_SET;
}

// ADC
bool isAdcReady() {
  return adcReady;
}

uint32_t getPotmeterValue() {
  return adcData[cIndexAdcPotmeter];
}

extern uint32_t getBatteryValue() {
  adcReady = false;
  uint32_t value = adcData[cIndexAdcBattery];
  return value;
}

// Motor
void setMotorPwm(uint32_t const aMotorPwm) {
  __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, aMotorPwm);
}