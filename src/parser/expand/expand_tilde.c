/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:18 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/03 15:02:04 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Construye el path expandido concatenando HOME con el resto del path
//Llamada desde: expand_tilde() después de validar que debe expandir
static char	*expand_tilde_home(char *path, char *home)
{
	char	*expanded;
	size_t	len;

	//Si solo (~) -> retorna HOME
	if (path[1] == '\0')
		return (ft_strdup(home));
	//Si (~/algo) -> concatena HOME + /algo
	if (path[1] == '/')
	{
		//Calcula tamaño: HOME + /algo + \0
		len = ft_strlen(home) + ft_strlen(path + 1) + 1;
		expanded = malloc(len);
		if (!expanded)
			return (NULL);
		//Copia HOME
		ft_strlcpy(expanded, home, len);
		//Añade /algo (path+1 salta el ~)
		ft_strlcat(expanded, path + 1, len);
		return (expanded);
	}
	//(~user) -> NO expande
	return (path);
}
//Verifica si debe expandir ~ y delega la construcción del path
//Llamada desde: expand_vars() o el executor cuando procesa argumentos
char	*expand_tilde(char *path, t_env *env)
{
	char	*home;

	//No expande si: NULL, marcado como quoted, o no empieza con ~
	if (!path || path[0] == '\x01' || path[0] != '~')
		return (path);
	//Busca el valor de HOME en el entorno
	home = env_get(env, "HOME");
	if (!home)
		return (path);
	//Reemplaza ~ por HOME
	return (expand_tilde_home(path, home));
}
