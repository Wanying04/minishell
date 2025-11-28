/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:18 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_value(char *var_name, t_env *env, char **exit_str)
{
	char	*value;

	*exit_str = NULL;
	if (ft_strncmp(var_name, "?", 2) == 0)
	{
		*exit_str = ft_itoa(env->exit_status);
		if (!*exit_str)
			return ("");
		return (*exit_str);
	}
	value = env_get(env, var_name);
	if (!value)
		return ("");
	return (value);
}

static char	*create_new_result(char *result, char *value)
{
	char	*new_result;
	size_t	len;

	len = ft_strlen(result) + ft_strlen(value) + 1;
	new_result = malloc(len);
	if (!new_result)
		return (result);
	ft_strlcpy(new_result, result, len);
	ft_strlcat(new_result, value, len);
	return (new_result);
}

char	*replace_var(char *result, char *var_name, t_env *env)
{
	char	*value;
	char	*new_result;
	char	*exit_str;

	value = get_var_value(var_name, env, &exit_str);
	if (!result)
	{
		if (exit_str)
			return (exit_str);
		return (ft_strdup(value));
	}
	new_result = create_new_result(result, value);
	if (!new_result)
	{
		if (exit_str)
			free(exit_str);
		return (result);
	}
	free(result);
	if (exit_str)
		free(exit_str);
	return (new_result);
}

static char	*create_literal(char *result, char *str, int start, int end)
{
	char	*literal;
	char	*new_result;
	size_t	len;

	len = ft_strlen(result) + (end - start) + 1;
	new_result = malloc(len);
	if (!new_result)
		return (result);
	ft_strlcpy(new_result, result, len);
	literal = ft_substr(str, start, end - start);
	if (!literal)
		return (free(new_result), result);
	ft_strlcat(new_result, literal, len);
	free(result);
	free(literal);
	return (new_result);
}

char	*append_literal(char *result, char *str, int start, int end)
{
	int	literal_len;

	literal_len = end - start;
	if (literal_len <= 0)
		return (result);
	if (!result)
		return (ft_substr(str, start, literal_len));
	return (create_literal(result, str, start, end));
}
