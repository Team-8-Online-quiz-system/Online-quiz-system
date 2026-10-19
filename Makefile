CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Werror -Wformat-security

SRCS := $(wildcard src/*.cpp)
LIB_SRCS := $(filter-out src/main.cpp,$(SRCS))
HEADERS := $(wildcard src/*.hpp)
TESTS := $(patsubst tests/%.cpp,build/%,$(wildcard tests/test_*.cpp))

BIN := quiz

.PHONY: all test clean

all: $(BIN)

$(BIN): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

build:
	mkdir -p build

build/test_%: tests/test_%.cpp $(LIB_SRCS) $(HEADERS) | build
	$(CXX) $(CXXFLAGS) -Isrc -o $@ $< $(LIB_SRCS)

test: $(TESTS)
	@for t in $(TESTS); do echo "== $$t"; ./$$t || exit 1; done

clean:
	rm -rf build $(BIN) $(BIN).exe
