/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:53 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/09 15:29:34 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*temp;

	i = 0;
	temp = dest;
	if (!dest && !src)
		return (0);
	while (i < n)
	{
		temp[i] = ((const char *)src)[i];
		i++;
	}
	return (dest);
}

/*int     main(void)
{
	char    dest[] = "12345678910";
	//char  src[30] = "AAAAAAAAAAAAAAAAAAAAAAAAA";
	char    *result = memcpy(dest + 3, dest, 9);
	printf("Destino: %s\n", result);
	return (0);
}*/
