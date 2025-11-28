/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:18:30 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_get_quoted_size(char *input, int *i)
{
	char	quote;
	int		size;

	quote = input[*i];
	size = 0;
	(*i)++;
	while (input[*i])
	{
		if (input[*i] == quote)
			break ;
		size++;
		(*i)++;
	}
	if (input[*i])
		(*i)++;
	return (size);
}

static char	*append_char(char *str, char c)
{
	char	*new;
	int		len;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	new = malloc(len + 2);
	if (!new)
		return (str);
	if (str)
	{
		ft_strlcpy(new, str, len + 1);
		free(str);
	}
	new[len] = c;
	new[len + 1] = '\0';
	return (new);
}

char	*ft_get_quoted_token(char *input, int *i)
{
	char	quote;
	char	*token;

	quote = input[*i];
	token = NULL;
	if (quote == '\'')
		token = append_char(token, '\x01');
	else if (quote == '"')
		token = append_char(token, '\x02');
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		token = append_char(token, input[*i]);
		(*i)++;
	}
	if (input[*i] == quote)
		(*i)++;
	if (!token)
		return (ft_strdup(""));
	return (token);
}

int	ft_skip_quoted_section(char *input, int *i)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i])
		(*i)++;
	return (1);
}
