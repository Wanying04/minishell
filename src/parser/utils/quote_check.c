/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:18:23 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/10 17:31:00 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

static int	handle_quote(char *input, int *i, char *quote_type, int *in_quotes)
{
	if (is_quote(input[*i]) && !*in_quotes)
	{
		*in_quotes = 1;
		*quote_type = input[*i];
	}
	else if (input[*i] == *quote_type && *in_quotes)
	{
		*in_quotes = 0;
		*quote_type = 0;
	}
	return (0);
}

int	ft_check_quotes(char *input)
{
	int		i;
	char	quote_type;
	int		in_quotes;

	if (!input)
		return (1);
	i = 0;
	in_quotes = 0;
	quote_type = 0;
	while (input[i])
	{
		handle_quote(input, &i, &quote_type, &in_quotes);
		i++;
	}
	return (in_quotes);
}
