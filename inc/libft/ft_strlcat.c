/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:48:12 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/09 18:27:04 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	temp;

	temp = ft_strlen(dst);
	i = 0;
	j = 0;
	if (size <= temp)
		return (ft_strlen(src) + size);
	while (dst[i])
		i++;
	while (i < size - 1 && src[j] != '\0')
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (ft_strlen(src) + temp);
}

/*int main(void)
{
	char dest[20] = "Hola ";
	char src[20] = "Manolo";
	int result;

	// Caso 1: Suficiente espacio en dest
	result = ft_strlcat(dest, src, sizeof(dest));
	printf("Caso 1:\n");
	printf("Longitud total esperada: %d\n", result);
	printf("Destino tras concatenar: '%s'\n\n", dest);

	// Caso 2: Sin espacio suficiente en dest
	char dest2[10] = "Hola ";
	result = ft_strlcat(dest2, src, sizeof(dest2));
	printf("Caso 2:\n");
	printf("Longitud total esperada: %d\n", result);
	printf("Destino tras concatenar: '%s'\n\n", dest2);

	// Caso 3: Espacio exacto para concatenar + '\0'
	char dest3[12] = "Hola ";
	result = ft_strlcat(dest3, src, sizeof(dest3));
	printf("Caso 3:\n");
	printf("Longitud total esperada: %d\n", result);
	printf("Destino tras concatenar: '%s'\n\n", dest3);

	// Caso 4: Tamaño del buffer = 0
	char dest4[20] = "Hola ";
	result = ft_strlcat(dest4, src, 0);
	printf("Caso 4:\n");
	printf("Longitud total esperada: %d\n", result);
	printf("Destino tras concatenar (no cambia): '%s'\n", dest4);

	// Caso 5: Tamaño del dest = 0
	char dest5[11] = "\0";
	result = ft_strlcat(dest5, "lorem ipsum", 15);
	printf("Caso 5:\n");
	printf("Longitud total esperada: %d\n", result);
	printf("Destino tras concatenar: '%s'\n", dest5);


	return 0;
}*/
