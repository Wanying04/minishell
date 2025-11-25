/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:18 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_variable(char *result, char *str, int *i, t_env *env)
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

static void	init_expand_state(int *i, int *start, t_quote_state *qs)
{
	*i = 0;
	*start = 0;
	qs->state = 0;
}

static char	*process_expand_loop(char *str, char *result, t_env *env)
{
	int				i;
	int				start;
	t_quote_state	qs;

	init_expand_state(&i, &start, &qs);
	while (str[i])
	{
		update_quote_state(str[i], &qs);
		if (str[i] == '\x01' || str[i] == '\x02')
		{
			result = append_literal(result, str, start, i);
			i++;
			start = i;
		}
		else if (should_expand(str[i], str[i + 1], &qs))
		{
			result = append_literal(result, str, start, i);
			result = process_variable(result, str, &i, env);
			start = i;
		}
		else
			i++;
	}
	return (append_literal(result, str, start, i));
}

char	*expand_variables(char *str, t_env *env, int in_single_quote)
{
	char	*result;

	if (!str)
		return (ft_strdup(""));
	if (in_single_quote)
		return (ft_strdup(str));
	result = process_expand_loop(str, NULL, env);
	if (!result)
		return (ft_strdup(""));
	return (result);
}
