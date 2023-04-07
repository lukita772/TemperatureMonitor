################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/mlx90621/subdir.mk
-include src/ILI9341/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ProyetoTD1_v1.axf

# Tool invocations
ProyetoTD1_v1.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/media/luca/LukStorage/LukStorage/Electronica/Codigos/repoTD2-TPS_v1/workspace/lpc_chip_175x_6x/Debug" -Xlinker -Map="ProyetoTD1_v1.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "ProyetoTD1_v1_Debug.ld" -o "ProyetoTD1_v1.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) ProyetoTD1_v1.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "ProyetoTD1_v1.axf"; # arm-none-eabi-objcopy -v -O binary "ProyetoTD1_v1.axf" "ProyetoTD1_v1.bin" ; # checksum -p LPC1769 -d "ProyetoTD1_v1.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
