/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 18:58:15 by wtang             #+#    #+#             */
/*   Updated: 2025/12/04 18:58:16 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_env_array(char **env_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

char **env_to_array(t_env *env)
{
	char	**env_array;
	int		i;

	if (!env || !env->vars)
		return (NULL);
	env_array = malloc(sizeof(char *) * (env->count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (i < env->count)
	{
		env_array[i] = ft_strdup(env->vars[i]);
		if (!env_array[i])
			return (free_env_array(env_array, i), NULL);
		i++;
	}
	env_array[env->count] = NULL;
	return (env_array);
}
