# --------------------------------------------
# Variables
# --------------------------------------------

NAME          = libft.a
INCLUDE       = include/
OBJ_DIR       = obj/
BONUS_NAME    = libft_bonus.a
SRC_DIR       = src
BONUS_SRC_DIR = src_bonus/
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

SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
      ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
      ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c \
      ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c \
      ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
      ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c

SRC_BONUS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
            ft_lstadd_back_bonus.c ft_lstlast_bonus.c ft_lstdelone_bonus.c \
            ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c ft_lstmap_bonus.c

# Object files paths (Main and Bonus)
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
OBJ_BONUS = $(addprefix $(OBJ_DIR)/, $(SRC_BONUS:.c=.o))

# --------------------------------------------
# Targets
# --------------------------------------------

# Default target
all: $(NAME)

# Build Libc (custom static library)
$(NAME): $(OBJ)
	@echo "$(YELLOW)Creating: $(NAME)$(DEF_COLOR)"
	@$(AR) $(NAME) $(OBJ)
	@echo "$(GREEN)$(NAME) compiled!$(DEF_COLOR)"

# Build bonus library
bonus: $(OBJ_BONUS)
	@echo "$(YELLOW)Creating: $(BONUS_NAME)$(DEF_COLOR)"
	@$(AR) $(BONUS_NAME) $(OBJ_BONUS)
	@echo "$(GREEN)$(BONUS_NAME) compiled!$(DEF_COLOR)"

# Object files compilation (Main source files)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
#	@echo "$(YELLOW)Compiling: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Object files compilation (Bonus source files)
$(OBJ_DIR)/%.o: $(BONUS_SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling bonus: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------
# Clean and Remove Object Files
# --------------------------------------------

clean:
	@$(RM) -rf $(OBJ_DIR)
	@echo "$(BLUE)Libft object files cleaned!$(DEF_COLOR)"

fclean: clean
	@$(RM) $(NAME) $(BONUS_NAME)
	@echo "$(CYAN)Libft executables cleaned!$(DEF_COLOR)"

re: fclean all
	@echo "$(GREEN)Cleaned and rebuilt everything for libft!$(DEF_COLOR)"

# --------------------------------------------
# Phony Targets
# --------------------------------------------

.PHONY: all clean fclean re bonus

