/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 19:04:04 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/06 21:40:28 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (8);
	if (c >= '0' && c <= '9')
		return (8);
	else
		return (0);
}

/*int	main(void)
{
	int result = ft_isalnum('A');
	printf("%i\n", result);
	result = ft_isalnum('a');
	printf("%i\n", result);
	result = ft_isalnum('3');
	printf("%i\n", result);
	result = ft_isalnum('-');
	printf("%i\n", result);
	return (0);
}*/
