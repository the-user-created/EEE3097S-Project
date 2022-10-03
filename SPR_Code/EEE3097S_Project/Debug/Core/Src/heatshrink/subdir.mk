################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/heatshrink/heatshrink_decoder.c \
../Core/Src/heatshrink/heatshrink_encoder.c 

OBJS += \
./Core/Src/heatshrink/heatshrink_decoder.o \
./Core/Src/heatshrink/heatshrink_encoder.o 

C_DEPS += \
./Core/Src/heatshrink/heatshrink_decoder.d \
./Core/Src/heatshrink/heatshrink_encoder.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/heatshrink/%.o Core/Src/heatshrink/%.su: ../Core/Src/heatshrink/%.c Core/Src/heatshrink/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F051x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-heatshrink

clean-Core-2f-Src-2f-heatshrink:
	-$(RM) ./Core/Src/heatshrink/heatshrink_decoder.d ./Core/Src/heatshrink/heatshrink_decoder.o ./Core/Src/heatshrink/heatshrink_decoder.su ./Core/Src/heatshrink/heatshrink_encoder.d ./Core/Src/heatshrink/heatshrink_encoder.o ./Core/Src/heatshrink/heatshrink_encoder.su

.PHONY: clean-Core-2f-Src-2f-heatshrink

