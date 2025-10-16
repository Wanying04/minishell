/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:29:01 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/23 20:07:48 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	del(lst->content);
	free(lst);
}

/*static void	del(void *content)
{
	free(content);
}

int main(void)
{
	t_list *node1 = ft_lstnew(ft_strdup("A"));
	t_list *node2 = ft_lstnew(ft_strdup("B"));
	t_list *node3 = ft_lstnew(ft_strdup("C"));
	if (!node1 || !node2 || !node3)
	{
		if (node1)
			ft_lstdelone(node1, del);
		if (node2)
			ft_lstdelone(node2, del);
		if (node3)
			ft_lstdelone(node3, del);
		return (1);
	}

	node1->next = node2;
	node2->next = node3;

	// Eliminar los nodos en orden inverso para evitar perder referencias
	printf("Eliminando nodo 3...\n");
	ft_lstdelone(node3, del);
	node2->next = NULL;
	
	printf("Eliminando nodo 2...\n");
	ft_lstdelone(node2, del);
	node1->next = NULL;

	printf("Eliminando nodo 1...\n");
	ft_lstdelone(node1, del);

	printf("Todos los nodos eliminados correctamente\n");
	return (0);
}*/
