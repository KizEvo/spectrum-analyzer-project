SRCPATH=src/
BUILDPATH=build/

CC=arm-none-eabi-gcc
OPTFLAG=-O0
ARMFLAG=-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAG=${ARMFLAG} -Tlinker.ld -nostartfiles -Wl,-Map=${BUILDPATH}main.map
CFLAG=-Wall -Wextra ${OPTFLAG} ${ARMFLAG} -Iinc/

OBJFILES=${SRCPATH}startup.o ${SRCPATH}main.o
ELFFILES=${BUILDPATH}main.elf

all: ${ELFFILES}

${ELFFILES}: ${OBJFILES}
	${CC} ${LDFLAG} -o $@ $^
	
%.o: %.c
	${CC} ${CFLAG} -c -o $@ $^
	
clean:
	rm -rf ${SRCPATH}*.o