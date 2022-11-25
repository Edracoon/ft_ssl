NAME=ft_ssl

CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

SRCS = ${wildcard *.c} ${wildcard */*.c}
OBJS = $(SRCS:.c=.o)

${NAME}: ${OBJS}
	gcc ${CFLAGS} ${OBJS} -o ${NAME}

all: ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean ${NAME}

.PHONY: all clean fclean re