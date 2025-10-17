NAME		:= minishell

CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror
INCLUDE		:= -Iinc -Iinc/libft -I/usr/local/include

SRC_DIR		:= src
OBJ_DIR		:= obj

LIBFT_DIR	:= inc/libft
LIBFT_A		:= $(LIBFT_DIR)/libft.a

LIBFT_SRCS	:= $(wildcard $(LIBFT_DIR)/*.c)
SRCS		:= $(wildcard $(SRC_DIR)/*.c)
OBJS		:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

DEPS		:= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)
	@echo "💻 minishell created"
	@echo "-------------------------------"

$(LIBFT_A): $(LIBFT_SRCS)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)
	@echo "-------------------------------"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE) -MMD -MP -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean
	@echo "🚮 minishell object files deleted"
	@echo "-------------------------------"

fclean:
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean
	@$(RM) -r $(OBJ_DIR)
	@$(RM) $(NAME)
	@echo "🚮 minishell object files deleted"
	@echo "🚮 minishell binary deleted"
	@echo "-------------------------------"

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
