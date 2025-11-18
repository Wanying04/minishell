#include "minishell.h"

int	ft_get_quoted_size(char *input, int *i)
{
	char	quote;
	int		size;

	quote = input[*i];
	size = 0;
	(*i)++;  // Skip the opening quote
	while (input[*i])
	{
		if (input[*i] == quote && input[*i - 1] != '\\')
			break ;
		size++;
		(*i)++;
	}
	if (input[*i])  // Skip the closing quote if found
		(*i)++;
	return (size);
}

char	*ft_get_quoted_token(char *input, int *i)
{
	char	quote;
	char	*token;
	int		size;
	int		j;

	quote = input[*i];
	j = *i + 1;
	size = 0;
	while (input[j] && input[j] != quote)
	{
		size++;
		j++;
	}
	if (quote == '\'')
		token = malloc(size + 2);
	else
		token = malloc(size + 1);
	if (!token)
		return (NULL);
	(*i)++;
	j = 0;
	if (quote == '\'')
		token[j++] = '\x01';
	while (input[*i] && input[*i] != quote)
		token[j++] = input[(*i)++];
	if (input[*i])
		(*i)++;
	token[j] = '\0';
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
