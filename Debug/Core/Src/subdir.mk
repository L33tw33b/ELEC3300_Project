################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adsr.c \
../Core/Src/biquad.c \
../Core/Src/font12.c \
../Core/Src/font16.c \
../Core/Src/font20.c \
../Core/Src/font24.c \
../Core/Src/font8.c \
../Core/Src/main.c \
../Core/Src/midi.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/synth.c \
../Core/Src/synthutil.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/wavetable.c \
../Core/Src/z_displ_ILI9XXX.c \
../Core/Src/z_touch_XPT2046.c \
../Core/Src/z_touch_XPT2046_menu.c 

C_DEPS += \
./Core/Src/adsr.d \
./Core/Src/biquad.d \
./Core/Src/font12.d \
./Core/Src/font16.d \
./Core/Src/font20.d \
./Core/Src/font24.d \
./Core/Src/font8.d \
./Core/Src/main.d \
./Core/Src/midi.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/synth.d \
./Core/Src/synthutil.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/wavetable.d \
./Core/Src/z_displ_ILI9XXX.d \
./Core/Src/z_touch_XPT2046.d \
./Core/Src/z_touch_XPT2046_menu.d 

OBJS += \
./Core/Src/adsr.o \
./Core/Src/biquad.o \
./Core/Src/font12.o \
./Core/Src/font16.o \
./Core/Src/font20.o \
./Core/Src/font24.o \
./Core/Src/font8.o \
./Core/Src/main.o \
./Core/Src/midi.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/synth.o \
./Core/Src/synthutil.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/wavetable.o \
./Core/Src/z_displ_ILI9XXX.o \
./Core/Src/z_touch_XPT2046.o \
./Core/Src/z_touch_XPT2046_menu.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/adsr.d ./Core/Src/adsr.o ./Core/Src/adsr.su ./Core/Src/biquad.d ./Core/Src/biquad.o ./Core/Src/biquad.su ./Core/Src/font12.d ./Core/Src/font12.o ./Core/Src/font12.su ./Core/Src/font16.d ./Core/Src/font16.o ./Core/Src/font16.su ./Core/Src/font20.d ./Core/Src/font20.o ./Core/Src/font20.su ./Core/Src/font24.d ./Core/Src/font24.o ./Core/Src/font24.su ./Core/Src/font8.d ./Core/Src/font8.o ./Core/Src/font8.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/midi.d ./Core/Src/midi.o ./Core/Src/midi.su ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/synth.d ./Core/Src/synth.o ./Core/Src/synth.su ./Core/Src/synthutil.d ./Core/Src/synthutil.o ./Core/Src/synthutil.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/wavetable.d ./Core/Src/wavetable.o ./Core/Src/wavetable.su ./Core/Src/z_displ_ILI9XXX.d ./Core/Src/z_displ_ILI9XXX.o ./Core/Src/z_displ_ILI9XXX.su ./Core/Src/z_touch_XPT2046.d ./Core/Src/z_touch_XPT2046.o ./Core/Src/z_touch_XPT2046.su ./Core/Src/z_touch_XPT2046_menu.d ./Core/Src/z_touch_XPT2046_menu.o ./Core/Src/z_touch_XPT2046_menu.su

.PHONY: clean-Core-2f-Src

