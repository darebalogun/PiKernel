# Cross compiler
TOOLCHAIN = aarch64-none-elf
CC = $(TOOLCHAIN)-gcc
GDB = $(TOOLCHAIN)-gdb
LD = $(TOOLCHAIN)-ld
COPY = $(TOOLCHAIN)-objcopy

# Directive
CFLAGS = -ffreestanding -Wall -O2 -fno-stack-protector -nostartfiles
CSRCFLAGS = -Wall -Wextra
LFLAGS = -nostdlib -nostartfiles

# Location of the files
KER_SRC = ../src/kernel
KER_HEAD = ../include
COMMON_SRC = ../src/common
OBJ_DIR = objects
KERSOURCES = $(wildcard $(KER_SRC)/*.c)
COMMONSOURCES = $(wildcard $(COMMON_SRC)/*.c)
ASMSOURCES = $(wildcard $(KER_SRC)/*.S)
OBJECTS = $(patsubst $(KER_SRC)/%.c, $(OBJ_DIR)/%.o, $(KERSOURCES))
OBJECTS += $(patsubst $(COMMON_SRC)/%.c, $(OBJ_DIR)/%.o, $(COMMONSOURCES))
OBJECTS += $(patsubst $(KER_SRC)/%.S, $(OBJ_DIR)/%.o, $(ASMSOURCES))
HEADERS = $(wildcard $(KER_HEAD)/*.h)

IMG_NAME = kernel8.elf

build: $(OBJECTS) $(HEADERS)
	$(CC) -T linker.ld -o $(IMG_NAME) $(LFLAGS) $(OBJECTS) font.o
	$(COPY) $(IMG_NAME) -O binary kernel8.img

$(OBJ_DIR)/%.o: $(KER_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.S
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -c $< -o $@

$(OBJ_DIR)/%.o: $(COMMON_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)


clean:
	rm -rf $(OBJ_DIR)
	rm $(IMG_NAME)

run: build
	qemu-system-aarch64 -M raspi3 -serial stdio -kernel kernel8.img

dbg:
	$(GDB) $(IMG_NAME)

dbgrun: build gdbinit
	qemu-system-aarch64 -M raspi3 -serial stdio -kernel $(IMG_NAME) -S -s

.PHONY: gdbinit
gdbinit:
	echo "target remote localhost:1234" > .gdbinit
	echo "break kernel_main" >> .gdbinit