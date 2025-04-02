# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/12 15:46:35 by lorey             #+#    #+#              #
#    Updated: 2025/04/02 14:42:02 by maambuhl         ###   LAUSANNE.ch        #
#                                                                              #
# **************************************************************************** #

# --------------------------------------------
# Variables
# --------------------------------------------

NAME            = minishell
INCLUDE         = include/
SRC_DIR         = src/
BUILTINS_DIR    = src/builtins/
GNL_DIR         = src/gnl/
ANIM_DIR        = src/animations/
EXEC_DIR        = src/execution/
PRE_PARSING_DIR = src/pre_parsing/
PARSING_DIR     = src/parsing/
SETUP_DIR       = src/setup/
OTHER_DIR       = src/other/
OBJ_DIR         = obj/
CC              = gcc
CFLAGS          = -g -I$(INCLUDE) -Wall -Werror -Wextra
RM              = rm -f
AR              = ar rcs

# --------------------------------------------
# Colors (For colored output in terminal)
# --------------------------------------------

DEF_COLOR = \033[0;39m
GREEN     = \033[0;92m
YELLOW    = \033[0;93m
BLUE      = \033[0;94m
CYAN      = \033[0;96m

# --------------------------------------------
# Source and Object Files
# --------------------------------------------

SRC_FILES          = minishell

BUILTINS_LIST      = builtins builtins_utils export export_2 export_3 cd cd_2 pwd echo env exit unset

GNL_FILES          = get_next_line_bonus get_next_line_utils_bonus

ANIM_FILES         = animation explo_anim

EXEC_FILES         = exec here exec_utils process files here_utils

PRE_PARSING_FILES  = pre_parsing pre_pars_expans pre_pars_skip

PARSING_FILES      = parsing arg files helpers here_doc var arg_2 value

SETUP_FILES        = init init_2 setup setup_terminal

OTHER_FILES      = error_free handle_signal quote update_env utils update_env_error

# Object files for main sources
OBJ_MAIN         = $(addprefix $(OBJ_DIR)/main/, $(addsuffix .o, $(SRC_FILES)))

# Object files for builtins
OBJ_BUILTINS     = $(addprefix $(OBJ_DIR)/builtins/, $(addsuffix .o, $(BUILTINS_LIST)))

# Object files for gnl source files
OBJ_GNL          = $(addprefix $(OBJ_DIR)/gnl/, $(addsuffix .o, $(GNL_FILES)))

# Object files for anim source files
OBJ_ANIM         = $(addprefix $(OBJ_DIR)/animations/, $(addsuffix .o, $(ANIM_FILES)))

# Object files for exec source files
OBJ_EXEC         = $(addprefix $(OBJ_DIR)/execution/, $(addsuffix .o, $(EXEC_FILES)))

# Object files for pre_parsing source files
OBJ_PRE_PARSING  = $(addprefix $(OBJ_DIR)/pre_parsing/, $(addsuffix .o, $(PRE_PARSING_FILES)))

# Object files for parsing source files
OBJ_PARSING      = $(addprefix $(OBJ_DIR)/parsing/, $(addsuffix .o, $(PARSING_FILES)))

# Object files for setuo source files
OBJ_SETUP        = $(addprefix $(OBJ_DIR)/setup/, $(addsuffix .o, $(SETUP_FILES)))

# Object files for setuo source files
OBJ_OTHER        = $(addprefix $(OBJ_DIR)/other/, $(addsuffix .o, $(OTHER_FILES)))


# Combine both into one list
OBJ              = $(OBJ_MAIN) $(OBJ_BUILTINS) $(OBJ_GNL) $(OBJ_ANIM) $(OBJ_EXEC) $(OBJ_PRE_PARSING) $(OBJ_PARSING) $(OBJ_SETUP) $(OBJ_OTHER)

# --------------------------------------------
# Libc (custom library)
# --------------------------------------------

LIBC_DIR = ./libft
LIBC = $(LIBC_DIR)/libft.a

# --------------------------------------------
# Targets
# --------------------------------------------

# Default target
all: $(NAME)

# Build Libc
$(LIBC):
	@make -s -C $(LIBC_DIR) all

# Minishell Compilation
$(NAME): $(LIBC) $(OBJ)
	@$(CC) $(OBJ) -o $(NAME) -Llibft -lft -lreadline -lhistory
	@echo "$(GREEN)minishell compiled!$(DEF_COLOR)"

# Object files compilation (Main source files)
$(OBJ_DIR)/main/%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)/main
	@echo "$(YELLOW)Compiling: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Object files compilation (Builtin source files)
$(OBJ_DIR)/builtins/%.o: $(BUILTINS_DIR)%.c
	@mkdir -p $(OBJ_DIR)/builtins
	@echo "$(YELLOW)Compiling built-in: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Object files compilation (gnl source files)
$(OBJ_DIR)/gnl/%.o: $(GNL_DIR)%.c
	@mkdir -p $(OBJ_DIR)/gnl
	@echo "$(YELLOW)Compiling gnl: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Object files compilation (anim source files)
$(OBJ_DIR)/animations/%.o: $(ANIM_DIR)%.c
	@mkdir -p $(OBJ_DIR)/animations
	@echo "$(YELLOW)Compiling anim: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Object files compilation (exec source files)
$(OBJ_DIR)/execution/%.o: $(EXEC_DIR)%.c
	@mkdir -p $(OBJ_DIR)/execution
	@echo "$(YELLOW)Compiling execution: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Object files compilation (pre_parsing source files)
$(OBJ_DIR)/pre_parsing/%.o: $(PRE_PARSING_DIR)%.c
	@mkdir -p $(OBJ_DIR)/pre_parsing
	@echo "$(YELLOW)Compiling pre_parsing: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Object files compilation (parsing source files)
$(OBJ_DIR)/parsing/%.o: $(PARSING_DIR)%.c
	@mkdir -p $(OBJ_DIR)/parsing
	@echo "$(YELLOW)Compiling parsing: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Object files compilation (setup source files)
$(OBJ_DIR)/setup/%.o: $(SETUP_DIR)%.c
	@mkdir -p $(OBJ_DIR)/setup
	@echo "$(YELLOW)Compiling setup: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Object files compilation (setup source files)
$(OBJ_DIR)/other/%.o: $(OTHER_DIR)%.c
	@mkdir -p $(OBJ_DIR)/other
	@echo "$(YELLOW)Compiling other: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@


# --------------------------------------------
# Clean and Remove Object Files
# --------------------------------------------

# Clean object files
clean:
	@rm -rf $(OBJ_DIR)
	@make -s -C $(LIBC_DIR) clean
	@echo "$(BLUE)minishell object files cleaned!$(DEF_COLOR)"

# Full clean (objects and executables)
fclean: clean
	@rm -f $(NAME)
	@make -s -C $(LIBC_DIR) fclean
	@echo "$(CYAN)minishell executables cleaned!$(DEF_COLOR)"

# Rebuild everything
re: fclean all
	@echo "$(GREEN)Cleaned and rebuilt everything for minishell!$(DEF_COLOR)"

# --------------------------------------------
# Phony Targets
# --------------------------------------------

.PHONY: all clean fclean re norm

