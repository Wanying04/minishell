/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 23:52:15 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/24 01:00:02 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f((void *)lst->content);
		lst = lst->next;
	}
}

/*static void	ft_tonext(void *s)
{
	if (!s)
		return ;
	char	*str;

	str = (char *)s;
	while (*str)
	{
		if(ft_isalpha(*str))
			*str = *str + 1;
		str++;
	}
}

int	main(void)
{
	t_list	*node1 = ft_lstnew(ft_strdup("A"));
	t_list	*node2 = ft_lstnew(ft_strdup("2"));
	t_list	*node3 = ft_lstnew(ft_strdup("C"));
	node1->next = node2;
	node2->next = node3;

	ft_lstiter(node1, ft_tonext);

	t_list	*temp = node1;
	if (!temp)
		return (1);
	while (temp)
	{
		printf("%s -> ", (char *)temp->content);
		temp = temp->next;
	}
	printf("NULL\n");
	ft_lstclear(&node1, free);
	return (0);
}*/
