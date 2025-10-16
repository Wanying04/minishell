/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 22:41:36 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/09 23:18:36 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if ((!*s1 && !*s2) || n == 0)
		return (0);
	while (*s1 == *s2 && i < n - 1 && *s1 != '\0' && *s2 != '\0')
	{
		s1++;
		s2++;
		i++;
	}
	if (*s1 == *s2)
		return (0);
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/*int	main(void)
{
	const char	*s1 = "aaaaaaaaaaaaaaaaaaaaaaaaholaaaaaaaaaaa";
	const char	*s2 = "aaaaaaaaaaaaaaaaaaaaaaaaholbaaaaaaaaaa";
	size_t		n = 1000;
	int		result = ft_strncmp(s1, s2, n);
	printf("Result: %i\n", result);
	return (0);
}*/
