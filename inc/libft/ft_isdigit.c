/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 18:21:32 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/06 21:59:53 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (2048);
	else
		return (0);
}

/*int	main(void)
{
	int result = ft_isdigit('A');
	printf("%i\n", result);
	result = ft_isdigit('a');
	printf("%i\n", result);
	result = ft_isdigit('3');
	printf("%i\n", result);
	return (0);
}*/
