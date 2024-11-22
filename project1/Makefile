# Directories
CURDIR := $(shell pwd)
OUTDIR = $(CURDIR)/out
REPORTDIR = $(CURDIR)/report
TESTDIR = $(CURDIR)/tests

# Source Files
SRC = $(wildcard $(CURDIR)/*.cpp)
TEST_INPUTS = $(wildcard $(TESTDIR)/*.in)
TEST_OUTPUTS = $(wildcard $(TESTDIR)/*.out)
TOTAL_ERRORS = 

# Targets
FILENAME = proj
TARGET = $(OUTDIR)/$(FILENAME).out
DEBUGTARGET = $(OUTDIR)/$(FILENAME).debug
INPUT_FILE = $(CURDIR)/$(FILENAME).in
TEST_OUT_EXT = .outme

# Compiler and flags
CC = g++
CCLAGS = -std=c++11 -O3 -Wall
VALFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$(REPORTDIR)/valgrind.log
GPROFFLAGS = gmon.out -b -p -q -A > $(REPORTDIR)/gprof.log
LIZFLAGS = -l cpp -Eduplicate -a 5 -C 10 -L 1000 -a 5 -E NS -t 4 -o $(REPORTDIR)/lizard.html

# Default rule
all: $(TARGET) valgrind gprof lizard

# interactive rule
inter: $(TARGET) inter-valgrind inter-gprof lizard

# Test rule
test: $(TESTDIR) $(TEST_INPUTS) $(TEST_OUTPUTS) $(TARGET)
	@TOTAL_ERRORS=""; \
	for file in $(TEST_INPUTS); do \
		echo "Running test $$file"; \
		$(TARGET) < $$file > $${file%.in}.outme; \
		if ! diff -q $${file%.in}.outme $${file%.in}.out; then \
			TOTAL_ERRORS="$$TOTAL_ERRORS $$file"; \
		fi; \
	done; \
	if [ -n "$$TOTAL_ERRORS" ]; then \
		echo "Tests failed for the following files: $$TOTAL_ERRORS"; \
	else \
		echo "All tests passed"; \
	fi

# Clean rule
clean:
	rm -rf $(OUTDIR)
	rm -rf $(REPORTDIR)


# Ensure the tests directory exists
$(TESTDIR):
	@if [ ! -d "$(TESTDIR)" ]; then \
		echo "The tests directory does not exist"; \
		exit 1; \
	fi

# Ensure the input and output files exist
$(TEST_INPUTS):
	@if [ -z "$(TEST_INPUTS)" ]; then \
		echo "No .in files found in $(TESTDIR)"; \
		exit 1; \
	fi

$(TEST_OUTPUTS):
	@if [ -z "$(TEST_OUTPUTS)" ]; then \
		echo "No .out files found in $(TESTDIR)"; \
		exit 1; \
	fi

# Ensure the output directory exists
$(OUTDIR):
	mkdir -p $(OUTDIR)

# Ensure the report directory exists
$(REPORTDIR):
	mkdir -p $(REPORTDIR)

# Compile and run checkers
$(TARGET): $(OUTDIR) $(SRC)
	$(CC) $(CCLAGS) -o $(TARGET) $(SRC)

# Compile and run debug
$(DEBUGTARGET): $(OUTDIR)
	$(CC) $(CCLAGS) -pg -o $(DEBUGTARGET) $(SRC)

# Run valgrind
valgrind: $(TARGET) $(REPORTDIR)
	valgrind $(VALFLAGS) $(TARGET) < $(INPUT_FILE)

# Run valgrind Interactive
inter-valgrind: $(TARGET) $(REPORTDIR)
	valgrind $(VALFLAGS) $(TARGET)

# Run gprof
gprof: $(DEBUGTARGET) $(REPORTDIR)
	$(DEBUGTARGET) < $(INPUT_FILE)
	gprof $(DEBUGTARGET) $(GPROFFLAGS)
	rm gmon.out

# Run gprof Interactive
inter-gprof: $(DEBUGTARGET) $(REPORTDIR)
	$(DEBUGTARGET)
	gprof $(DEBUGTARGET) $(GPROFFLAGS)
	rm gmon.out

# Run lizard
lizard: $(REPORTDIR)
	lizard $(LIZFLAGS) $(SRC)

# Phony rules
.PHONY: all inter test clean valgrind inter-valgrind gprof inter-gprof lizard
