#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || 
			c == '\v' || c == '\f' || c == '\r');
}

int	is_special(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

static char	*get_special_token(char *input, int *i)
{
	char	buffer[2];

	buffer[0] = input[*i];
	buffer[1] = '\0';
	(*i)++;
	return (ft_strdup(buffer));
}

static char	*get_normal_token(char *input, int *i)
{
	char	buffer[1024];
	int		j;

	j = 0;
	while (input[*i] && !ft_isspace(input[*i]) && !is_special(input[*i]))
		buffer[j++] = input[(*i)++];
	buffer[j] = '\0';
	return (ft_strdup(buffer));
}

static int	count_tokens(char *input)
{
    int	count;
    int	i;

    count = 0;
    i = 0;
    while (input[i])
    {
        if (ft_isspace(input[i]))
            i++;
        else if (is_special(input[i]))
        {
            count++;
            i++;
        }
        else
        {
            count++;
            while (input[i] && !ft_isspace(input[i]) && !is_special(input[i]))
                i++;
        }
    }
    return (count);
}

char	**split_tokens(char *input)
{
    char	**tokens;
    int		token_count;
    int		i;
    int		k;

    if (!input)
        return (NULL);
    token_count = count_tokens(input);
    tokens = malloc(sizeof(char *) * (token_count + 1));
    if (!tokens)
        return (NULL);
    i = 0;
    k = 0;
    while (input[i])
    {
        if (ft_isspace(input[i]))
            i++;
        else if (is_special(input[i]))
            tokens[k++] = get_special_token(input, &i);
        else
            tokens[k++] = get_normal_token(input, &i);
    }
    tokens[k] = NULL;
    return (tokens);
}

int main(void)
{
    char *line = "echo hola | grep h > salida.txt";
    char **tokens = split_tokens(line);

    for (int i = 0; tokens[i]; i++)
        printf("token[%d]: %s\n", i, tokens[i]);

    return 0;
}

// Pendiente de añadir y arreglar:
/*
static int	get_quoted_size(char *input, int *i)
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

static char	*get_quoted_token(char *input, int *i)
{
    char	quote;
    char	*token;
    int		size;
    int		j;
    int		start;

    start = *i;
    quote = input[*i];
    size = get_quoted_size(input + start, &start);
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

static char	*get_normal_token(char *input, int *i)
{
    int		size;
    char	*token;
    int		j;
    int		start;

    start = *i;
    size = 0;
    while (input[*i] && !ft_isspace(input[*i]) && !is_special(input[*i]))
    {
        if (input[*i] == '"' || input[*i] == '\'')
            return (get_quoted_token(input, i));
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

static int	skip_quoted_section(char *input, int *i)
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

static int	count_tokens(char *input)
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
            count += skip_quoted_section(input, &i);
        else if (is_special(input[i]))
        {
            count++;
            i++;
        }
        else
        {
            count++;
            while (input[i] && !ft_isspace(input[i]) && 
                   !is_special(input[i]) && input[i] != '"' && input[i] != '\'')
                i++;
        }
    }
    return (count);
}
*/
