NAME:=btc
CXX:=c++
STDFLG := -std=c++98 -pedantic
DEPFLG := -MMD -MP
WARNFLGS := -Wall -Wextra -Werror
CXXFLAGS = $(STDFLG) $(DEPFLG) $(WARNFLGS)
ODIR=obj/
SRCS:=main.cpp BitcoinExchange.cpp
OBJS= $(SRCS:%.cpp=$(ODIR)%.o)
DEPS= $(OBJS:.o=.d)
INPUT_FILE=input.txt

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $^

$(ODIR)%.o: %.cpp | $(ODIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(ODIR):
	mkdir $@

clean:
	rm -rf $(ODIR)

fclean: clean
	rm -rf $(NAME)

re: fclean
	@make all

run: $(NAME)
	./$< $(INPUT_FILE)

-include $(DEPS)

.PHONY: all clean fclean re
