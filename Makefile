SRCPATH=src/
BUILDPATH=build/

CC=arm-none-eabi-gcc
OPTFLAG=-O0
ARMFLAG=-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAG=${ARMFLAG} -Iinc/ -I. -Tlinker.ld -nostartfiles -Wl,-Map=${BUILDPATH}main.map
CFLAG=-Wall -Wextra ${OPTFLAG} ${ARMFLAG} -Iinc/ -I.

OBJFILES=${SRCPATH}startup.o ${SRCPATH}main.o ${SRCPATH}gpio.o ${SRCPATH}rcc.o 
ELFFILES=${BUILDPATH}main.elf
BINFILES=${BUILDPATH}build.bin

all: ${ELFFILES}

${ELFFILES}: ${OBJFILES}
	${CC} ${LDFLAG} -o $@ $^
	
%.o: %.c
	${CC} ${CFLAG} -c -o $@ $^

build: ${BINFILES}
	st-flash --connect-under-reset --freq=1200k write $< 0x08000000

${BINFILES}: ${ELFFILES}
	arm-none-eabi-objcopy -O binary $< $@

clean:
	rm -rf ${SRCPATH}*.o