#include "minishell.h"

int is_special(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

char **split_tokens(char *input)
{
	char **tokens = malloc(sizeof(char *) * 100);
	int i = 0, j = 0, k = 0;
	char buffer[1024];

	while (input[i])
	{
		if (isspace(input[i]))
			i++;
		else if (is_special(input[i]))
		{
			buffer[0] = input[i];
			buffer[1] = '\0';
			tokens[k++] = strdup(buffer);
			i++;
		}
		else
		{
			j = 0;
			while (input[i] && !isspace(input[i]) && !is_special(input[i]))
				buffer[j++] = input[i++];
			buffer[j] = '\0';
			tokens[k++] = strdup(buffer);
		}
	}
	tokens[k] = NULL;
	return tokens;
}

int main(void)
{
    char *line = "echo hola | grep h > salida.txt";
    char **tokens = split_tokens(line);

    for (int i = 0; tokens[i]; i++)
        printf("token[%d]: %s\n", i, tokens[i]);

    return 0;
}
