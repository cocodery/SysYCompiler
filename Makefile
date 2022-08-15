TOPNAME := compiler 
BUILD_DIR := ./build
BINARY := $(BUILD_DIR)/$(TOPNAME)
CMAKE := cmake
MAKE := make
GDB := gdb
LLDB := lldb
OS := $(shell uname)
ifeq ($(OS), Darwin)
	LLVM_LINK := /usr/local/opt/llvm/bin/llvm-link
	LLI := /usr/local/opt/llvm/bin/lli
	DIFF := /usr/local/Cellar/diffutils/3.8/bin/diff
else
	LLVM_LINK := llvm-link
	LLI := lli
	DIFF := diff
endif
MODE := func # func or perf, selet functional or performance
TEST_MODE := functional # test case directory

# re-define 
#ifeq ($(MODE), "func")
#	TEST_MODE = functional 
#else
#	TEST_MODE = performance
#endif

TEST_DIR := ./compiler2022/公开样例与运行时库/$(TEST_MODE)
TEST_CASES = $(shell find $(TEST_DIR) -name "*.sy")
TEST_NUM := $(words $(TEST_CASES));
TEST := 00

OUTPUT_ASM = $(addsuffix .s, $(basename $(TEST_CASES)))
OUTPUT_RES = $(addsuffix .res, $(basename $(TEST_CASES)))
OUTPUT_LOG = $(addsuffix .log, $(basename $(TEST_CASES)))
OUTPUT_IR = $(addsuffix .ll, $(basename $(TEST_CASES)))

CASE = $(shell find $(TEST_DIR) -name "$(TEST)*.sy")

$(shell mkdir -p $(BUILD_DIR))

.PHONY: build
build:
	@$(CMAKE) -S . -B $(BUILD_DIR)
	@$(MAKE) -C $(BUILD_DIR) --file=Makefile -j8 -s

.PHONY: run
run:
	@cd $(BUILD_DIR); ./$(TOPNAME) -S -o ../main.s -l ../main.ll ../main.sy ; cd ..
	@$(LLVM_LINK) sylib.ll main.ll -S -o run.ll

.PHONY: test
test:
	@cd $(BUILD_DIR); ./$(TOPNAME) -S -o ../main.s -l ../main.ll ../$(CASE); cd ..
	@$(LLVM_LINK) sylib.ll main.ll -S -o run.ll

.ONESHELL:
.PHONY: all
all:
	@success=0
	@for file in $(sort $(TEST_CASES))
	do
		ASM=$${file%.*}.s
		LOG=$${file%.*}.log
		RES=$${file%.*}.res
		LL=$${file%.*}.ll
		IN=$${file%.*}.in
		OUT=$${file%.*}.out
		FILE=$${file##*/}
		FILE=$${FILE%.*}
		timeout 500s ./$(BUILD_DIR)/$(TOPNAME) -S -o $${ASM} -l $${LL} $${file}  >> $${LOG}
		RETURN_VALUE=$$? 
		if [ $$RETURN_VALUE = 124 ]; then
			echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mCompile Timeout\033[0m"
			continue
		else if [ $$RETURN_VALUE != 0 ]; then
			echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mCompile Error\033[0m"
			continue
			fi
		fi
		$(LLVM_LINK) sylib.ll $${LL} -S -o $${LL} >> $${LOG} 2>&1
		if [ $$? != 0 ]; then
			echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mAssemble Error\033[0m"
		else
			if [ -f "$${IN}" ]; then
				timeout 10s $(LLI) $${LL} <$${IN} 2>>$${LOG} >$${RES}
			else
				timeout 10s $(LLI) $${LL} 2>>$${LOG} >$${RES}
			fi
			RETURN_VALUE=$$?
			FINAL=`tail -c 1 $${RES}`
			[ $${FINAL} ] && echo "\n$${RETURN_VALUE}" >> $${RES} || echo "$${RETURN_VALUE}" >> $${RES}

			$(DIFF) -Z $${RES} $${OUT} >/dev/null 2>&1
			if [ $$? != 0 ]; then
				echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mWrong Answer\033[0m"
			else
				success=$$((success + 1))
				echo "\033[1;32mPASS:\033[0m $${FILE}"
			fi
		fi
	done

.ONESHELL:
.PHONY: asm
asm:
	@success=0
	@for file in $(sort $(TEST_CASES))
	do
		ASM=$${file%.*}.s
		LOG=$${file%.*}.log
		RES=$${file%.*}.res
		IN=$${file%.*}.in
		OUT=$${file%.*}.out
		FILE=$${file##*/}
		FILE=$${FILE%.*}
		timeout 500s ./$(BUILD_DIR)/$(TOPNAME) -S -o $${ASM} $${file} >> $${LOG}
		RETURN_VALUE=$$? 
		if [ $$RETURN_VALUE = 124 ]; then
			echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mCompile Timeout\033[0m"
			continue
		else if [ $$RETURN_VALUE != 0 ]; then
			echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mCompile Error\033[0m"
			continue
			fi
		fi
		gcc -march=armv7-a $${ASM} sylib.o -o exec -static >> $${LOG} 2>&1
		if [ $$? != 0 ]; then
			echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mLink Error\033[0m"
		else
			if [ -f "$${IN}" ]; then
				timeout 10s ./exec <$${IN} 2>>$${LOG} >$${RES}
			else
				timeout 10s ./exec 2>>$${LOG} >$${RES}
			fi
			RETURN_VALUE=$$?
			FINAL=`tail -c 1 $${RES}`
			[ $${FINAL} ] && echo "\n$${RETURN_VALUE}" >> $${RES} || echo "$${RETURN_VALUE}" >> $${RES}

			diff -Z $${RES} $${OUT} >/dev/null 2>&1
			if [ $$? != 0 ]; then
				echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mWrong Answer\033[0m"
			else
				success=$$((success + 1))
				echo "\033[1;32mPASS:\033[0m $${FILE}"
			fi
		fi
	done

.PHONY: gdb
gdb:
	@cd $(BUILD_DIR); $(GDB) compiler; cd ..

.PHONY: lldb
lldb:
	@cd $(BUILD_DIR); $(LLDB) compiler; cd ..

.PHONY: clean
clean:
	-rm -rf $(BUILD_DIR) $(TOPNAME)

.PHONY: clean-test
clean-test:
	-@rm -rf $(OUTPUT_ASM) $(OUTPUT_LOG) $(OUTPUT_RES) $(OUTPUT_IR)  
