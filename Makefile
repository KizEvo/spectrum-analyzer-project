SRCDIR=src/
BUILDDIR=build/

CC=arm-none-eabi-gcc
OPTFLAG=-O0
ARMFLAG=-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAG=${ARMFLAG} -Iinc/ -I. -Tlinker.ld -nostartfiles -Wl,-Map=${BUILDDIR}main.map
CFLAG=-Wall -Wextra ${OPTFLAG} ${ARMFLAG} -Iinc/ -I.

SRCFILES=${shell find ${SRCDIR} -name "*.c"}
OBJFILES=${patsubst ${SRCDIR}%.c,${BUILDDIR}%.o, ${SRCFILES}}
ELFFILES=${BUILDDIR}main.elf
BINFILES=${BUILDDIR}build.bin

all: ${ELFFILES}

${ELFFILES}: ${OBJFILES}
	${CC} ${LDFLAG} -o $@ $^
	
${OBJFILES}: 
	${CC} ${CFLAG} -c -o $@ ${patsubst ${BUILDDIR}%.o,${SRCDIR}%.c, $@}

build: ${BINFILES}
	st-flash --connect-under-reset --freq=1200k write $< 0x08000000

${BINFILES}: ${ELFFILES}
	arm-none-eabi-objcopy -O binary $< $@

clean:
	rm -rf ${OBJFILES} ${ELFFILES} ${BINFILES} ${BUILDDIR}main.map