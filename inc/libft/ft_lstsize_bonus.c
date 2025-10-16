/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 02:12:21 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/23 02:28:56 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	if (!lst)
		return (0);
	while (lst)
	{
		lst = lst->next;
		count++;
	}
	return (count);
}

/*int	main(void)
{
	t_list	*node1 = ft_lstnew((void *)'A');
	t_list	*node2 = ft_lstnew((void *)'B');
	t_list	*node3 = ft_lstnew((void *)'C');
	t_list	*node4 = ft_lstnew((void *)'D');
	t_list	*head = node1;
	node1->next = node2;
	node2->next = node3;
	node3->next = node4;

	int	res = ft_lstsize(head);
	printf("Result: %i\n", res);
	return (0);
}*/
