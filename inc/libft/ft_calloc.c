/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:08:55 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/10 23:09:02 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	void	*memory;

	memory = malloc(nmemb * size);
	if (memory == NULL)
		return (NULL);
	i = 0;
	while (i < (nmemb * size))
	{
		((char *)memory)[i] = '\0';
		i++;
	}
	return (memory);
}

/*int	main(void)
{
	char	*str = ft_calloc(6, 0);
	if (str == NULL)
		printf("Error al asignar memoria\n");
	return (0);
}*/
