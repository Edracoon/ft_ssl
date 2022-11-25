NNAME=Gomoku

CPPFLAGS = -Wall -Wextra -Werror -std=c++17 -g -fsanitize=address

SRCS = ${wildcard srcs/*.cpp}
OBJS = $(SRCS:.cpp=.o)

${NAME}: ${OBJS}
	g++ ${CPPFLAGS} ${OBJS} -o ${NAME}

all: ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean ${NAME}

.PHONY: all clean fclean re