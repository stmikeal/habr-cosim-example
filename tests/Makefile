.PHONY: all qemu dir
all: dir qemu

dir:
	mkdir build/
build/test.o: src/test.c
	riscv64-unknown-elf-gcc -c -g -O0 -Iinclude/ -ffreestanding -march=rv32i -mabi=ilp32 -o build/test.o src/test.c
build/start.o: asm/start.s
	riscv64-unknown-elf-as -g -march=rv32i -mabi=ilp32 -o build/start.o asm/start.s
build/hello.elf: build/test.o build/start.o
	riscv64-unknown-elf-ld -T ld/baremetal.ld -m elf32lriscv -o build/hello.elf build/test.o build/start.o
qemu: build/hello.elf
	${XILINX_QEMU}/qemu-system-riscv32 -M virt-cosim -chardev socket,id=cosim,path=/tmp/cosim.sock,server=on -nographic -machine-path /tmp/machine-riscv32 -bios build/hello.elf