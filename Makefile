NAME		:= minishell

CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror
INCLUDE		:= -Iinc -Iinc/libft -I/usr/local/include

SRC_DIR		:= src
OBJ_DIR		:= obj

LIBFT_DIR	:= inc/libft
LIBFT_A		:= $(LIBFT_DIR)/libft.a

# Source files
LIBFT_SRCS	:= $(wildcard $(LIBFT_DIR)/*.c)
MAIN_SRCS	:= $(wildcard $(SRC_DIR)/*.c)
PARSER_SRCS	:= $(shell find $(SRC_DIR)/parser -name '*.c')
EXECUTOR_SRCS:= $(wildcard $(SRC_DIR)/executor/*.c)

# Object files
MAIN_OBJS	:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(MAIN_SRCS))
PARSER_OBJS	:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(PARSER_SRCS))
EXECUTOR_OBJS:= $(patsubst $(SRC_DIR)/executor/%.c,$(OBJ_DIR)/executor/%.o,$(EXECUTOR_SRCS))

# All objects combined
OBJS		:= $(MAIN_OBJS) $(PARSER_OBJS) $(EXECUTOR_OBJS)

DEPS		:= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)
	@echo "💻 minishell created"
	@echo "-------------------------------"

$(LIBFT_A): $(LIBFT_SRCS)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)
	@echo "-------------------------------"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -MMD -MP -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

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

# Specific targets for parser and executor
parser: $(LIBFT_A) $(MAIN_OBJS) $(PARSER_OBJS)
	@$(CC) $(CFLAGS) $(MAIN_OBJS) $(PARSER_OBJS) $(LIBFT_A) -lreadline -o $(NAME)
	@echo "💻 minishell parser components compiled"
	@echo "-------------------------------"

executor: $(LIBFT_A) $(MAIN_OBJS) $(EXECUTOR_OBJS)
	@$(CC) $(CFLAGS) $(MAIN_OBJS) $(EXECUTOR_OBJS) $(LIBFT_A) -lreadline -o $(NAME)
	@echo "💻 minishell executor components compiled"
	@echo "-------------------------------"

.PHONY: all clean fclean re parser executor

-include $(DEPS)
