CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -MMD -MP
LDFLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib

SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests

# Auto-detect sources
APP_SRCS = $(wildcard $(SRC_DIR)/*.c)
APP_OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(APP_SRCS:.c=.o)))
APP_EXEC = $(BUILD_DIR)/main

# Test sources: all src/*.c except src/main.c, plus tests/main_test.c
TEST_APP_SRCS = $(filter-out $(SRC_DIR)/main.c, $(APP_SRCS))
TEST_SRCS = $(TEST_DIR)/main_test.c $(TEST_DIR)/autocomplete_test.c $(TEST_APP_SRCS)
TEST_OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(TEST_SRCS:.c=.o)))
TEST_EXEC = $(BUILD_DIR)/main_test

# Include dependency files
DEPS = $(APP_OBJS:.o=.d) $(TEST_OBJS:.o=.d)

.PHONY: run clean test

run: $(APP_EXEC)
	./$(APP_EXEC)

$(APP_EXEC): $(APP_OBJS)
	$(CC) $(APP_OBJS) -o $@ $(LDFLAGS)

$(TEST_EXEC): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $@ $(LDFLAGS)

# Build rule for source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build rule for test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: $(TEST_EXEC)
	./$(TEST_EXEC)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)