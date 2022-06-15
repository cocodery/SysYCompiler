TOPNAME := compiler
BUILD_DIR := ./build
BINARY := $(BUILD_DIR)/$(TOPNAME)
CMAKE := cmake
MAKE := make
GDB := gdb
LLDB := lldb

$(shell mkdir -p $(BUILD_DIR))

.PHONY: build
build:
	$(CMAKE) -S . -B $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR) --file=Makefile -j2 -s

.PHONY: run
run:
	@cd $(BUILD_DIR); ./$(TOPNAME); cd ..

.PHONY: gdb
gdb:
	@cd $(BUILD_DIR); $(GDB) compiler; cd ..

.PHONY: lldb
lldb:
	@cd $(BUILD_DIR); $(LLDB) compiler; cd ..

.PHONY: clean
clean:
	-rm -rf $(BUILD_DIR) $(TOPNAME)
