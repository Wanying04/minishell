#include "minishell.h"

// Function to expand ~
char *expand_tilde(char *path, t_env *env)
{
	char	*home;
	char	*expanded;
	size_t	len;

	if (!path || path[0] != '~')
		return (path);
	home = env_get(env, "HOME");
	if (!home)
		return (path);
	if (path[1] == '\0')
		return (ft_strdup(home));
	if (path[1] == '/')
	{
		len = ft_strlen(home) + ft_strlen(path + 1) + 1;
		expanded = malloc(len);
		if (!expanded)
			return (NULL);
		ft_strlcpy(expanded, home, len);
		ft_strlcat(expanded, path + 1, len);
		return (expanded);
	}
	return (path);
}

// Helper: obtener el nombre de la variable desde str[*i] (apuntando al $)
// Avanza i hasta después del nombre
static char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	(*i)++;  // saltar el $
	start = *i;
	
	// El nombre de variable puede ser alfanumérico o _
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	
	len = *i - start;
	if (len == 0)
		return (NULL);
	
	name = malloc(len + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, str + start, len + 1);
	return (name);
}

// Helper: reemplazar $VAR con su valor
static char	*replace_var(char *result, char *var_name, t_env *env)
{
	char	*value;
	char	*new_result;
	size_t	len;

	value = env_get(env, var_name);
	if (!value)
		value = "";  // Si no existe, es cadena vacía
	
	if (!result)
		return (ft_strdup(value));
	
	len = ft_strlen(result) + ft_strlen(value) + 1;
	new_result = malloc(len);
	if (!new_result)
		return (result);
	
	ft_strlcpy(new_result, result, len);
	ft_strlcat(new_result, value, len);
	free(result);
	return (new_result);
}

// Helper: agregar texto literal al resultado
static char	*append_literal(char *result, char *str, int start, int end)
{
	char	*literal;
	char	*new_result;
	size_t	len;
	int		literal_len;

	literal_len = end - start;
	if (literal_len <= 0)
		return (result);
	
	literal = malloc(literal_len + 1);
	if (!literal)
		return (result);
	ft_strlcpy(literal, str + start, literal_len + 1);
	
	if (!result)
	{
		free(literal);
		return (ft_substr(str, start, literal_len));
	}
	
	len = ft_strlen(result) + literal_len + 1;
	new_result = malloc(len);
	if (!new_result)
	{
		free(literal);
		return (result);
	}
	
	ft_strlcpy(new_result, result, len);
	ft_strlcat(new_result, literal, len);
	free(result);
	free(literal);
	return (new_result);
}

// Expandir variables de entorno $VAR
// Nota: NO expandir dentro de comillas simples
char	*expand_variables(char *str, t_env *env, int in_single_quote)
{
	char	*result;
	char	*var_name;
	int		i;
	int		start;

	if (!str)
		return (ft_strdup(""));
	if (str[0] == '\x01')
		return (ft_strdup(str + 1));
	if (in_single_quote)
		return (ft_strdup(str));
	
	result = NULL;
	i = 0;
	start = 0;
	
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
		{
			// Agregar el texto literal antes del $
			result = append_literal(result, str, start, i);
			
			// Obtener el nombre de la variable
			var_name = get_var_name(str, &i);
			if (var_name)
			{
				result = replace_var(result, var_name, env);
				free(var_name);
			}
			start = i;
		}
		else
			i++;
	}
	
	// Agregar el resto del texto
	result = append_literal(result, str, start, i);
	
	if (!result)
		return (ft_strdup(""));
	return (result);
}
