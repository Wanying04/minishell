/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 12:25:00 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/09 19:44:24 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_converter(const char *nptr, int i, int number)
{
	if (ft_isdigit(nptr[i]))
	{
		while (ft_isdigit(nptr[i]))
		{
			number = 10 * number + (nptr[i] - '0');
			i++;
		}
		return (number);
	}
	return (0);
}

static int	ft_issign(const char *nptr, int i, int sign)
{
	int	counter;

	counter = 0;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1 * sign;
		i++;
		counter++;
	}
	if (counter > 1)
		return (0);
	else
		return (sign);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	number;
	int	sign;

	i = 0;
	number = 0;
	sign = 1;
	while (nptr[i] != '\0')
	{
		while (nptr[i] == '\f' || nptr[i] == '\n' || nptr[i] == '\r'
			|| nptr[i] == '\t' || nptr[i] == '\v' || nptr[i] == ' ')
			i++;
		if (nptr[i] == '+' || nptr[i] == '-')
		{
			sign = ft_issign(nptr, i, sign);
			i++;
		}
		if (ft_isdigit(nptr[i]))
			return ((ft_converter(nptr, i, number)) * sign);
		else
			return (0);
		i++;
	}
	return (0);
}

/*int	main(void)
{
	int	result = ft_atoi("   \t \v \n	\f \r- 28j2fdg34");
	printf("Result: %i\n", result);
	return (0);
}*/
