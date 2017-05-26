BIN = Wii_U_ticket_patcher

SRCS = source/main.c source/title.c

OBJS = $(SRCS:.c=.o)

INCLUDE = include/common.h

FLAG = -Wall -Wextra -Werror

CC = gcc

all: $(BIN)

$(BIN): $(OBJS)
		$(CC) -o $@ $^
		@printf "Compilation done\n"

%.o: %.c $(INCLUDE)
		@printf "$(notdir $<)\n"
		$(CC) -o $@ -c $< $(FLAG)

clean:
		@rm -fr $(OBJS)
		@printf "All .o deleted !\n"

fclean: clean
		@rm -fr $(BIN)

re: fclean all
