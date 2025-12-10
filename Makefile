# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/10 22:37:11 by wtang             #+#    #+#              #
#    Updated: 2025/12/10 22:37:12 by wtang            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell

CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror
INCLUDE		:= -Iinc -Iinc/libft -I/usr/local/include

SRC_DIR		:= src
OBJ_DIR		:= obj

LIBFT_DIR	:= inc/libft
LIBFT_A		:= $(LIBFT_DIR)/libft.a

# Source files
SRCS		:= src/main.c \
			   src/executor/builtins/builtin_cd.c \
			   src/executor/builtins/cd_utils.c \
			   src/executor/builtins/builtin_dot.c \
			   src/executor/builtins/builtin_echo.c \
			   src/executor/builtins/builtin_env.c \
			   src/executor/builtins/builtin_exit.c \
			   src/executor/builtins/builtin_export.c \
			   src/executor/builtins/builtin_pwd.c \
			   src/executor/builtins/builtin_unset.c \
			   src/executor/env/env_core.c \
			   src/executor/env/env_operations.c \
			   src/executor/env/path.c \
			   src/executor/env/env_to_array.c \
			   src/executor/execution/execute_external_command.c \
			   src/executor/execution/execute_pipeline_core.c \
			   src/executor/execution/execute_pipeline.c \
			   src/executor/execution/execute_single_command.c \
			   src/executor/execution/execute.c \
			   src/executor/redirection/handle_redirections_only.c \
			   src/executor/redirection/handle_redirections.c \
			   src/executor/redirection/handle_file_redirection.c \
			   src/executor/redirection/process_heredoc.c \
			   src/executor/signals.c \
			   src/executor/utils/utils.c \
			   src/executor/utils/utils2.c \
			   src/parser/expand/expand_heredoc.c \
			   src/parser/expand/expand_tilde.c \
			   src/parser/expand/expand_utils.c \
			   src/parser/expand/expand_utils2.c \
			   src/parser/expand/expand_vars.c \
			   src/parser/lexer/lexer_count.c \
			   src/parser/lexer/lexer_split.c \
			   src/parser/lexer/lexer_tokens.c \
			   src/parser/lexer/lexer_quoted_utils.c \
			   src/parser/parse/parse_args.c \
			   src/parser/parse/parse_core.c \
			   src/parser/parse/parse_ctx.c \
			   src/parser/parse/parse_handlers.c \
			   src/parser/parse/parse_node.c \
			   src/parser/parse/parse_token.c \
			   src/parser/utils/free_command.c \
			   src/parser/utils/quote_check.c \
			   src/parser/utils/quotes.c \
			   src/parser/utils/utils.c

OBJS		:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS		:= $(OBJS:.o=.d)

HEADERS		:= inc/minishell.h \
			   inc/parser.h \
			   inc/executor.h \
			   inc/libft/libft.h

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)
	@echo "💻 minishell created"
	@echo "-------------------------------"

$(LIBFT_A):
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)
	@echo "-------------------------------"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
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

.PHONY: all clean fclean re

-include $(DEPS)
