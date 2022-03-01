SRC=src
OBJ=obj
TEST_OBJ=tests/obj
TEST=tests
BIN=bin

CFLAGS +=-W  -Wall -Wextra -g3 -Iinclude -fsanitize=address 
CFLAGS_NO_SANINATIZE +=-W -Wall -Wextra -g3 -Iinclude -pedantic
TESTFLAGS += -lcriterion -I../include -g3 -Wall -Wextra -pedantic -fsanitize=address 

CC=gcc
TARGET=$(BIN)/my_curl
RM=rm -rf

$(shell mkdir -p obj bin)

SRCS=$(wildcard $(SRC)/*.c)
TESTS=$(wildcard $(TEST)/*.c)

TEST_SRCS=$(filter-out $(SRC)/main.c, $(SRCS))
TEST_OBJS=$(patsubst $(SRC)/%.c, $(TEST_OBJ)/%.o, $(TEST_SRCS))

OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all: $(TARGET)

run: ${TARGET}
	./${TARGET}

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) 

$(OBJ)/%.o: $(SRC)/%.c 
	${CC} ${CFLAGS} -c $< -o $@

let_leak: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS_NO_SANINATIZE) $(LDFLAGS) $(LDLIBS)

$(OBJ)/%.o: $(SRC)/%.c 
	${CC} ${CFLAGS_NO_SANINATIZE} -c $< -o $@

test: $(TEST_OBJS)
	$(CC) -o $(TEST)/test $(TEST_OBJS) $(TESTS) $(TESTFLAGS); ./$(TEST)/test

$(TEST_OBJ)/%.o: $(SRC)/%.c 
	${CC} ${TESTFLAGS} -c $< -o $@

clean:
	$(RM) $(TARGET) $(BIN)/*.dSYM $(OBJ)/*.o 

test_clean:
	$(RM) $(TARGET) $(TEST)/test $(TEST_OBJ)/*.o 

.PHONY:
	all run clean test
