/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:55:46 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/05 20:25:08 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, int unsigned start, size_t len)
{
	char	*str;

	if (start > ft_strlen(s))
	{
		str = (char *)ft_calloc(1, 1);
		if (str == NULL)
			return (NULL);
		str[0] = '\0';
		return (str);
	}
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	str = (char *)ft_calloc(len + 1, 1);
	if (str == NULL)
		return (NULL);
	ft_memcpy(str, s + start, len);
	str[len] = '\0';
	return (str);
}

/*int	main(void)
{
	char	*str = ft_substr("lorem ipsu", '', 10);
	printf("Substring: %s", str);
	free(str);
	return (0);
}*/
