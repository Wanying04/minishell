/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 21:05:21 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/07 22:10:50 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*start;

	start = s;
	while (*s != '\0')
		s++;
	while (s >= start)
	{
		if (*s == (const char)c)
			return ((char *)s);
		s--;
	}
	return (NULL);
}

/*int	main(void)
{
	char	*s = "     ooooooohohohotomatehotomahtomhohoho";
	int	c = 'x';
	char	*result = ft_strrchr(s, c);

	printf("Result: %s\n", result);
	return (0);
}*/
