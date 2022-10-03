################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lzss/bitio.c \
../Core/Src/lzss/errhand.c \
../Core/Src/lzss/lzss.c 

OBJS += \
./Core/Src/lzss/bitio.o \
./Core/Src/lzss/errhand.o \
./Core/Src/lzss/lzss.o 

C_DEPS += \
./Core/Src/lzss/bitio.d \
./Core/Src/lzss/errhand.d \
./Core/Src/lzss/lzss.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lzss/%.o Core/Src/lzss/%.su: ../Core/Src/lzss/%.c Core/Src/lzss/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F051x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lzss

clean-Core-2f-Src-2f-lzss:
	-$(RM) ./Core/Src/lzss/bitio.d ./Core/Src/lzss/bitio.o ./Core/Src/lzss/bitio.su ./Core/Src/lzss/errhand.d ./Core/Src/lzss/errhand.o ./Core/Src/lzss/errhand.su ./Core/Src/lzss/lzss.d ./Core/Src/lzss/lzss.o ./Core/Src/lzss/lzss.su

.PHONY: clean-Core-2f-Src-2f-lzss

