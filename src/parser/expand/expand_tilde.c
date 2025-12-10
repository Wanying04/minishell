/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:18 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/10 17:12:26 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_tilde_home(char *path, char *home)
{
	char	*expanded;
	size_t	len;

	if (path[1] == '\0')
		return (ft_strdup(home));
	if (path[1] == '/')
	{
		len = ft_strlen(home) + ft_strlen(path + 1) + 1;
		expanded = ft_calloc(len, 1);
		if (!expanded)
			return (NULL);
		ft_strlcpy(expanded, home, len);
		ft_strlcat(expanded, path + 1, len);
		return (expanded);
	}
	return (path);
}

char	*expand_tilde(char *path, t_env *env)
{
	char	*home;

	if (!path || path[0] == '\x01' || path[0] != '~')
		return (path);
	home = env_get(env, "HOME");
	if (!home)
		return (path);
	return (expand_tilde_home(path, home));
}
