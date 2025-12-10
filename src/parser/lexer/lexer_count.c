/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_count.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:26 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/10 18:35:33 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_normal_token(char *input, int *i)
{
	while (input[*i] && !ft_isspace(input[*i]) && !ft_isspecial(input[*i])
		&& input[*i] != '"' && input[*i] != '\'')
		(*i)++;
}

int	ft_count_tokens(char *input)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '"' || input[i] == '\'')
			count += ft_skip_quoted_section(input, &i);
		else if (ft_isspecial(input[i]))
		{
			count++;
			i++;
		}
		else
		{
			count++;
			skip_normal_token(input, &i);
		}
	}
	return (count);
}
