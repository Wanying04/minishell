#include "minishell.h"

char	*ft_get_special_token(char *input, int *i)
{
	char	buffer[2];

	buffer[0] = input[*i];
	buffer[1] = '\0';
	(*i)++;
	return (ft_strdup(buffer));
}

char	*ft_get_normal_token(char *input, int *i)
{
	int		size;
	char	*token;
	int		j;
	int		start;

	start = *i;
	size = 0;
	while (input[*i] && !ft_isspace(input[*i]) && !ft_isspecial(input[*i]))
	{
		if (input[*i] == '"' || input[*i] == '\'')
			return (ft_get_quoted_token(input, i));
		size++;
		(*i)++;
	}
	token = malloc(size + 1);
	if (!token)
		return (NULL);
	j = 0;
	while (start < *i)
		token[j++] = input[start++];
	token[j] = '\0';
	return (token);
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
			while (input[i] && !ft_isspace(input[i]) && 
				!ft_isspecial(input[i]) && input[i] != '"' && input[i] != '\'')
				i++;
		}
	}
	return (count);
}

char	**ft_split_tokens(char *input)
{
	char	**tokens;
	int		token_count;
	int		i;
	int		k;

	if (!input)
		return (NULL);
	token_count = ft_count_tokens(input);
	tokens = malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
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
	return (tokens);
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
