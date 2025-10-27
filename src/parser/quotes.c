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
	int		start;

	start = *i;
	quote = input[*i];
	size = ft_get_quoted_size(input + start, &start);
	token = malloc(size + 1);
	if (!token)
		return (NULL);
	*i = *i + 1;  // Skip opening quote
	j = 0;
	while (input[*i] && j < size)
	{
		if (input[*i] != quote)
			token[j++] = input[(*i)];
		(*i)++;
	}
	if (input[*i])  // Skip closing quote if found
		(*i)++;
	token[j] = '\0';
	return (token);
}

int	ft_skip_quoted_section(char *input, int *i)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && (input[*i] != quote || input[*i - 1] == '\\'))
		(*i)++;
	if (input[*i])
		(*i)++;
	return (1);
}
