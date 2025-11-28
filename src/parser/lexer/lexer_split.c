/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:26 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_invalid_chars(char *input)
{
	int	j;

	j = 0;
	while (input[j])
	{
		if (input[j] == '\\')
		{
			ft_putendl_fd("minishell: syntax error: invalid character '\\'",
				2);
			return (1);
		}
		if (input[j] == ';')
		{
			ft_putendl_fd("minishell: syntax error: invalid character ';'", 2);
			return (1);
		}
		j++;
	}
	return (0);
}

static void	fill_tokens(char **tokens, char *input)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (ft_isspecial(input[i]))
			tokens[k++] = ft_get_special_token(input, &i);
		else
			tokens[k++] = ft_get_normal_token(input, &i);
	}
	tokens[k] = NULL;
}

char	**ft_split_tokens(char *input)
{
	char	**tokens;
	int		token_count;

	if (!input)
		return (NULL);
	if (ft_check_quotes(input))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes", 2);
		return (NULL);
	}
	if (check_invalid_chars(input))
		return (NULL);
	token_count = ft_count_tokens(input);
	tokens = malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	fill_tokens(tokens, input);
	return (tokens);
}
