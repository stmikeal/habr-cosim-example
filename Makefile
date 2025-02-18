.PHONY: all clean sim

all: sim

TARGET = main
TOP_MODULE = testbench

BUILD_DIR = build
SIM_DIR = $(BUILD_DIR)/simulation
SRC_DIR = rtl-bridge

SRCS := $(shell find ./$(SRC_DIR) -name '*.cpp')

VERILATOR_FLAGS = --trace --exe --cc --build --timing -Wno-PINMISSING 					\
				  -Wno-IMPLICIT -Wno-WIDTHEXPAND -Wno-INITIALDLY 						\
				  -Wno-CASEINCOMPLETE -Wno-WIDTHTRUNC --Mdir $(SIM_DIR)	\
				  --coverage 

TARGET = main

$(TARGET): $(SRCS) $(SRC_DIR)/$(TOP_MODULE).sv
	mkdir -p $(SIM_DIR)
	verilator $(VERILATOR_FLAGS) -o V$(TOP_MODULE) $(SRC_DIR)/$(TOP_MODULE).sv $(SRCS)
	make -C $(SIM_DIR) -f V$(TOP_MODULE).mk
	ln -sf $(SIM_DIR)/V$(TOP_MODULE) $(TARGET)

sim: $(TARGET) Makefile
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)