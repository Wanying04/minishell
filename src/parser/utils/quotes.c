/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:18:30 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/10 18:43:26 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_char(char *str, char c)
{
	char	*new;
	int		len;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	new = ft_calloc(len + 2, 1);
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

static char	*add_quote_marker(char *token, char quote)
{
	if (quote == '\'')
		return (append_char(token, '\x01'));
	else
		return (append_char(token, '\x02'));
}

char	*ft_get_quoted_token(char *input, int *i)
{
	char	quote;
	char	*token;

	quote = input[(*i)++];
	token = add_quote_marker(NULL, quote);
	while (input[*i] && input[*i] != quote)
		token = append_char(token, input[(*i)++]);
	if (input[*i] == quote)
	{
		(*i)++;
		token = add_quote_marker(token, quote);
	}
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
