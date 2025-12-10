#include "minishell.h"

static char *handle_dollar_before_quotes(char *token, char quote, int *i)
{
	char	*temp;

	token[ft_strlen(token) - 1] = '\0';
	if (quote == '\'')
		temp = ft_strdup("\x01\x01");
	else
		temp = ft_strdup("\x02\x02");
	token = ft_join_tokens(token, temp);
	(*i) += 2;
	return (token);
}

static char *handle_empty_quote_markers(char *token, char quote, char *input, int *i)
{
	char	*temp;
	int		empty_count = 0;

	while (input[*i] == quote && input[*i + 1] == quote)
	{
		empty_count++;
		(*i) += 2;
	}
	if (empty_count > 0)
	{
		if (quote == '\'')
			temp = ft_strdup("\x01\x01");
		else
			temp = ft_strdup("\x02\x02");
		token = ft_join_tokens(token, temp);
		return (token);
	}
	return (NULL);
}

char *handle_empty_quotes(char *token, char quote, char *input, int *i)
{
	int	token_len = 0;

	if (token)
		token_len = ft_strlen(token);
	if (token_len > 0 && token[token_len - 1] == '$')
		return (handle_dollar_before_quotes(token, quote, i));
	return (handle_empty_quote_markers(token, quote, input, i));
}

char *extract_quoted_content(char *token, char *input, int *i)
{
	char	*temp = ft_get_quoted_token(input, i);

	return (ft_join_tokens(token, temp));
}
