##
## Blib tests, 2019
## Makefile
##

RM	=	rm -f
CC	=	g++

################################################################################

NAME	=	tests

SRCS	=	B/Directory/scan.cpp		\
		B/Vector/assign.cpp		\
		B/Vector/capacity.cpp		\
		B/Vector/elementAccess.cpp	\
		B/Vector/modifiers.cpp		\
		B/Vector/operations.cpp		\

OBJS	=	$(SRCS:.cpp=.o)

################################################################################

CXXFLAGS	+=	-std=c++17
CXXFLAGS	+=	-I../sources
CXXFLAGS	+=	-W -Wall -Wextra
CXXFLAGS	+=	-g3

LDFLAGS		+=	-L../build/ -lB
LDFLAGS		+=	-lcriterion

################################################################################

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

dump:
	@ echo "CXXFLAGS: [$(CXXFLAGS)]"
	@ echo "LDFLAGS: [$(LDFLAGS)]"

.PHONY: all $(NAME) clean fclean re dump