GFXINC += $(GFXLIB)/drivers/gdisp/WS29EPD
GFXSRC += $(GFXLIB)/drivers/gdisp/WS29EPD/gdisp_lld_WS29EPD.c

ifneq ($(STMHAL),)
GFXSRC += $(STMHAL)/Src/stm32f4xx_hal.c \
	  $(STMHAL)/Src/stm32f4xx_hal_spi.c \
	  $(STMHAL)/Src/stm32f4xx_hal_gpio.c \
	  $(STMHAL)/Src/stm32f4xx_hal_flash.c \
	  $(STMHAL)/Src/stm32f4xx_hal_flash_ex.c \
	  $(STMHAL)/Src/stm32f4xx_hal_rcc.c \
	  $(STMHAL)/Src/stm32f4xx_hal_pwr.c \
	  $(STMHAL)/Src/stm32f4xx_hal_cortex.c \
	  $(STMHAL)/Src/stm32f4xx_hal_dma.c
GFXINC += $(STMHAL)/Inc
endif

ifneq ($(CMSIS),)
GFXSRC += $(CMSIS)/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f401xe.s \
	  $(CMSIS)/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
GFXINC += $(CMSIS)/Include \
	  $(CMSIS)/Device/ST/STM32F4xx/Include
endif
