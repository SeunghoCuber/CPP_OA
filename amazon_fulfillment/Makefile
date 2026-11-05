CXX      ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -g -O0
INCLUDES := -Iinclude -Itests

BUILD    := build
SRC      := $(wildcard src/*.cpp)
TEST_SRC := $(wildcard tests/*.cpp)
OBJ      := $(SRC:%.cpp=$(BUILD)/%.o) $(TEST_SRC:%.cpp=$(BUILD)/%.o)
BIN      := $(BUILD)/run_tests

.PHONY: all test clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# make test               -> run all tests
# make test FILTER=T05    -> run only tests whose name contains "T05"
test: $(BIN)
	./$(BIN) $(FILTER)

clean:
	rm -rf $(BUILD)

-include $(OBJ:.o=.d)
