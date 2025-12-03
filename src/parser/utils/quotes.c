/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:18:30 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/03 18:49:00 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Añadir un carácter a un string
static char	*append_char(char *str, char c)
{
	char	*new;
	int		len;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	//Se reserva memoria para el nuevo token. MALLOC.
	new = malloc(len + 2);
	if (!new)
		return (str);
	if (str)
	{
		//Se copia el string a la memoria reservada
		ft_strlcpy(new, str, len + 1);
		//Se libera el string anterior
		free(str);
	}
	//Se añade el marcador al final (o al principio si no había nada antes)
	new[len] = c;
	new[len + 1] = '\0';
	return (new);
}
//Añadir marcador al token según el tipo de comilla
static char	*add_quote_marker(char *token, char quote)
{
	if (quote == '\'')
		return (append_char(token, '\x01'));
	else
		return (append_char(token, '\x02'));
}
//Extraer un solo token con todo lo que haya entre comillas (incluyendo marcadores)
char	*ft_get_quoted_token(char *input, int *i)
{
	char	quote;
	char	*token;

	//Guardamos la comilla y avanzamos al siguiente carácter
	quote = input[(*i)++];
	//Creamos el token con el marcador
	token = add_quote_marker(NULL, quote);
	//Copiamos todo lo que está entre comillas al token
	while (input[*i] && input[*i] != quote)
		token = append_char(token, input[(*i)++]);
	//Si encontramos la comilla de cierre, añadimos el marcador de cierre
	if (input[*i] == quote)
	{
		(*i)++;
		token = add_quote_marker(token, quote);
	}
	//Por seguridad, si algo falló, devolvemos el token vacío
	if (!token)
		return (ft_strdup(""));
	return (token);
}

//Salta toda la sección entre comillas sin procesarla (útil para contar tokens o buscar espacios)
int	ft_skip_quoted_section(char *input, int *i)
{
	char	quote;

	//Guardamos qué tipo de comilla abre
	quote = input[*i];
	//Saltamos la comilla de apertura
	(*i)++;
	//Avanzamos hasta encontrar la comilla de cierre o el final del string
	while (input[*i] && input[*i] != quote)
		(*i)++;
	//Si encontramos la comilla de cierre, la saltamos también
	if (input[*i])
		(*i)++;
	return (1);
}
