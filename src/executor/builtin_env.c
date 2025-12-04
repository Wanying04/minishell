/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 18:57:23 by wtang             #+#    #+#             */
/*   Updated: 2025/12/04 18:57:24 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_command *cmd, t_env *env)
{
	int	i;
	(void)cmd;

	i = 0;
	while (i < env->count)
	{
		printf("%s\n", env->vars[i]);
		i++;
	}
	return (SUCCESS);
}
