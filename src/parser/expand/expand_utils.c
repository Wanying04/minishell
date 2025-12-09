/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:18 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/09 20:36:40 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Extrae el nombre de una variable después del $
//Llamada desde: process_heredoc_var() y process_variable()
char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	//Salta el $
	(*i)++;
	//Salta los marcadores de comillas vacías después de $
	while (str[*i] == '\x01' || str[*i] == '\x02')
		(*i)++;
	//Caso especial: $? (exit status)
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));// ? es un nombre de una variable, sí
	}
	//Marca donde empieza el nombre de la variable
	start = *i;
	//Recorre mientras sean caracteres válidos típicos de una variable
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	//Al llegar al final calcula la longitud recorrida, es decir,
	//la longitud del nombre de la variable
	len = *i - start;
	//Si no hay nombre después del $, retorna NULL
	if (len == 0)
		return (NULL);
	//Hace malloc y copia el nombre
	name = ft_calloc(len + 1, 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, str + start, len + 1);
	return (name);
}
//Actualiza el estado de las comillas mientras recorre el string
//Llamada desde: process_expand_loop() en expand_vars.c en cada iteración del bucle
void	update_quote_state(char c, t_quote_state *qs)
{
	//Marca especial para comilla simple
	if (c == '\x01')
	{
		if (qs->state == 0)
			qs->state = 1;
		else if (qs->state == 1)
			qs->state = 0;
	}
	//Marca especial para comilla doble
	else if (c == '\x02')
	{
		if (qs->state == 0)
			qs->state = 2;
		else if (qs->state == 2)
			qs->state = 0;
	}
}
//Determina si debe expandir una variable según el contexto
//Llamada desde: process_expand_loop() en expand_vars.c antes de expandir cada variable
int	should_expand(char c, char next, t_quote_state *qs)
{
	//No es un $, no hay nada que expandir
	if (c != '$')
		return (0);
	//Después del dólar no hay nada válido (número, espacio, fin del string)
	if (!next || (!ft_isalpha(next) && next != '_' && next != '?'))
		return (0);
	//Dentro de comillas simples no se expande
	if (qs->state == 1)
		return (0);
	return (1);
}
