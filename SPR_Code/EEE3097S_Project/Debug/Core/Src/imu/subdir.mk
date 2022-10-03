################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/imu/lps22hb.c 

OBJS += \
./Core/Src/imu/lps22hb.o 

C_DEPS += \
./Core/Src/imu/lps22hb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/imu/%.o Core/Src/imu/%.su: ../Core/Src/imu/%.c Core/Src/imu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F051x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-imu

clean-Core-2f-Src-2f-imu:
	-$(RM) ./Core/Src/imu/lps22hb.d ./Core/Src/imu/lps22hb.o ./Core/Src/imu/lps22hb.su

.PHONY: clean-Core-2f-Src-2f-imu

