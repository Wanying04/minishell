#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

// Standard C library headers
# include <stdio.h>          // printf, perror
# include <stdlib.h>         // malloc, free, exit, getenv
# include <unistd.h>         // write, access, read, close, fork, execve, dup, dup2, pipe, getcwd, chdir, isatty, ttyname, ttyslot
# include <fcntl.h>          // open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC, O_APPEND
# include <signal.h>         // signal, sigaction, sigemptyset, sigaddset, kill
# include <string.h>         // strerror
# include <sys/wait.h>       // wait, waitpid, wait3, wait4
# include <sys/stat.h>       // stat, lstat, fstat
# include <sys/ioctl.h>      // ioctl
# include <termios.h>        // tcsetattr, tcgetattr
# include <dirent.h>         // opendir, readdir, closedir

// Readline library headers
# include <readline/readline.h>  // readline, rl_on_new_line, rl_replace_line, rl_redisplay
# include <readline/history.h>   // add_history, rl_clear_history

# include "parser.h"
# include "executor.h"

// Constant definitions
# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_APPEND 3
# define REDIR_HEREDOC 4

# define SUCCESS 0
# define FAILURE 1
# define SYNTAX_ERROR 2

// Global variable
extern volatile sig_atomic_t g_sigint_received;

// Data structures
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
}	t_env;

typedef struct s_redirect
{
	char	*file;
	int		type;
	int		fd;
}	t_redirect;

typedef struct s_command
{
	char	**argv;
	t_redirect	*redirects;
	int		redirect_count;
	struct s_command	*next;
	struct s_command	*prev;
	int		is_builtin;
	char	*heredoc_delimiter;
}	t_command;

// ============ Signals (main.c) ============
void	setup_signals(void);
void	handle_signal(int sig);

// ============ Environment Management (executor/env.c) ============
// These functions are used across the entire project (parser, executor, main)
t_env	*env_create(char **envp);
void	env_destroy(t_env *env);
char	*env_get(t_env *env, const char *name);
void	env_set(t_env *env, const char *var_str);
void	env_unset(t_env *env, const char *name);

// ============ Executor Main Interface (executor/execute.c) ============
// Main execution entry point, called by main.c
int		execute(t_command *cmd, t_env *env);

// ============ Helper Functions ============
// expand_tilde is used by both parser and executor
char	*expand_tilde(char *path, t_env *env);

#endif