/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 23:28:07 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/23 15:24:42 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!new)
		return ;
	new->next = *lst;
	*lst = new;
}

/*int	main(void)
{
	t_list	*node1 = ft_lstnew("A");
	t_list	*node2 = ft_lstnew("B");
	t_list	*head = node1;
	node1->next = node2;

	t_list	*new_node = ft_lstnew("X");
	ft_lstadd_front(&head, new_node);

	t_list	*temp = head;
	while (temp)
	{
		printf("%s -> ", (char *)temp->content);
		temp = temp->next;
	}
	printf("NULL\n");
	return (0);
}*/
