CC := gcc
CXX := gcc
CFLAGS := -g
#CFLAGS := -g -DDEBUG
#CFLAGS := -Wall -Wextra
LDFLAGS := -lm

TARGET_EXEC := main

BUILD_DIR := ./build
SRC_DIRS := ./clox ./utils


#SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
SRCS := $(shell find $(SRC_DIRS) -name '*.c')

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)
# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS := $(INC_FLAGS) -MMD -MP

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# # Build step for C++ source
# $(BUILD_DIR)/%.cpp.o: %.cpp
#     mkdir -p $(dir $@)
#     $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

TEST_SRC_DIRS := ./clox ./utils ./tests/keywords
TEST_KEYWORD_TARGET_EXEC := keywords-test
TEST_KEYWORD_SRCS := tests/keywords/keyword-test.c clox/scanner.c utils/hashmap.c clox/clox.c
TEST_KEYWORD_OBJS := $(TEST_KEYWORD_SRCS:%=$(BUILD_DIR)/%.o)

TEST_KEYWORD_INC_DIRS := $(shell find $(TEST_SRC_DIRS) -type d)
TEST_KEYWORD_INC_FLAGS := $(addprefix -I,$(TEST_KEYWORD_INC_DIRS))
TEST_KEYWORD_CPPFLAGS := $(TEST_KEYWORD_INC_FLAGS) -MMD -MP

$(BUILD_DIR)/tests/$(TEST_KEYWORD_TARGET_EXEC): $(TEST_KEYWORD_OBJS)
	$(CXX) $(TEST_KEYWORD_OBJS) -o $@ $(LDFLAGS)
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(TEST_KEYWORD_CPPFLAGS) $(CFLAGS) -c $< -o $@


.PHONY: clean main test_keywords
clean:
	rm -r $(BUILD_DIR)
main: $(BUILD_DIR)/$(TARGET_EXEC)
test_keywords: $(BUILD_DIR)/tests/$(TEST_KEYWORD_TARGET_EXEC)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
