################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include tim/subdir.mk
-include menu/subdir.mk
-include i2c/subdir.mk
-include eprom/subdir.mk
-include ds1307/subdir.mk
-include disp/subdir.mk
-include buzzer/subdir.mk
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
zegarek.lss \

FLASH_IMAGE += \
zegarek.hex \

EEPROM_IMAGE += \
zegarek.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: zegarek.elf secondary-outputs

# Tool invocations
zegarek.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,zegarek.map -Lzegarek/i2c -mmcu=atmega16 -o "zegarek.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

zegarek.lss: zegarek.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S zegarek.elf  >"zegarek.lss"
	@echo 'Finished building: $@'
	@echo ' '

zegarek.hex: zegarek.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex zegarek.elf  "zegarek.hex"
	@echo 'Finished building: $@'
	@echo ' '

zegarek.eep: zegarek.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex zegarek.elf  "zegarek.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: zegarek.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega16 zegarek.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) zegarek.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
