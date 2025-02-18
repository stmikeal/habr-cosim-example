# Структура проекта
├── .gitignore\
├── Makefile\
├── rtl-bridge - часть, отвечающая за Verilator\
│   ├── connection.cpp - адаптер для протокола Remote Port\
│   ├── sim_main.cpp - основной поток выполнения в Verilator\
│   └── testbench.sv - тестбенч на языке SystemVerilog\
└── tests - часть, отвечающая за QEMU\
    ├── Makefile\
    ├── asm\
    │   └── start.s - инициализация процессора\
    ├── ld\
    │   └── baremetal.ld - линкер скрипт для корректного расположения секций\
    └── src\
        └── test.c - software тест, который вам и предстоит писать\