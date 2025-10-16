/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 20:32:31 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/21 16:59:26 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isset(const char *s1, const char *set)
{
	while (*set != '\0')
	{
		if (*s1 == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	const char	*end;
	const char	*start;
	char		*str;

	if (!s1 || !set)
		return (NULL);
	end = s1 + ft_strlen(s1) - 1;
	while (*s1 != '\0' && ft_isset(s1, set))
		s1++;
	start = s1;
	while (end > start && ft_isset(end, set))
		end--;
	str = ft_substr(start, 0, end - start + 1);
	return (str);
}

/*int     main(void)
{
        char    *res = ft_strtrim(" . . .  .  .hola a. a . . ... ", " .");
        printf("Result: %s\n", res);
        return (0);
}*/
