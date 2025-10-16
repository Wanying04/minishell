/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 11:25:35 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/09 02:07:39 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*dst;
	const char	*source;

	dst = dest;
	source = src;
	if (!dest && !src)
		return (0);
	if (dst < source)
	{
		while (n--)
			*dst++ = *source++;
	}
	else
	{
		while (n--)
			dst[n] = source[n];
	}
	return (dest);
}

/*int     main(void)
{
	char    dest[10] = "";
	//char  src[3] = "abc";
	char    *result = ft_memmove(dest, "con\0sec\0\0te\0tur", 10);
	printf("Retorno: %s\n", result);
	printf("Destino: %s\n", dest);
	return (0);
}*/
