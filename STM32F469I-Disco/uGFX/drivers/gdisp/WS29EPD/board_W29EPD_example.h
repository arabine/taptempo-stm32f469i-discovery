/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#ifndef GDISP_LLD_BOARD_H
#define GDISP_LLD_BOARD_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_rcc_ex.h"

#include "WS29EPD.h"

/* Global variables. */
SPI_HandleTypeDef spiHandle;

/* Defined functions. */
#define DC_H() {  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);  }
#define DC_L() {  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);  }

#define CS_H() {  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);  }
#define CS_L() {  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);  }

static GFXINLINE void init_board(GDisplay *g) {
	(void) g;
	/* Enable peripheral clocks. */
	__HAL_RCC_SPI3_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

  /* Initialize the spiHandle structure. */
  spiHandle.Instance = SPI3;
  spiHandle.Init.Mode = SPI_MODE_MASTER;
  spiHandle.Init.Direction = SPI_DIRECTION_2LINES;
  spiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
  spiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
  spiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
  spiHandle.Init.NSS = SPI_NSS_SOFT;
  spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  spiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  spiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
  spiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  spiHandle.Init.CRCPolynomial = 0;
  
  HAL_SPI_Init(&spiHandle);
  
  /* Initialize the pins. */
  GPIO_InitTypeDef gpioInit;
  /* C10 SCK */
  gpioInit.Pin = GPIO_PIN_10;
  gpioInit.Mode = GPIO_MODE_AF_PP;
  gpioInit.Pull = GPIO_NOPULL;
  gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
  gpioInit.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &gpioInit);
  
  /* C12 MOSI */
  gpioInit.Pin = GPIO_PIN_12;
  HAL_GPIO_Init(GPIOC, &gpioInit);
  
  /* D2 CS (chip select) */
  gpioInit.Pin = GPIO_PIN_2;
  gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOD, &gpioInit);
  
  /* B7 Reset */
  gpioInit.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOB, &gpioInit);
  
  /* C2 D/C */
  gpioInit.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOC, &gpioInit);
  
  /* C3 Busy */
  gpioInit.Pin = GPIO_PIN_3;
  gpioInit.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(GPIOC, &gpioInit);
  /* Put chip select high to unselect the device. */
  CS_H();  
}

static GFXINLINE void post_init_board(GDisplay *g) {
	(void) g;
}

static GFXINLINE void setpin_reset(GDisplay *g, bool_t state) {
	(void) g;
  /* Set reset pin low/high. */
  if(state)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}

static GFXINLINE void acquire_bus(GDisplay *g) {
	(void) g;
  CS_L();   // Lower chip select pin
}

static GFXINLINE void release_bus(GDisplay *g) {
	(void) g;
  CS_H();   // Unselect the device again
}

static GFXINLINE void write_data(GDisplay *g, uint8_t data) {
	(void) g;

  /* Wait for the Busy pin to go low. */
  while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) == GPIO_PIN_SET)
    gfxSleepMilliseconds(20);
  
  DC_H();   // Data is sent with the data pin high
  HAL_SPI_Transmit(&spiHandle, &data, 1, 1000);
}

static GFXINLINE void write_reg(GDisplay *g, uint8_t reg, uint8_t data){
  (void) g;
  /* Wait for the Busy pin to go low. */
  while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) == GPIO_PIN_SET)
    gfxSleepMilliseconds(20);
  
  
  DC_L();   // Register address is sent with data pin low
  HAL_SPI_Transmit(&spiHandle, &reg, 1, 1000);
  DC_H();   // Data is sent with data pin high
  HAL_SPI_Transmit(&spiHandle, &data, 1, 1000);
  
}

static GFXINLINE void write_cmd(GDisplay *g, uint8_t reg){
  (void) g;
  /* Wait for the Busy pin to go high. */
  while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) == GPIO_PIN_SET)
    gfxSleepMilliseconds(20);

  DC_L();   // Command is sent with data pin low
  HAL_SPI_Transmit(&spiHandle, &reg, 1, 1000);
}

static GFXINLINE void write_reg_data(GDisplay *g, uint8_t reg, uint8_t *data, uint8_t len) {
  (void) g;
  
  /* Wait for the Busy pin to go low. */
  while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) == GPIO_PIN_SET)
    gfxSleepMilliseconds(20);
  
  DC_L();   // Register address is sent with the data pin low
  HAL_SPI_Transmit(&spiHandle, &reg, 1, 1000);
  DC_H();   // Data is sent with the data pin high
  HAL_SPI_Transmit(&spiHandle, data, len, 1000);
}

#endif /* GDISP_LLD_BOARD_H */
