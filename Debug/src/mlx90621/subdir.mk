################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/mlx90621/mlx90621_driver.c 

OBJS += \
./src/mlx90621/mlx90621_driver.o 

C_DEPS += \
./src/mlx90621/mlx90621_driver.d 


# Each subdirectory must supply rules for building sources it contributes
src/mlx90621/%.o: ../src/mlx90621/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DNO_BOARD_LIB -D__LPC17XX__ -D__REDLIB__ -I"/media/luca/LukStorage/LukStorage/Electronica/Codigos/repoTD2-TPS_v1/workspace/lpc_chip_175x_6x/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


