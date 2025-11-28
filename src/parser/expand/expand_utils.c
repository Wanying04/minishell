/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:18 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (NULL);
	name = malloc(len + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, str + start, len + 1);
	return (name);
}

void	update_quote_state(char c, t_quote_state *qs)
{
	if (c == '\x01')
		qs->state = 1;
	else if (c == '\x02')
		qs->state = 2;
	else if (c == '\'')
	{
		if (qs->state == 0)
			qs->state = 1;
		else if (qs->state == 1)
			qs->state = 0;
	}
	else if (c == '"')
	{
		if (qs->state == 0)
			qs->state = 2;
		else if (qs->state == 2)
			qs->state = 0;
	}
}

int	should_expand(char c, char next, t_quote_state *qs)
{
	if (c != '$')
		return (0);
	if (!next || (!ft_isalpha(next) && next != '_' && next != '?'))
		return (0);
	if (qs->state == 1)
		return (0);
	return (1);
}
