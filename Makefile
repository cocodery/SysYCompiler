TOPNAME := compiler
BUILD_DIR := ./build
BINARY := $(BUILD_DIR)/$(TOPNAME)
CMAKE := cmake
MAKE := make

.PHONY: build
build:
	$(CMAKE) -S . -B $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR) --file=Makefile -j -s

.PHONY: run
run:
	@cd $(BUILD_DIR); ./$(TOPNAME); cd ..

.PHONY: clean
clean:
	-rm -rf $(BUILD_DIR) $(TOPNAME)