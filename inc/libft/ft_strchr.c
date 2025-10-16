/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 20:37:40 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/09 20:05:30 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while ((*s != (const char)c) && *s != '\0')
		s++;
	if (*s == (char)c)
		return ((char *)s);
	else
		return (NULL);
}

/*int	main(void)
{
	char	*s = "     ooooooohohohotomatehotomahtomhohoho";
	int	c = 't';
	char	*result = ft_strchr(s, c);

	printf("Result: %s\n", result);
	return (0);
}*/
