/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:18 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/10 18:22:12 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	should_expand_heredoc(char *str, int i)
{
	if (str[i] == '$' && str[i + 1])
	{
		if (ft_isalpha(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?')
			return (1);
	}
	return (0);
}

static char	*process_heredoc_var(char *result, char *str, int *i, t_env *env)
{
	char	*var_name;

	var_name = get_var_name(str, i);
	if (var_name)
	{
		result = replace_var(result, var_name, env);
		free(var_name);
	}
	return (result);
}

char	*expand_heredoc(char *str, t_env *env, int dont_expand)
{
	char	*result;
	int		i;
	int		start;

	if (!str)
		return (ft_strdup(""));
	if (dont_expand)
		return (ft_strdup(str));
	result = NULL;
	i = 0;
	start = 0;
	while (str[i])
	{
		if (should_expand_heredoc(str, i))
		{
			result = append_literal(result, str, start, i);
			result = process_heredoc_var(result, str, &i, env);
			start = i;
		}
		else
			i++;
	}
	result = append_literal(result, str, start, i);
	if (!result)
		return (ft_strdup(""));
	return (result);
}
