/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:26 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/09 17:30:26 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Extrae operadores especiales
char	*ft_get_special_token(char *input, int *i)
{
	int		pos;
	char	first;
	char	buf[3];

	pos = *i;
	first = input[pos];
	//Esto es por si es APPEND (>>) o HEREDOC (<<)
	if ((first == '>' || first == '<') && input[pos + 1] == first)
	{
		buf[0] = first;
		buf[1] = first;
		buf[2] = '\0';
		*i = pos + 2;
		return (ft_strdup(buf));
	}
	//Esto por si es IN (<) o OUT (>)
	buf[0] = first;
	buf[1] = '\0';
	*i = pos + 1;
	return (ft_strdup(buf));
}
//Concatena dos strings y libera los originales para evitar memory leaks
char	*ft_join_tokens(char *s1, char *s2)
{
	char	*result;
	int		len1;
	int		len2;

	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	//Hace un nuevo malloc
	result = ft_calloc(len1 + len2 + 1, 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, len1 + 1);
	ft_strlcpy(result + len1, s2, len2 + 1);
	//Libera la memoria de los punteros anteriores reservada por ft_strdup
	free(s1);
	free(s2);
	return (result);
}

static char	*process_quoted_section(char *token, char *input, int *i, int start)
{
	char	*temp;

	if (*i > start)
		//Une lo que haya en token
		//con lo que ft_substr extrae del input, desde start, la longitud avanzada
		//antes de la comilla
		token = ft_join_tokens(token, ft_substr(input, start, *i - start));
	//Extrae la parte entre comillas
	temp = ft_get_quoted_token(input, i);
	//La une con el resto del token
	token = ft_join_tokens(token, temp);
	return (token);
}
//Extrae tokens normales (comandos, argumentos, archivos)
//Cada vez que se llama, extrae un token
char	*ft_get_normal_token(char *input, int *i)
{
	char	*token;
	int		start;

	start = *i;
	token = NULL;
	//Avanza mientras no sea el final, un espacio o un operador especial
	while (input[*i] && !ft_isspace(input[*i]) && !ft_isspecial(input[*i]))
	{
		if (input[*i] == '"' || input[*i] == '\'')
		{
			token = process_quoted_section(token, input, i, start);
			start = *i;
		}
		else
			(*i)++;
	}
	if (*i > start)
		//Si salió del while y quedan cosas por guardar en token, lo hace
		token = ft_join_tokens(token, ft_substr(input, start, *i - start));
	// Si no hay token pero hubo comillas (token podría ser NULL si solo espacios/nada)
	if (!token)
		return (ft_strdup(""));
	return (token);
}
//Libera el array de tokens completo
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
