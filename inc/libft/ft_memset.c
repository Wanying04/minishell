/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:03:42 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/07 04:00:16 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;
	char	*temp;

	i = 0;
	temp = s;
	while (i < n)
	{
		temp[i] = (char)c;
		i++;
	}
	return (s);
}

/*int	main(void)
{
	char	buffer[20];
	memset(buffer, 'A', 5);
	buffer[4] = '\0';
	printf("%s\n", buffer);
	return (0);
}*/
