/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 09:24:16 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/23 09:57:26 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
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

	char	*res = (char *)ft_lstlast(head);
	printf("Result: %s\n", res);
	return (0);
}*/
