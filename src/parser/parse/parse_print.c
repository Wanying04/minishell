/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/02 19:04:57 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Código de debug no hace falta entenderlo
static void	print_redir_type(t_redirect *redir)
{
	if (redir->type == REDIR_OUT)
		ft_putstr_fd("OUT->", 1);
	else if (redir->type == REDIR_APPEND)
		ft_putstr_fd("APPEND->", 1);
	else if (redir->type == REDIR_IN)
		ft_putstr_fd("IN->", 1);
	else if (redir->type == REDIR_HEREDOC)
		ft_putstr_fd("HEREDOC->", 1);
	ft_putstr_fd("\"", 1);
	ft_putstr_fd(redir->file, 1);
	ft_putstr_fd("\" ", 1);
}

static void	print_cmd_argv(t_command *curr)
{
	int	j;

	j = 0;
	if (curr->argv)
	{
		while (curr->argv[j])
		{
			ft_putstr_fd("\"", 1);
			ft_putstr_fd(curr->argv[j], 1);
			ft_putstr_fd("\" ", 1);
			j++;
		}
	}
	else
		ft_putstr_fd("(null)", 1);
}

static void	print_cmd_redirs(t_command *curr)
{
	int	j;

	if (curr->redirects && curr->redirect_count > 0)
	{
		j = 0;
		while (j < curr->redirect_count)
		{
			print_redir_type(&curr->redirects[j]);
			j++;
		}
		ft_putstr_fd("\n", 1);
	}
	else
		ft_putendl_fd("none", 1);
}

static void	print_single_command(t_command *curr, int idx)
{
	ft_putstr_fd("Command ", 1);
	ft_putnbr_fd(idx, 1);
	ft_putstr_fd(":\n  argv: ", 1);
	print_cmd_argv(curr);
	ft_putstr_fd("\n  heredoc: ", 1);
	if (curr->heredoc_delimiter)
		ft_putendl_fd(curr->heredoc_delimiter, 1);
	else
		ft_putendl_fd("(none)", 1);
	ft_putstr_fd("  redirects: ", 1);
	print_cmd_redirs(curr);
}

void	print_command_list(t_command *head)
{
	int			idx;
	t_command	*curr;

	if (!head)
	{
		ft_putendl_fd("[parser] empty command list", 1);
		return ;
	}
	idx = 0;
	curr = head;
	while (curr)
	{
		print_single_command(curr, idx);
		curr = curr->next;
		idx++;
	}
}
