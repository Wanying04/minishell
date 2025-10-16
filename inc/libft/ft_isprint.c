/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 21:11:13 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/06 22:01:31 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (16384);
	else
		return (0);
}

/*int     main(void)
{
	int result = ft_isprint('0');
	printf("%i\n", result);
	result = ft_isprint(50);
	printf("%i\n", result);
	result = ft_isprint(' ');
	printf("%i\n", result);
	result = ft_isprint('-');
	printf("%i\n", result);
	result = ft_isprint('\n');
	printf("%i\n", result);
	result = ft_isprint(164);
	printf("%i\n", result);
	result = ft_isprint('@');
	printf("%i\n", result);
	result = ft_isprint(-1);
	printf("%i\n", result);
	return (0);
}*/
