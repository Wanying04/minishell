/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 09:26:37 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/08 11:31:31 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_match(const char *big, const char *little, size_t i, size_t len)
{
	size_t	j;

	j = 0;
	while (i < len && big[i] == little[j] && little[j] != '\0')
	{
		i++;
		j++;
	}
	return (little[j] == '\0');
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	if (!*little)
		return ((char *)big);
	i = 0;
	while (i < len && big[i] != '\0')
	{
		if (big[i] == little[0] && ft_match(big, little, i, len))
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}

/*int	main(void)
{
	char	*str = ft_strnstr("holo manolA qué tal", "ola", 40);
	
	printf("Result: %s", str);
	return (0);
}*/
