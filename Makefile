CXX := g++

CXXFLAGS := -g -DDEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ \
 -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations \
 -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
 -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
 -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 \
 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual \
 -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
 -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 \
 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types \
 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef \
 -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros \
 -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing \
 -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new \
 -fsized-deallocation -fstack-protector -fstrict-overflow \
 -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 \
 -Wstack-usage=8192 -pie -fPIE -Werror=vla


SIMD_FLAGS := -mavx2 -mfma

SANITIZERS := -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CXXFLAGS += $(SIMD_FLAGS)
CXXFLAGS += -march=native -fno-omit-frame-pointer

INCLUDES := -I headers
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj


OTHER_SOURCES := $(SRC_DIR)/analyse.cpp \
                 $(SRC_DIR)/benchmark.cpp \
                 $(SRC_DIR)/errors_ht.cpp \
                 $(SRC_DIR)/hash_functions.cpp \
                 $(SRC_DIR)/hash_table.cpp \
                 $(SRC_DIR)/load_ht.cpp \
                 $(SRC_DIR)/text.cpp


OTHER_OBJS := $(OTHER_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


MAIN_SOURCE := $(SRC_DIR)/main.cpp
MAIN_OBJ := $(OBJ_DIR)/main.o


TARGET_COMPARISON := $(BIN_DIR)/comparison
TARGET_BENCHMARK := $(BIN_DIR)/benchmark


all: $(TARGET_COMPARISON) $(TARGET_BENCHMARK)


comparison: $(TARGET_COMPARISON)

$(TARGET_COMPARISON): $(OTHER_OBJS) $(OBJ_DIR)/main_comparison.o | $(BIN_DIR)
	@echo " Linking comparison..."
	@$(CXX) $^ $(SANITIZERS) -o $@
	@echo " Comparison ready: $@"


benchmark: $(TARGET_BENCHMARK)

$(TARGET_BENCHMARK): $(OTHER_OBJS) $(OBJ_DIR)/main_benchmark.o | $(BIN_DIR)
	@echo " Linking benchmark..."
	@$(CXX) $^ $(SANITIZERS) -o $@
	@echo " Benchmark ready: $@"


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo " Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


$(OBJ_DIR)/main_comparison.o: $(MAIN_SOURCE) | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo " Compiling main.cpp for COMPARISON mode..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -DCOMPARISON -c $< -o $@


$(OBJ_DIR)/main_benchmark.o: $(MAIN_SOURCE) | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo " Compiling main.cpp for BENCHMARK mode..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -DBENCHMARK -c $< -o $@


$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@


check: comparison
	@echo ""
	@echo " Running comparison..."
	@echo "=========================================="
	@cd $(BIN_DIR) && ./comparison

check-benchmark: benchmark
	@echo ""
	@echo " Running benchmark..."
	@echo "=========================================="
	@cd $(BIN_DIR) && ./benchmark


run-comparison: comparison
	@cd $(BIN_DIR) && ./comparison

run-benchmark: benchmark
	@cd $(BIN_DIR) && ./benchmark

# Очистка
clean:
	@echo " Cleaning..."
	@rm -rf $(BUILD_DIR)
	@rm -rf logger/
	@mkdir -p logger/
	@touch logger/hashtable.log
	@echo " Clean complete"


rebuild: clean all


logger-clean:
	@rm -rf logger/
	@mkdir -p logger/
	@touch logger/hashtable.log
	@echo " Logger cleaned"


info:
	@echo "═══════════════════════════════════════════════════════════"
	@echo " Available targets:"
	@echo "   make comparison     - Build comparison tool"
	@echo "   make benchmark      - Build benchmark tool"
	@echo "   make all            - Build both"
	@echo ""
	@echo " Run targets:"
	@echo "   make check           - Run comparison"
	@echo "   make check-benchmark - Run benchmark"
	@echo "   make run-comparison  - Run comparison"
	@echo "   make run-benchmark   - Run benchmark"
	@echo ""
	@echo " Maintenance:"
	@echo "   make clean         - Clean build files"
	@echo "   make rebuild       - Full rebuild"
	@echo "   make logger-clean  - Clean logs only"
	@echo "═══════════════════════════════════════════════════════════"

.PHONY: all clean rebuild check check-benchmark comparison benchmark \
        logger-clean run-comparison run-benchmark info