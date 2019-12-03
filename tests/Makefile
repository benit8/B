##
## Blib tests, 2019
## Makefile
##

RM	=	rm -f
CC	=	g++

################################################################################

NAME	=	tests

SRCS	=	tests.cpp

OBJS	=	$(SRCS:.cpp=.o)

################################################################################

CXXFLAGS	+=	-std=c++17
CXXFLAGS	+=	-I../sources
CXXFLAGS	+=	-W -Wall -Wextra
CXXFLAGS	+=	-g3

LDFLAGS		+=	-L../.build/ -lB

################################################################################

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

raw: raw.o
	$(CC) -o raw raw.o $(CXXFLAGS) $(LDFLAGS)

dump:
	@ echo "CXXFLAGS: [$(CXXFLAGS)]"
	@ echo "LDFLAGS: [$(LDFLAGS)]"

.PHONY: all $(NAME) clean fclean re raw dump