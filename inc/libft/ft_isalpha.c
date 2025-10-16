/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:49:13 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/07 09:52:48 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1024);
	else
		return (0);
}

/*int	main(void)
{
	int result = ft_isalpha('A');
	printf("%i\n", result);
	result = ft_isalpha('a');
	printf("%i\n", result);
	result = ft_isalpha('3');
	printf("%i\n", result);
	return (0);
}*/
