/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:18 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/10 17:17:07 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Obtiene el valor de una variable del entorno
//Llamada desde replace_var()
static char	*get_var_value(char *var_name, t_env *env, char **exit_str)
{
	char	*value;

	*exit_str = NULL;
	//Caso especial: $? devuelve el valor del exit status del último comando
	if (ft_strncmp(var_name, "?", 2) == 0)
	{
		*exit_str = ft_itoa(env->exit_status);
		if (!*exit_str)
			return ("");
		return (*exit_str);
	}
	//Busca la variable en el entorno
	value = env_get(env, var_name);
	if (!value)
		return ("");
	return (value);
}
//Crea un nuevo string concatenando result + value
//Llamada desde replace_var()
static char	*create_new_result(char *result, char *value)
{
	char	*new_result;
	size_t	len;

	len = ft_strlen(result) + ft_strlen(value) + 1;
	new_result = ft_calloc(len, 1);
	if (!new_result)
		return (result);
	ft_strlcpy(new_result, result, len);
	ft_strlcat(new_result, value, len);
	return (new_result);
}
//Reemplaza una variable por su valor en el resultado
//Llamada desde: process_variable() en expand_vars.c
char	*replace_var(char *result, char *var_name, t_env *env)
{
	char	*value;
	char	*new_result;
	char	*exit_str;

	//printf("result: %s\n", result);
	//Obtiene el valor de la variable
	value = get_var_value(var_name, env, &exit_str);
	//Si result está vacío, devuelve directamente el valor
	if (!result)//Lo que hayamos leído hasta la variable
	{
		if (exit_str)
			return (exit_str);
		// Si el valor es vacío, devolver NULL para evitar alocar memoria innecesaria
		if (!value || !*value)
			return (NULL);
		return (ft_strdup(value));
	}
	//Concatena result (lo leído hasta la variable) con el valor de la variable
	new_result = create_new_result(result, value);
	if (!new_result)
	{
		if (exit_str)
			free(exit_str);
		return (result);
	}
	//printf("new_result: %s\n", new_result);
	free(result);
	if (exit_str)
		free(exit_str);
	return (new_result);
}

static char	*create_literal(char *result, char *str, int start, int end)
{
	char	*literal;
	char	*new_result;
	size_t	len;

	len = ft_strlen(result) + (end - start) + 1;
	new_result = ft_calloc(len, 1);
	if (!new_result)
		return (result);
	ft_strlcpy(new_result, result, len);
	literal = ft_substr(str, start, end - start);
	if (!literal)
		return (free(new_result), result);
	ft_strlcat(new_result, literal, len);
	free(result);
	free(literal);
	return (new_result);
}

char	*append_literal(char *result, char *str, int start, int end)
{
	int	literal_len;

	literal_len = end - start;
	if (literal_len <= 0)
		return (result);
	if (!result)
		return (ft_substr(str, start, literal_len));
	return (create_literal(result, str, start, end));
}
