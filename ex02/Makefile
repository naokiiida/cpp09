NAME:=PmergeMe
CXX:=c++
STDFLG := -std=c++98 -pedantic
DEPFLG := -MMD -MP
WARNFLGS := -Wall -Wextra -Werror
CXXFLAGS = $(STDFLG) $(DEPFLG) $(WARNFLGS)
ODIR=obj/
SRCS:=main.cpp PmergeMe.cpp
OBJS= $(SRCS:%.cpp=$(ODIR)%.o)
DEPS= $(OBJS:.o=.d)

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
	./tests.sh

-include $(DEPS)

.PHONY: all clean fclean re
