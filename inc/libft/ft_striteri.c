/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:53:43 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/22 14:59:35 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int		i;

	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}

/*static void	ft_idk1(unsigned int i, char *s)
{
	if (s[i] == '\0')
		return;
	if (s[i] % 2 == 0)
		s[i] = 'x';
}

int	main(void)
{
	char	res[] = "hola";

	ft_striteri(res, ft_idk1);
	printf("Result: %s\n", res);
	return (0);
}*/
