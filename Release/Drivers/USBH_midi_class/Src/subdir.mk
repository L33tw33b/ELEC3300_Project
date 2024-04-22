################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/USBH_midi_class/Src/usbh_MIDI.c 

C_DEPS += \
./Drivers/USBH_midi_class/Src/usbh_MIDI.d 

OBJS += \
./Drivers/USBH_midi_class/Src/usbh_MIDI.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/USBH_midi_class/Src/%.o Drivers/USBH_midi_class/Src/%.su: ../Drivers/USBH_midi_class/Src/%.c Drivers/USBH_midi_class/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc -Os -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-USBH_midi_class-2f-Src

clean-Drivers-2f-USBH_midi_class-2f-Src:
	-$(RM) ./Drivers/USBH_midi_class/Src/usbh_MIDI.d ./Drivers/USBH_midi_class/Src/usbh_MIDI.o ./Drivers/USBH_midi_class/Src/usbh_MIDI.su

.PHONY: clean-Drivers-2f-USBH_midi_class-2f-Src

