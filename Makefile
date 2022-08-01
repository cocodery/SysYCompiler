TOPNAME := compiler 
BUILD_DIR := ./build
BINARY := $(BUILD_DIR)/$(TOPNAME)
CMAKE := cmake
MAKE := make
GDB := gdb
LLDB := lldb
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
	@cd $(BUILD_DIR); ./$(TOPNAME) -S -o ../main.asm -l ../main.ll ../main.sy ; cd ..
	@llvm-link sylib.ll main.ll -S -o run.ll

.PHONY: test
test:
	@cd $(BUILD_DIR); ./$(TOPNAME) -S -o ../main.asm -l ../main.ll ../$(CASE); cd ..
	@llvm-link sylib.ll main.ll -S -o run.ll

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
		llvm-link sylib.ll $${LL} -S -o $${LL} >> $${LOG} 2>&1
		if [ $$? != 0 ]; then
			echo "\033[1;31mFAIL:\033[0m $${FILE}\t\033[1;31mAssemble Error\033[0m"
		else
			if [ -f "$${IN}" ]; then
				timeout 10s lli $${LL} <$${IN} 2>>$${LOG} >$${RES}
			else
				timeout 10s lli $${LL} 2>>$${LOG} >$${RES}
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

.PHONY: submit
submit:
		clang++ -std=c++17 -O2 -lm -L/extlibs -I/extlibs -lantlr4-runtime \
		./src/common.cpp \
		./src/main.cpp \
		./src/optimizer/pass_manager.cpp \
		./src/optimizer/lvn.cpp \
		./src/optimizer/mem2reg.cpp \
		./src/frontend/ASTVisitor.cpp \
		./src/frontend/SysYLexer.cpp \
		./src/frontend/SysYVisitor.cpp \
		./src/frontend/SysYBaseVisitor.cpp \
		./src/frontend/SysYParser.cpp \
		./src/structure/llir.cpp \
		./src/structure/symtable.cpp \
		./src/structure/ir.cpp \
		./src/structure/value.cpp \
		./third_party/antlr4-runtime/TokenStream.cpp \
		./third_party/antlr4-runtime/RuntimeMetaData.cpp \
		./third_party/antlr4-runtime/Exceptions.cpp \
		./third_party/antlr4-runtime/BufferedTokenStream.cpp \
		./third_party/antlr4-runtime/BaseErrorListener.cpp \
		./third_party/antlr4-runtime/Token.cpp \
		./third_party/antlr4-runtime/DefaultErrorStrategy.cpp \
		./third_party/antlr4-runtime/InputMismatchException.cpp \
		./third_party/antlr4-runtime/DiagnosticErrorListener.cpp \
		./third_party/antlr4-runtime/CharStream.cpp \
		./third_party/antlr4-runtime/NoViableAltException.cpp \
		./third_party/antlr4-runtime/RecognitionException.cpp \
		./third_party/antlr4-runtime/ParserInterpreter.cpp \
		./third_party/antlr4-runtime/Recognizer.cpp \
		./third_party/antlr4-runtime/Lexer.cpp \
		./third_party/antlr4-runtime/FailedPredicateException.cpp \
		./third_party/antlr4-runtime/ListTokenSource.cpp \
		./third_party/antlr4-runtime/ANTLRErrorStrategy.cpp \
		./third_party/antlr4-runtime/BailErrorStrategy.cpp \
		./third_party/antlr4-runtime/UnbufferedCharStream.cpp \
		./third_party/antlr4-runtime/Vocabulary.cpp \
		./third_party/antlr4-runtime/ANTLRErrorListener.cpp \
		./third_party/antlr4-runtime/TokenSource.cpp \
		./third_party/antlr4-runtime/ParserRuleContext.cpp \
		./third_party/antlr4-runtime/CommonTokenFactory.cpp \
		./third_party/antlr4-runtime/RuleContextWithAltNum.cpp \
		./third_party/antlr4-runtime/InterpreterRuleContext.cpp \
		./third_party/antlr4-runtime/IntStream.cpp \
		./third_party/antlr4-runtime/CommonTokenStream.cpp \
		./third_party/antlr4-runtime/ANTLRInputStream.cpp \
		./third_party/antlr4-runtime/ConsoleErrorListener.cpp \
		./third_party/antlr4-runtime/WritableToken.cpp \
		./third_party/antlr4-runtime/RuleContext.cpp \
		./third_party/antlr4-runtime/UnbufferedTokenStream.cpp \
		./third_party/antlr4-runtime/LexerInterpreter.cpp \
		./third_party/antlr4-runtime/TokenStreamRewriter.cpp \
		./third_party/antlr4-runtime/LexerNoViableAltException.cpp \
		./third_party/antlr4-runtime/ANTLRFileStream.cpp \
		./third_party/antlr4-runtime/CommonToken.cpp \
		./third_party/antlr4-runtime/ProxyErrorListener.cpp \
		./third_party/antlr4-runtime/Parser.cpp \
		./src/codegen/armv8/liveness_info.cpp \
		./third_party/antlr4-runtime/atn/PredicateEvalInfo.cpp \
		./third_party/antlr4-runtime/atn/RuleTransition.cpp \
		./third_party/antlr4-runtime/atn/PredicateTransition.cpp \
		./third_party/antlr4-runtime/atn/ContextSensitivityInfo.cpp \
		./third_party/antlr4-runtime/atn/LexerTypeAction.cpp \
		./third_party/antlr4-runtime/atn/LexerCustomAction.cpp \
		./third_party/antlr4-runtime/atn/ActionTransition.cpp \
		./third_party/antlr4-runtime/atn/WildcardTransition.cpp \
		./third_party/antlr4-runtime/atn/StarLoopbackState.cpp \
		./third_party/antlr4-runtime/atn/ProfilingATNSimulator.cpp \
		./third_party/antlr4-runtime/atn/LexerATNConfig.cpp \
		./third_party/antlr4-runtime/atn/ATNDeserializer.cpp \
		./third_party/antlr4-runtime/atn/ATNConfig.cpp \
		./third_party/antlr4-runtime/atn/LexerActionExecutor.cpp \
		./third_party/antlr4-runtime/atn/OrderedATNConfigSet.cpp \
		./third_party/antlr4-runtime/atn/LexerIndexedCustomAction.cpp \
		./third_party/antlr4-runtime/atn/Transition.cpp \
		./third_party/antlr4-runtime/atn/RuleStartState.cpp \
		./third_party/antlr4-runtime/atn/BlockStartState.cpp \
		./third_party/antlr4-runtime/atn/LexerPushModeAction.cpp \
		./third_party/antlr4-runtime/atn/TokensStartState.cpp \
		./third_party/antlr4-runtime/atn/LexerChannelAction.cpp \
		./third_party/antlr4-runtime/atn/ErrorInfo.cpp \
		./third_party/antlr4-runtime/atn/DecisionInfo.cpp \
		./third_party/antlr4-runtime/atn/LexerAction.cpp \
		./third_party/antlr4-runtime/atn/DecisionEventInfo.cpp \
		./third_party/antlr4-runtime/atn/EpsilonTransition.cpp \
		./third_party/antlr4-runtime/atn/LoopEndState.cpp \
		./third_party/antlr4-runtime/atn/NotSetTransition.cpp \
		./third_party/antlr4-runtime/atn/EmptyPredictionContext.cpp \
		./third_party/antlr4-runtime/atn/ParseInfo.cpp \
		./third_party/antlr4-runtime/atn/LL1Analyzer.cpp \
		./third_party/antlr4-runtime/atn/SetTransition.cpp \
		./third_party/antlr4-runtime/atn/LookaheadEventInfo.cpp \
		./third_party/antlr4-runtime/atn/BasicBlockStartState.cpp \
		./third_party/antlr4-runtime/atn/PlusBlockStartState.cpp \
		./third_party/antlr4-runtime/atn/ATNState.cpp \
		./third_party/antlr4-runtime/atn/PrecedencePredicateTransition.cpp \
		./third_party/antlr4-runtime/atn/SemanticContext.cpp \
		./third_party/antlr4-runtime/atn/BlockEndState.cpp \
		./third_party/antlr4-runtime/atn/AmbiguityInfo.cpp \
		./third_party/antlr4-runtime/atn/AbstractPredicateTransition.cpp \
		./third_party/antlr4-runtime/atn/PredictionMode.cpp \
		./third_party/antlr4-runtime/atn/ArrayPredictionContext.cpp \
		./third_party/antlr4-runtime/atn/ParserATNSimulator.cpp \
		./third_party/antlr4-runtime/atn/ATNSerializer.cpp \
		./third_party/antlr4-runtime/atn/BasicState.cpp \
		./third_party/antlr4-runtime/atn/RangeTransition.cpp \
		./third_party/antlr4-runtime/atn/ATN.cpp \
		./third_party/antlr4-runtime/atn/StarBlockStartState.cpp \
		./third_party/antlr4-runtime/atn/PlusLoopbackState.cpp \
		./third_party/antlr4-runtime/atn/StarLoopEntryState.cpp \
		./third_party/antlr4-runtime/atn/LexerModeAction.cpp \
		./third_party/antlr4-runtime/atn/ATNSimulator.cpp \
		./third_party/antlr4-runtime/atn/LexerATNSimulator.cpp \
		./third_party/antlr4-runtime/atn/AtomTransition.cpp \
		./third_party/antlr4-runtime/atn/SingletonPredictionContext.cpp \
		./third_party/antlr4-runtime/atn/ATNConfigSet.cpp \
		./third_party/antlr4-runtime/atn/LexerSkipAction.cpp \
		./third_party/antlr4-runtime/atn/LexerMoreAction.cpp \
		./third_party/antlr4-runtime/atn/ATNDeserializationOptions.cpp \
		./third_party/antlr4-runtime/atn/LexerPopModeAction.cpp \
		./third_party/antlr4-runtime/atn/PredictionContext.cpp \
		./third_party/antlr4-runtime/atn/RuleStopState.cpp \
		./third_party/antlr4-runtime/atn/DecisionState.cpp \
		./third_party/antlr4-runtime/misc/InterpreterDataReader.cpp \
		./third_party/antlr4-runtime/misc/Predicate.cpp \
		./third_party/antlr4-runtime/misc/Interval.cpp \
		./third_party/antlr4-runtime/misc/MurmurHash.cpp \
		./third_party/antlr4-runtime/misc/IntervalSet.cpp \
		./third_party/antlr4-runtime/dfa/LexerDFASerializer.cpp \
		./third_party/antlr4-runtime/dfa/DFA.cpp \
		./third_party/antlr4-runtime/dfa/DFASerializer.cpp \
		./third_party/antlr4-runtime/dfa/DFAState.cpp \
		./third_party/antlr4-runtime/tree/ParseTreeVisitor.cpp \
		./third_party/antlr4-runtime/tree/IterativeParseTreeWalker.cpp \
		./third_party/antlr4-runtime/tree/ErrorNode.cpp \
		./third_party/antlr4-runtime/tree/ParseTree.cpp \
		./third_party/antlr4-runtime/tree/ParseTreeWalker.cpp \
		./third_party/antlr4-runtime/tree/TerminalNode.cpp \
		./third_party/antlr4-runtime/tree/ErrorNodeImpl.cpp \
		./third_party/antlr4-runtime/tree/ParseTreeListener.cpp \
		./third_party/antlr4-runtime/tree/Trees.cpp \
		./third_party/antlr4-runtime/tree/TerminalNodeImpl.cpp \
		./third_party/antlr4-runtime/support/guid.cpp \
		./third_party/antlr4-runtime/support/StringUtils.cpp \
		./third_party/antlr4-runtime/support/CPPUtils.cpp \
		./third_party/antlr4-runtime/support/Any.cpp \
		./third_party/antlr4-runtime/support/Arrays.cpp \
		./third_party/antlr4-runtime/tree/pattern/ParseTreeMatch.cpp \
		./third_party/antlr4-runtime/tree/pattern/TagChunk.cpp \
		./third_party/antlr4-runtime/tree/pattern/ParseTreePattern.cpp \
		./third_party/antlr4-runtime/tree/pattern/Chunk.cpp \
		./third_party/antlr4-runtime/tree/pattern/ParseTreePatternMatcher.cpp \
		./third_party/antlr4-runtime/tree/pattern/RuleTagToken.cpp \
		./third_party/antlr4-runtime/tree/pattern/TokenTagToken.cpp \
		./third_party/antlr4-runtime/tree/pattern/TextChunk.cpp \
		./third_party/antlr4-runtime/tree/xpath/XPathLexer.cpp \
		./third_party/antlr4-runtime/tree/xpath/XPathLexerErrorListener.cpp \
		./third_party/antlr4-runtime/tree/xpath/XPathWildcardElement.cpp \
		./third_party/antlr4-runtime/tree/xpath/XPathRuleElement.cpp \
		./third_party/antlr4-runtime/tree/xpath/XPathTokenAnywhereElement.cpp \
		./third_party/antlr4-runtime/tree/xpath/XPathTokenElement.cpp \
		./third_party/antlr4-runtime/tree/xpath/XPathRuleAnywhereElement.cpp \
		./third_party/antlr4-runtime/tree/xpath/XPathElement.cpp \
		./third_party/antlr4-runtime/tree/xpath/XPathWildcardAnywhereElement.cpp \
		./third_party/antlr4-runtime/tree/xpath/XPath.cpp -I \
		./src -I \
		./src/optimizer -I \
		./src/frontend -I \
		./src/structure -I \
		./third_party/antlr4-runtime -I \
		./src/codegen/armv8 -I \
		./third_party/antlr4-runtime/atn -I \
		./third_party/antlr4-runtime/misc -I \
		./third_party/antlr4-runtime/dfa -I \
		./third_party/antlr4-runtime/tree -I \
		./third_party/antlr4-runtime/support -I \
		./third_party/antlr4-runtime/tree/pattern -I \
		./third_party/antlr4-runtime/tree/xpath -o \
		./compiler
