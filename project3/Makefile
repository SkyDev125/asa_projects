# Directories
CURDIR := $(shell pwd)
OUTDIR = $(CURDIR)/out
REPORTDIR = $(CURDIR)/report
TESTDIR = $(CURDIR)/tests
STRESSDIR = $(CURDIR)/analysis

# Source Files
SRC = $(wildcard $(CURDIR)/*.py)
TEST_INPUTS = $(wildcard $(TESTDIR)/*.in)
TEST_OUTPUTS = $(wildcard $(TESTDIR)/*.out)
STRESS_INPUTS = $(wildcard $(STRESSDIR)/**/*.in)
TOTAL_ERRORS = 

# Targets
FILENAME = proj
TARGET = $(CURDIR)/$(FILENAME).py
INPUT_FILE = $(CURDIR)/$(FILENAME).in
TEST_OUT_EXT = .outme
STRESSTEST_EXT = .analysis.json

# Compiler and flags
# Remove compiler and flags since it's a Python project

# Default rule
all: test

# interactive rule
inter: test lizard

# Test rule
test: clean $(TESTDIR) $(TEST_INPUTS) $(TEST_OUTPUTS)
	@TOTAL_ERRORS=""; \
	TOTAL_PASSES=""; \
	FAILED_COUNT=0; \
	PASSED_COUNT=0; \
	echo "<================= Running Tests ==================>"; \
	for file in $(TEST_INPUTS); do \
		RELATIVE_FILE=$$(realpath --relative-to=. $${file%.in}$(TEST_OUT_EXT)); \
		python3 $(TARGET) < $$file > $${file%.in}.outme; \
		TMP_DIFF=$$(mktemp); \
		if ! diff $${file%.in}.outme $${file%.in}.out > $$TMP_DIFF; then \
			mv $$TMP_DIFF $${file%.in}.diff; \
			echo "[FAIL] $$(basename $${file%.in})"; \
			if [ $$FAILED_COUNT -gt 0 ]; then \
				TOTAL_ERRORS="$${TOTAL_ERRORS}\n$$RELATIVE_FILE"; \
			else \
				TOTAL_ERRORS="$$RELATIVE_FILE"; \
			fi; \
			FAILED_COUNT=$$(($$FAILED_COUNT + 1)); \
		else \
			echo "[PASS] $$(basename $${file%.in})"; \
			if [ $$PASSED_COUNT -gt 0 ]; then \
				TOTAL_PASSES="$${TOTAL_PASSES}\n$$RELATIVE_FILE"; \
			else \
				TOTAL_PASSES="$$RELATIVE_FILE"; \
			fi; \
			PASSED_COUNT=$$(($$PASSED_COUNT + 1)); \
		fi; \
	done; \
	if [ -n "$$TOTAL_ERRORS" ]; then \
		echo "------------------- Failed Tests -------------------"; \
		echo "$$TOTAL_ERRORS"; \
	fi; \
	if [ -n "$$TOTAL_PASSES" ]; then \
		echo "------------------- Passed Tests -------------------"; \
		echo "$$TOTAL_PASSES"; \
	fi; \
	echo "<================ $$PASSED_COUNT/$$(($$PASSED_COUNT + $$FAILED_COUNT)) tests - $$(expr $$PASSED_COUNT \* 100 \/ \( $$PASSED_COUNT + $$FAILED_COUNT \))% =================>"; \

stresstest: clean $(STRESSDIR) $(STRESS_INPUTS)
	@for file in $(STRESS_INPUTS); do \
		hyperfine -N --command-name "Benchmarking $$file" --runs 10 --input $$file --export-json $${file%.in}$(STRESSTEST_EXT) "python3 $(TARGET)"; \
		done; \

# Clean rule
clean:
	@rm -rf $(OUTDIR)
	@rm -rf $(REPORTDIR)
	@rm -f $(TESTDIR)/*.outme
	@rm -f $(TESTDIR)/*.diff
	@rm -f $(STRESSDIR)/**/*$(STRESSTEST_EXT)

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
	@mkdir -p $(OUTDIR)

# Ensure the report directory exists
$(REPORTDIR):
	@mkdir -p $(REPORTDIR)

# Run lizard
lizard: $(REPORTDIR)
	@lizard $(SRC)

# Phony rules
.PHONY: all inter test clean lizard
