/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 19:24:17 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/06 21:45:11 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	else
		return (0);
}

/*int     main(void)
{
	int result = ft_isascii('0');
	printf("%i\n", result);
	result = ft_isascii(50);
	printf("%i\n", result);
	result = ft_isascii('a');
	printf("%i\n", result);
	result = ft_isascii('-');
	printf("%i\n", result);
	result = ft_isascii('\n');
	printf("%i\n", result);
	result = ft_isascii(164);
	printf("%i\n", result);
	result = ft_isascii(128);
	printf("%i\n", result);
	result = ft_isascii(-1);
	printf("%i\n", result);
	return (0);
}*/
