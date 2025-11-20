CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib

SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests

APP_SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/mosh.c $(SRC_DIR)/executor.c $(SRC_DIR)/history.c $(SRC_DIR)/builtins.c $(SRC_DIR)/parser.c $(SRC_DIR)/utils.c
APP_OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(APP_SRCS:.c=.o)))
APP_EXEC = $(BUILD_DIR)/main

TEST_SRCS = $(TEST_DIR)/main_test.c $(SRC_DIR)/mosh.c $(SRC_DIR)/executor.c $(SRC_DIR)/history.c $(SRC_DIR)/builtins.c $(SRC_DIR)/parser.c $(SRC_DIR)/utils.c
TEST_OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(TEST_SRCS:.c=.o)))
TEST_EXEC = $(BUILD_DIR)/main_test

run: $(APP_EXEC)
	./$(APP_EXEC)

$(APP_EXEC): $(APP_OBJS)
	$(CC) $(APP_OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	gcc -c $< -o $@

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)