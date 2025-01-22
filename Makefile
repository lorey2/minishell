# --------------------------------------------
# Variables
# --------------------------------------------

NAME          = minishell
NAME_BONUS    = minishell_bonus
INCLUDE       = include
SRC_DIR       = src/
SRC_DIR_BONUS = src_bonus/
OBJ_DIR       = obj/
OBJ_DIR_BONUS = obj_bonus/
CC            = gcc
CFLAGS        = -g -Wall -Werror -Wextra -I$(INCLUDE)
RM            = rm -f
AR            = ar rcs

# --------------------------------------------
# Colors
# --------------------------------------------

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

# --------------------------------------------
# Source and Object Files
# --------------------------------------------

SRC_FILES        = minishell handle_signal \
				   init setup \
				   execute exec \
				   parsing \
				   builtins cd pwd echo env exit unset update_env\
				   free error_free 
SRC_BONUS_FILES  =

SRC              = $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
SRC_BONUS        = $(addprefix $(SRC_DIR_BONUS), $(addsuffix .c, $(SRC_BONUS_FILES)))
OBJ              = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
OBJ_BONUS        = $(addprefix $(OBJ_DIR_BONUS), $(addsuffix .o, $(SRC_BONUS_FILES)))

# --------------------------------------------
# Libc
# --------------------------------------------

LIBC_DIR = ./libft
LIBC = $(LIBC_DIR)/libft.a

# --------------------------------------------
# Targets
# --------------------------------------------

# Default target
all: $(NAME)

$(LIBC):
	make -C $(LIBC_DIR) all
# Minishell Compilation
$(NAME): $(LIBC) $(OBJ)
	@$(CC) $(OBJ) -o $(NAME) -Llibft -lft -lreadline -lhistory
	@echo "$(GREEN)minishell compiled!$(DEF_COLOR)"

# Minishell Bonus Compilation
bonus: $(OBJ_BONUS)
	@$(CC) $(OBJ_BONUS) -o $(NAME_BONUS)
	@echo "$(GREEN)minishell bonus compiled!$(DEF_COLOR)"

# Object files compilation (Main)
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Object files compilation (Bonus)
$(OBJ_DIR_BONUS)%.o: $(SRC_DIR_BONUS)%.c
	@mkdir -p $(OBJ_DIR_BONUS)
	@echo "$(YELLOW)Compiling Bonus: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------
# Clean and Remove Object Files
# --------------------------------------------

# Clean object files
clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR_BONUS)
	make -C $(LIBC_DIR) clean
	@echo "$(BLUE)minishell object files cleaned!$(DEF_COLOR)"

# Full clean (objects and executables)
fclean: clean
	@rm -f $(NAME)
	@rm -f $(NAME_BONUS)
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
	@norminette $(SRC) $(SRC_BONUS) $(INCLUDE) | grep -v Norme -B1 || true

# --------------------------------------------
# Phony Targets
# --------------------------------------------

.PHONY: all clean fclean re norm bonus clean_bonus fclean_bonus
