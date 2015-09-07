LIBROOT=/home/frank/STM32Cube_FW_F4_V1.7.0
DEVICE=$(LIBROOT)/Drivers/CMSIS/Device/ST/STM32F4xx
CORE=$(LIBROOT)/Drivers/CMSIS
HAL =$(LIBROOT)/Drivers/STM32F4xx_HAL_Driver

# Sources
ASM_SOURCES = $(wildcard *.s)
C_SOURCES   = $(wildcard *.c)

# Add modules to be compiled
HAL_SRC = $(HAL)/Src
C_HAL_MODS =  $(HAL_SRC)/stm32f4xx_hal.c $(HAL_SRC)/stm32f4xx_hal_cortex.c
C_HAL_MODS += $(HAL_SRC)/stm32f4xx_hal_rcc.c $(HAL_SRC)/stm32f4xx_hal_gpio.c

# Targets
OBJS = $(ASM_SOURCES:.s=.o) $(C_SOURCES:.c=.o) $(C_HAL_MODS:.c=.o)
ELF=$(notdir $(CURDIR)).elf                    
HEX=$(notdir $(CURDIR)).hex                    

# Tools
GCC= arm-none-eabi-gcc
#CC = $(GCC) -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DSTM32F401xE
CC = $(GCC) -mthumb -mcpu=cortex-m4 -mfloat-abi=soft -DSTM32F401xE -Os -flto
LD = $(CC) -TSTM32_f401re.ld
CP = arm-none-eabi-objcopy
SZ = arm-none-eabi-size
HX = $(CP) -O ihex

# Libraries
LIBS = -lc -lnosys

# Includes
INC = -I$(DEVICE)/Include -I$(CORE)/Include -I$(HAL)/Inc -DUSE_HAL_DRIVER -I./
                              
# Build executable
%.o: %.c
	$(CC) -g -c $(INC) $< -o $@

%.o: %.s
	$(CC) -g -c $< -o $@

$(HEX): $(ELF)
	$(HX) $< $@

$(ELF) : $(OBJS)
	$(LD) $(LIBS) $(OBJS) -o $@
	$(SZ) $@

clean:
	rm -f $(OBJS) $(ELF) $(HEX)

print-%:
	@echo $* = $($*)

