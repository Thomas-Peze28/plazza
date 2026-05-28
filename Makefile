##
## EPITECH PROJECT, 2026
## Plazza
## File description:
## Makefile
##

RED=\033[0;31m
GREEN=\033[0;32m
YELLOW=\033[0;33m
BLUE=\033[0;34m
MAGENTA=\033[0;35m
CYAN=\033[0;36m
RESET=\033[0m

SRC     =   src/main.cpp \
			src/Core/Core.cpp \
			src/Producer/Producer.cpp \
			src/Consumer/Consumer.cpp \
			src/Parser/Parser.cpp \
			src/ThreadPool/ThreadPool.cpp \
			src/Utils/Logger/Logger.cpp \

INCLUDE =  	src/Core/Core.hpp \
			src/Include/RTCError.hpp \
			src/Utils/SafeQueue/ISafeQueue.hpp \
			src/Utils/SafeQueue/SafeQueue.hpp \
			src/Producer/Producer.hpp \
			src/Consumer/Consumer.hpp \
			src/Parser/Parser.hpp \
			src/ThreadPool/ThreadPool.hpp \
			src/Utils/Logger/Logger.hpp \
			src/Kitchen/Recipe/IRecipe.hpp \
			src/Kitchen/Recipe/Recipe.hpp \

CXX     =   g++

OBJ     =   $(SRC:.cpp=.o)

NAME    =   plazza

CXXFLAGS = -Wextra -Wall $(addprefix -I, $(sort $(dir $(INCLUDE)))) -std=c++20

LDLIBS   = -lm -pthread

.SILENT:

all:    $(NAME)

$(NAME):    $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(CXXFLAGS) $(LDLIBS)
	@printf "$(GREEN)[OK]$(RESET) Binary $(CYAN)'$(NAME)'$(RESET) compiled successfully.\n"

clean:
	rm -f $(OBJ)
	@printf "$(YELLOW)[INFO]$(RESET) Object files removed.\n"

fclean: clean
	rm -f $(NAME)
	@printf "$(YELLOW)[INFO]$(RESET) Binary $(CYAN)'$(NAME)'$(RESET) removed.\n"

re:     fclean all
		@printf "$(BLUE)[INFO]$(RESET) Project rebuilt successfully.\n"

.PHONY: all clean fclean re tests_run clean_tests