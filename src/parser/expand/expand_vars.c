/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:18 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/04 12:54:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Procesa una variable encontrada: extrae su nombre y la reemplaza por su valor
static char	*process_variable(char *result, char *str, int *i, t_env *env)
{
	char	*var_name;

	//Extrae el nombre de la variable después del $
	var_name = get_var_name(str, i);
	if (var_name)
	{
		//Reemplaza la variable por su valor y lo añade a lo que teníamos (result)
		result = replace_var(result, var_name, env);
		free(var_name);
	}
	return (result);
}

//Inicializa las variables del estado de expansión y las comillas
static void	init_expand_state(int *i, int *start, t_quote_state *qs)
{
	*i = 0;
	*start = 0;
	qs->state = 0;
}

//Recorre el string carácter por carácter expandiendo variables
static char	*process_expand_loop(char *str, char *result, t_env *env)
{
	int				i;
	int				start;
	t_quote_state	qs;

	init_expand_state(&i, &start, &qs);
	while (str[i])
	{
		//Actualiza el estado de comillas (simple/doble)
		update_quote_state(str[i], &qs);
		//Si encuentra marcador de comilla (\x01 o \x02)
		if (str[i] == '\x01' || str[i] == '\x02')
		{
			//Añade el literal anterior (antes del marcador)
			result = append_literal(result, str, start, i);
			i++;
			start = i;//Se actualiza start después del marcador
		}
		//Si encuentra un $ expandible
		else if (should_expand(str[i], str[i + 1], &qs))
		{
			//Añade el literal anterior (antes del $)
			result = append_literal(result, str, start, i);
			//Procesa y expande la variable
			result = process_variable(result, str, &i, env);
			start = i;//Se actualiza start después de la variable
		}
		else
			i++;//Carácter normal, continúa
	}
	//Si falta algo por añadir, lo hace
	return (append_literal(result, str, start, i));
}

//Función principal de expansión de variables
char	*expand_variables(char *str, t_env *env, int in_single_quote) //in_single_quote sobra
{
	char	*result;

	if (!str)
		return (ft_strdup(""));
	//Si está en comillas simples no expande nada
	if (in_single_quote)
		return (ft_strdup(str));
	//Procesa el string expandiendo las variables
	result = process_expand_loop(str, NULL, env);
	if (!result)
		return (ft_strdup(""));
	return (result);
}
