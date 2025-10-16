/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 01:03:56 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/24 11:53:40 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new_node;
	void	*content;

	if (!lst || !f || !del)
		return (NULL);
	new_lst = NULL;
	while (lst)
	{
		content = f(lst->content);
		if (!content)
			return (ft_lstclear(&new_lst, del), NULL);
		new_node = ft_lstnew(content);
		if (!new_node)
			return (del(content), ft_lstclear(&new_lst, del), NULL);
		ft_lstadd_back(&new_lst, new_node);
		lst = lst->next;
	}
	return (new_lst);
}

/*static void *ft_tonext(void *s)
{
    if (!s)
        return (NULL);
    char *str = (char *)s;
    char *new_str = ft_strdup(str);
    char *ptr = new_str;

    if (!new_str)
        return (NULL);
    while (*ptr)
    {
        if (ft_isalpha(*ptr))
            *ptr = *ptr + 1;
        ptr++;
    }
    return (new_str);
}

int main(void)
{
    t_list *node1 = ft_lstnew(ft_strdup("abc"));
    t_list *node2 = ft_lstnew(ft_strdup("def"));
    t_list *node3 = ft_lstnew(ft_strdup("xyz"));
    t_list *new_list;
    t_list *temp;

    if (!node1 || !node2 || !node3)
        return (1);

    node1->next = node2;
    node2->next = node3;

    new_list = ft_lstmap(node1, ft_tonext, free);

    printf("Original list:\n");
    temp = node1;
    while (temp)
    {
        printf("%s -> ", (char *)temp->content);
        temp = temp->next;
    }
    printf("NULL\n");

    printf("\nMapped list:\n");
    temp = new_list;
    while (temp)
    {
        printf("%s -> ", (char *)temp->content);
        temp = temp->next;
    }
    printf("NULL\n");

    ft_lstclear(&node1, free);
    ft_lstclear(&new_list, free);

    return (0);
}*/
