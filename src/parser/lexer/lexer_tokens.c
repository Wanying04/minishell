/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:26 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_special_token(char *input, int *i)
{
	int		pos;
	char	first;
	char	buf[3];

	pos = *i;
	first = input[pos];
	if ((first == '>' || first == '<') && input[pos + 1] == first)
	{
		buf[0] = first;
		buf[1] = first;
		buf[2] = '\0';
		*i = pos + 2;
		return (ft_strdup(buf));
	}
	buf[0] = first;
	buf[1] = '\0';
	*i = pos + 1;
	return (ft_strdup(buf));
}

static char	*ft_join_tokens(char *s1, char *s2)
{
	char	*result;
	int		len1;
	int		len2;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, len1 + 1);
	ft_strlcpy(result + len1, s2, len2 + 1);
	free(s1);
	free(s2);
	return (result);
}

static char	*process_quoted_section(char *input, int *i, int start)
{
	char	*token;
	char	*temp;

	token = NULL;
	if (*i > start)
		token = ft_join_tokens(token, ft_substr(input, start, *i - start));
	temp = ft_get_quoted_token(input, i);
	token = ft_join_tokens(token, temp);
	return (token);
}

char	*ft_get_normal_token(char *input, int *i)
{
	char	*token;
	int		start;

	start = *i;
	token = NULL;
	while (input[*i] && !ft_isspace(input[*i]) && !ft_isspecial(input[*i]))
	{
		if (input[*i] == '"' || input[*i] == '\'')
		{
			token = process_quoted_section(input, i, start);
			start = *i;
		}
		else
			(*i)++;
	}
	if (*i > start)
		token = ft_join_tokens(token, ft_substr(input, start, *i - start));
	if (token)
		return (token);
	else
		return (ft_strdup(""));
}

void	ft_free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
