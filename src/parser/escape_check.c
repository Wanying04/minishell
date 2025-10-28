#include "minishell.h"

static int	is_valid_escape_char(char c)
{
	return (ft_strchr("\\\'\"$", c) != NULL);
}

static int	handle_backslash(char *input, int *i, int in_quotes, char quote_type)
{
	if (input[*i] == '\\')
	{
		if (!input[*i + 1])
			return (1);
		if (!in_quotes || quote_type == '\"')
		{
			if (!is_valid_escape_char(input[*i + 1]))
				return (1);
		}
		(*i)++;
	}
	return (0);
}

static int	update_quote_status(char c, int *in_quotes, char *quote_type)
{
	if ((c == '\'' || c == '\"') && !*in_quotes)
	{
		*in_quotes = 1;
		*quote_type = c;
	}
	else if (c == *quote_type && *in_quotes)
	{
		*in_quotes = 0;
		*quote_type = 0;
	}
	return (0);
}

int	ft_check_escape_chars(char *input)
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
		if (update_quote_status(input[i], &in_quotes, &quote_type))
			return (1);
		if (handle_backslash(input, &i, in_quotes, quote_type))
			return (1);
		i++;
	}
	return (0);
}