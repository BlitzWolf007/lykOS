# Executable name
KERNEL = out/lykos-kernel.elf

# Compilers
CC = x86_64-elf-gcc
NASM = nasm
LD = x86_64-elf-ld

# Compiler flags
CFLAGS = \
	-I./src \
    -Wall \
    -Wextra \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-PIE \
    -fno-PIC \
    -m64 \
    -march=x86-64 \
    -mabi=sysv \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -mcmodel=kernel \
    -g \
    -Wno-pointer-arith 

NASMFLAGS = \
    -Wall \
    -f elf64

LINK_FILE = linker.ld

LDFLAGS =    				\
	-nostdlib				\
    -static 				\
    -m elf_x86_64			\
    -z max-page-size=0x1000 \
    -T $(LINK_FILE)

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source files
ASM_FILES := $(shell find $(SRC_DIR) -name '*.asm')
C_FILES := $(shell find $(SRC_DIR) -name '*.c')
CXX_FILES := $(shell find $(SRC_DIR) -name '*.cpp')

# Object files
ASM_OBJ_FILES := $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(ASM_FILES))
C_OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_FILES))

all: $(KERNEL)

$(SRC_DIR)/limine.h:
	curl -Lo $@ https://github.com/limine-bootloader/limine/raw/trunk/limine.h

$(KERNEL): $(ASM_OBJ_FILES) $(C_OBJ_FILES) $(CXX_OBJ_FILES)
	mkdir -p out
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm $(SRC_DIR)/limine.h
	mkdir -p $(@D)
	$(NASM) $(NASMFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/limine.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) out