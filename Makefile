SRC=src
OBJ=obj
BIN=bin

CFLAGS +=-W  -Wall -Wextra -g3 -Iinclude -fsanitize=address 
CFLAGS_NO_SANINATIZE +=-W -Wall -Wextra -g3 -Iinclude -pedantic

CC=gcc
TARGET=$(BIN)/my_curl
RM=rm -rf

$(shell mkdir -p obj bin)

SRCS=$(wildcard $(SRC)/*.c)
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

clean:
	$(RM) $(TARGET) $(BIN)/*.dSYM $(OBJ)/*.o 

.PHONY:
	all run clean
