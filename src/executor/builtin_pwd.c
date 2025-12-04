/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 18:57:42 by wtang             #+#    #+#             */
/*   Updated: 2025/12/04 18:57:43 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_command *cmd)
{
	char	cwd[4096];

	(void)cmd;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: pwd");
		return (FAILURE);
	}
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	return (SUCCESS);
}
