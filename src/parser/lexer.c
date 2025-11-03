#include "minishell.h"

char	*ft_get_special_token(char *input, int *i)
{
	char	buffer[2];

	buffer[0] = input[*i];
	buffer[1] = '\0';
	(*i)++;
	return (ft_strdup(buffer));
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

char	*ft_get_normal_token(char *input, int *i)
{
	char	*token;
	char	*temp;
	int		start;

	start = *i;
	token = NULL;
	while (input[*i] && !ft_isspace(input[*i]) && !ft_isspecial(input[*i]))
	{
		if (input[*i] == '"' || input[*i] == '\'')
		{
			if (*i > start)
				token = ft_join_tokens(token, ft_substr(input, start, *i - start));
			temp = ft_get_quoted_token(input, i);
			token = ft_join_tokens(token, temp);
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
	if (ft_check_quotes(input))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes", 2);
		return (NULL);
	}
	if (ft_check_escape_chars(input))
	{
		ft_putendl_fd("minishell: syntax error: invalid escape sequence", 2);
		return (NULL);
	}
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
