/* ************************************************************************** */
/*                                                                           */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 12:57:50 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/09 15:24:58 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size <= 0)
		return (ft_strlen(src));
	if (size > 0)
	{
		while (i < size - 1 && src[i] != '\0')
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
		return (ft_strlen(src));
	}
	return (0);
}

/*int	main(void)
{
	char	dest[20];
	char	src[20] = "manolo";
	int	result;

	result	= ft_strlcpy(dest, src, 7);
	printf("Longitud de la cadena que se intentó copiar: %i\n", result);
	printf("Destino: %s\n", dest);
	return (0);
}*/
