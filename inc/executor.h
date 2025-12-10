/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 19:31:10 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 17:04:24 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_command	t_command;
typedef struct s_env		t_env;

int		execute(t_command *cmd, t_env *env);
int		execute_pipeline(t_command *cmd, t_env *env);
int		execute_pipeline_core(t_command *cmd, t_env *env);
int		execute_single_command(t_command *cmd, t_env *env);
int		execute_external_command(t_command *cmd, t_env *env);

int		is_builtin_command(t_command *cmd);
int		execute_builtins(t_command *cmd, t_env *env);
int		builtin_echo(t_command *cmd);
int		builtin_cd(t_command *cmd, t_env *env);
int		builtin_pwd(t_command *cmd, t_env *env);
int		builtin_export(t_command *cmd, t_env *env);
int		builtin_unset(t_command *cmd, t_env *env);
int		builtin_env(t_command *cmd, t_env *env);
int		builtin_exit(t_command *cmd);
int		builtin_dot(t_command *cmd);

void	child_process(t_command *cmd, t_env *env);
int		handle_redirections(t_command *cmd, t_env *env);
int		handle_redirections_only(t_command *cmd);
char	*find_command_path(char *cmd, t_env *env);
void	reset_signals_to_default(void);

t_env	*env_create(char **envp);
void	env_destroy(t_env *env);
void	env_set(t_env *env, const char *var_str);
char	*env_get(t_env *env, const char *name);
void	env_unset(t_env *env, const char *name);
void	env_expand(t_env *env);
char	**env_to_array(t_env *env);

int		is_valid_var_name(char *str);
void	cleanup_empty_args(t_command *cmd);
int		is_a_directory(const char *path);
int		ft_atol(const char *nptr, int *value);
int		is_integer(char *str);

#endif