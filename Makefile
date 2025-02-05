# --------------------------------------------
# Variables
# --------------------------------------------

NAME          = minishell
INCLUDE       = include/
SRC_DIR       = src/
BUILTINS_DIR  = src/builtins/
OBJ_DIR       = obj/
CC            = gcc
CFLAGS        = -g -Wall -Werror -Wextra -I$(INCLUDE)
RM            = rm -f
AR            = ar rcs

# --------------------------------------------
# Colors (For colored output in terminal)
# --------------------------------------------

DEF_COLOR = \033[0;39m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
CYAN = \033[0;96m

# --------------------------------------------
# Source and Object Files
# --------------------------------------------

SRC_FILES        = minishell handle_signal \
                   init setup \
                   exec parsing \
                   update_env free error_free

BUILTINS_LIST    = builtins builtins_utils export export_2 cd pwd echo env exit unset

# Object files for main sources
OBJ_MAIN         = $(addprefix $(OBJ_DIR)/main/, $(addsuffix .o, $(SRC_FILES)))

# Object files for builtins
OBJ_BUILTINS     = $(addprefix $(OBJ_DIR)/builtins/, $(addsuffix .o, $(BUILTINS_LIST)))

# Combine both into one list
OBJ              = $(OBJ_MAIN) $(OBJ_BUILTINS)

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
	make -C $(LIBC_DIR) all

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

# --------------------------------------------
# Clean and Remove Object Files
# --------------------------------------------

# Clean object files
clean:
	@rm -rf $(OBJ_DIR)
	make -C $(LIBC_DIR) clean
	@echo "$(BLUE)minishell object files cleaned!$(DEF_COLOR)"

# Full clean (objects and executables)
fclean: clean
	@rm -f $(NAME)
	make -C $(LIBC_DIR) fclean
	@echo "$(CYAN)minishell executables cleaned!$(DEF_COLOR)"

# Rebuild everything
re: fclean all
	@echo "$(GREEN)Cleaned and rebuilt everything for minishell!$(DEF_COLOR)"

# --------------------------------------------
# Norminette Check
# --------------------------------------------

# Norminette code style check
norm:
	@norminette $(SRC_DIR) $(BUILTINS_DIR) $(INCLUDE) | grep -v Norme -B1 || true

# --------------------------------------------
# Phony Targets
# --------------------------------------------

.PHONY: all clean fclean re norm

