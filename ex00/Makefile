NAME:=btc
CXX:=c++
SAN := -fsanitize=address -g
CHECK := --analyzer
STDFLG := -std=c++98
DEPFLG := -MMD -MP
WARNFLGS := -Wall -Wextra -Werror -Wshadow -Wno-shadow
CFLAGS = $(STDFLG) $(DEPFLG) $(WARNFLGS)
ODIR=obj/
SRCS:=main.cpp BitcoinExchange.cpp
OBJS= $(SRCS:%.cpp=$(ODIR)%.o)
DEPS= $(OBJS:.o=.d)
INPUT_FILE=input.txt

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CFLAGS) -o $(NAME) $^

$(ODIR)%.o: %.cpp | $(ODIR)
	$(CXX) $(CFLAGS) -o $@ -c $<

$(ODIR):
	mkdir $@

clean:
	rm -rf $(ODIR)

fclean: clean
	rm -rf $(NAME)

re: fclean
	@make all

leak: $(OBJS)
	$(CXX) $(CFLAGS) $(SAN) -o $(NAME) $^
	ASAN_OPTIONS=detect_leaks=1 ./$(NAME)

check: $(OBJS)
	$(CXX) $(CFLAGS) $(CHECK) -o $(NAME) $^
	./$(NAME)

run: $(NAME)
	./$< $(INPUT_FILE)

cmd:
	@compiledb make re

-include $(DEPS)

.PHONY: all clean fclean re
