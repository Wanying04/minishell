/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:18:23 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/03 18:23:53 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Que si es comilla, basicamente
static int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}
//Solo importa si la primera comilla que abre es igual que la que cierra
static int	handle_quote(char *input, int *i, char *quote_type, int *in_quotes)
{
	//Si es comilla y no estábamos dentro marcamos que estamos dentro y guardamos el tipo de comilla
	if (is_quote(input[*i]) && !*in_quotes)
	{
		*in_quotes = 1;
		*quote_type = input[*i];
	}
	//Si es comilla pero estábamos dentro y es el mismo tipo que la que abrió, estamos fuera
	else if (input[*i] == *quote_type && *in_quotes)
	{
		*in_quotes = 0;
		*quote_type = 0;
	}
	return (0);
}
//Recorre el input para comprobar si las comillas están cerradas o no
int	ft_check_quotes(char *input)
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
		handle_quote(input, &i, &quote_type, &in_quotes);
		i++;
	}
	//Si devuelve 0, están cerradas. Si devuelve 1, no están cerradas
	return (in_quotes);
}
