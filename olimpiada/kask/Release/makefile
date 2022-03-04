################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
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
olimpiada1.lss \

FLASH_IMAGE += \
olimpiada1.hex \

EEPROM_IMAGE += \
olimpiada1.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: olimpiada1.elf secondary-outputs

# Tool invocations
olimpiada1.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,olimpiada1.map -mmcu=atmega16 -o "olimpiada1.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

olimpiada1.lss: olimpiada1.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S olimpiada1.elf  >"olimpiada1.lss"
	@echo 'Finished building: $@'
	@echo ' '

olimpiada1.hex: olimpiada1.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex olimpiada1.elf  "olimpiada1.hex"
	@echo 'Finished building: $@'
	@echo ' '

olimpiada1.eep: olimpiada1.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex olimpiada1.elf  "olimpiada1.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: olimpiada1.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega16 olimpiada1.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) olimpiada1.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
