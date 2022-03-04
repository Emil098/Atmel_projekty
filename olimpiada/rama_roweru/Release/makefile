################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include ws2812/subdir.mk
-include uart/subdir.mk
-include lsm303/subdir.mk
-include i2c/subdir.mk
-include hc05/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
olimpiada.lss \

FLASH_IMAGE += \
olimpiada.hex \

EEPROM_IMAGE += \
olimpiada.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: olimpiada.elf secondary-outputs

# Tool invocations
olimpiada.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,olimpiada.map -mmcu=atmega328p -o "olimpiada.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

olimpiada.lss: olimpiada.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S olimpiada.elf  >"olimpiada.lss"
	@echo 'Finished building: $@'
	@echo ' '

olimpiada.hex: olimpiada.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex olimpiada.elf  "olimpiada.hex"
	@echo 'Finished building: $@'
	@echo ' '

olimpiada.eep: olimpiada.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex olimpiada.elf  "olimpiada.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: olimpiada.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p olimpiada.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) olimpiada.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
