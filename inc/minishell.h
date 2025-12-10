/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 21:46:25 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 21:48:27 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <string.h>        
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>  
# include <termios.h>
# include <dirent.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "parser.h"
# include "executor.h"

# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_APPEND 3
# define REDIR_HEREDOC 4

# define SUCCESS 0
# define FAILURE 1
# define SYNTAX_ERROR 2

extern volatile sig_atomic_t	g_sigint_received;

typedef struct s_pwd
{
	char	*pwd;
	char	*oldpwd;
}	t_pwd;

typedef struct s_env
{
	char	**vars;
	int		count;
	int		capacity;
	t_pwd	*pwd;
	int		exit_status;
}	t_env;

typedef struct s_redirect
{
	char	*file;
	int		type;
	int		fd;
}	t_redirect;

typedef struct s_command
{
	char				**argv;
	t_redirect			*redirects;
	int					redirect_count;
	struct s_command	*next;
	struct s_command	*prev;
	int					is_builtin;
	int					is_piped;
	char				*heredoc_delimiter;
	int					dont_expand;
}	t_command;

typedef struct s_dupinfo
{
	char	*file;
	int		flags;
	int		mode;
	int		target_fd;
	int		should_dup;
}	t_dupinfo;

void	setup_signals(void);
void	handle_signal(int sig);
void	ignore_sigint(void);

t_env	*env_create(char **envp);
void	env_destroy(t_env *env);
char	*env_get(t_env *env, const char *name);
void	env_set(t_env *env, const char *var_str);
void	env_unset(t_env *env, const char *name);
int		execute(t_command *cmd, t_env *env);

char	*remove_last_component(char *path);
char	*build_logical_path(char *current_pwd, char *target);
void	set_env_var(t_env *env, const char *name, const char *value);
void	update_pwd_oldpwd(t_env *env, char *old_cwd, char *new_cwd);

char	*expand_tilde(char *path, t_env *env);
char	*expand_variables(char *str, t_env *env, int in_single_quote);
char	*expand_heredoc(char *str, t_env *env, int dont_expand);
int		cleanup_and_exit(t_env *env);
int		is_empty_input(const char *str);
int		process_heredoc(char *delimiter, t_env *env, int dont_expand,
			int should_dup);
int		handle_file_redirection(t_redirect *redir, int should_dup);

#endif