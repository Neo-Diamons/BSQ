##
## EPITECH PROJECT, 2024
## BSQ
## File description:
## Makefile
##

NAME		:=	bsq
EXT			:=	.c

DIR			:=	src
SRC			:=	$(addprefix $(DIR)/,								\
					main.c											\
					bsq.c											\
					get_file.c										\
					my_atoi.c										\
				)

DIR_UTEST	:=	tests/unit
SRC_UTEST	:=	$(addprefix $(DIR_UTEST)/,							\
				)

DIR_BUILD	:=	build

ROOT_OBJ	:=	$(addprefix $(DIR_BUILD)/,obj/)
DIR_OBJ		:=	$(addprefix $(ROOT_OBJ),$(DIR))
OBJ			:=	$(patsubst %$(EXT),$(ROOT_OBJ)%.o,$(SRC))

D_ROOT_OBJ	:=	$(addprefix $(DIR_BUILD)/debug/,obj/)
D_DIR_OBJ	:=	$(addprefix $(D_ROOT_OBJ),$(DIR))
D_OBJ		:=	$(patsubst %$(EXT),$(D_ROOT_OBJ)%.o,$(SRC))

TEST_DIR	:=	$(addprefix $(DIR_BUILD)/,tests/)
FUNC_TEST	:=	functional_test
UNIT_TEST	:=	unit_test

RM			:=	rm -rf

CXX			:=	gcc
CXXFLAGS	:=	-iquote./include -Wall -Wextra

all:				$(DIR_OBJ) $(NAME)

$(DIR_OBJ):
	@mkdir -p $@													\
	&& printf "\033[93m[CREATED]\033[0m %s\n" $(DIR_OBJ)			\
	|| printf "\033[31m[ERROR]\033[0m %s\n"   $(DIR_OBJ)

$(ROOT_OBJ)%.o:		%$(EXT)
	@$(CXX) $(CXXFLAGS) -c $< -o $@									\
	&& printf "\033[32m[OK]\033[0m %s\n" $<							\
	|| printf "\033[31m[KO]\033[0m %s\n" $<

$(NAME):			$(OBJ)
	@$(CXX) -o $(NAME) $(OBJ) $(CXXFLAGS)							\
	&& printf "\033[32m[SUCCES]\033[0m %s\n" $@						\
	|| printf "\033[31m[ERROR]\033[0m %s\n"  $@

##  _______        _
## |__   __|      | |
##    | | ___  ___| |_ ___
##    | |/ _ \/ __| __/ __|
##    | |  __/\__ \ |_\__ \
##    |_|\___||___/\__|___/

tests:				all
	./tests/functional/tester.sh ./$(NAME)							\
		./tests/files/maps ./tests/files/maps_solved				\
	&& printf "\033[32m[SUCCES]\033[0m %s\n" $@						\
	|| printf "\033[31m[ERROR]\033[0m %s\n"  $@

$(UNIT_TEST):		CFLAGS += -lcriterion --coverage
$(UNIT_TEST):		fclean
	@mkdir -p $(TEST_DIR)
	@$(CXX) $(CXXFLAGS) $(SRC) $(SRC_UTEST) -o $(TEST_DIR)$@		\
	&& printf "\033[32m[SUCCES]\033[0m %s\n" $@						\
	|| printf "\033[31m[ERROR]\033[0m %s\n"  $@

tests_run:			$(UNIT_TEST)
	@$(TEST_DIR)$(UNIT_TEST)
	gcovr $(TEST_DIR) --exclude tests/
	gcovr $(TEST_DIR) --exclude tests/ --branches

##  _____       _
## |  __ \     | |
## | |  | | ___| |__  _   _  __ _
## | |  | |/ _ \ '_ \| | | |/ _` |
## | |__| |  __/ |_) | |_| | (_| |
## |_____/ \___|_.__/ \__,_|\__, |
##                           __/ |
##                          |___/

$(D_DIR_OBJ):
	@mkdir -p $@													\
	&& printf "\033[93m[CREATED]\033[0m %s\n" $(DIR_OBJ)			\
	|| printf "\033[31m[ERROR]\033[0m %s\n"   $(DIR_OBJ)

$(D_ROOT_OBJ)%.o:	%$(EXT)
	@$(CXX) $(CXXFLAGS) -c $< -o $@									\
	&& printf "\033[32m[OK]\033[0m %s\n" $<							\
	|| printf "\033[31m[KO]\033[0m %s\n" $<

debug:				CXXFLAGS += -g
debug:				$(D_DIR_OBJ) $(D_OBJ)
	@$(CXX) -o $(NAME) $(D_OBJ) $(CXXFLAGS)							\
	&& printf "\033[32m[SUCCES]\033[0m %s\n" $(NAME)				\
	|| printf "\033[31m[ERROR]\033[0m %s\n"  $(NAME)

##   _____ _
##  / ____| |
## | |    | | ___  __ _ _ __
## | |    | |/ _ \/ _` | '_ \
## | |____| |  __/ (_| | | | |
##  \_____|_|\___|\__,_|_| |_|

clean:
	@[ -d $(DIR_BUILD) ]											\
	&& $(RM) $(DIR_BUILD)											\
	&& printf "\033[31m[DELETED]\033[0m %s\n" $(DIR_BUILD) || true

fclean:				clean
	@[ -f $(NAME) ]													\
	&& $(RM) $(NAME)												\
	&& printf "\033[31m[DELETED]\033[0m %s\n" $(NAME) || true

##   ____  _   _
##  / __ \| | | |
## | |  | | |_| |__   ___ _ __
## | |  | | __| '_ \ / _ \ '__|
## | |__| | |_| | | |  __/ |
##  \____/ \__|_| |_|\___|_|

re:					fclean all

.PHONY:				all tests tests_run debug clean fclean re 		\
					$(DIR_OBJ) $(ROOT_OBJ)%.o $(NAME) $(UNIT_TEST)

.SILENT:			all tests tests_run debug clean fclean re 		\
					$(DIR_OBJ) $(ROOT_OBJ)%.o $(NAME) $(UNIT_TEST)
