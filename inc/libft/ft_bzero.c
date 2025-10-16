/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:43:15 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/07 09:57:01 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*temp;

	i = 0;
	temp = s;
	while (i < n)
	{
		temp[i] = '\0';
		i++;
	}
	return (s);
}

/*int   main(void)
{       
	char	buffer[20] = "holacaracola";
	ft_bzero(buffer, sizeof(buffer));
	buffer[4] = '\0';
	printf("%s\n", buffer);
	return (0);
}*/
