/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:18 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/04 15:56:19 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Determina si hay que expandir una variable en heredoc,
//lo cual si existe siempre hay que hacer EXCEPTO si el delimitador
//estaba entre comillas
static int	should_expand_heredoc(char *str, int i)
{
	//Debe haber un dólar y algo después
	if (str[i] == '$' && str[i + 1])
	{
		//Nombre de variable válido
		if (ft_isalpha(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?')
			return (1);
	}
	return (0);
}
//Procesa y expande una variable en heredoc
static char	*process_heredoc_var(char *result, char *str, int *i, t_env *env)
{
	char	*var_name;

	//Extrae el nombre de la variable (USER de $USER)
	var_name = get_var_name(str, i);
	if (var_name)
	{
		//Busca el valor en env y lo añade al resultado
		result = replace_var(result, var_name, env);
		free(var_name);
	}
	return (result);
}
//Expande variables en una línea de heredoc
char	*expand_heredoc(char *str, t_env *env/* , int dont_expand */)
{
	char	*result;
	int		i;
	int		start;

	if (!str)
		return (ft_strdup(""));
	//Si el delimitador estaba entre comillas, NO expande
	/* if (dont_expand)
		return (ft_strdup(str)); *///o result = append_literal(result, str, start, i); */
	result = NULL;
	i = 0;
	start = 0;//Marca el inicio del texto literal
	while (str[i])
	{
		//Si encuentra una variable a expandir
		if (should_expand_heredoc(str, i))
		{
			//Añade el texto literal desde start hasta i
			result = append_literal(result, str, start, i);
			//Expande la variable u actualiza i
			result = process_heredoc_var(result, str, &i, env);
			//El próximo literal empieza donde terminó la varible
			start = i;
		}
		else
			i++;
	}
	//Añade el resto del texto literal final
	result = append_literal(result, str, start, i);
	//Si falló todo, retorna string vacío
	if (!result)
		return (ft_strdup(""));
	return (result);
}
