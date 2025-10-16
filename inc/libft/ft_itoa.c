/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 19:10:16 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/22 01:22:24 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_countnbr(int n)
{
	int	count;

	count = 0;
	while (n > 9)
	{
		count++;
		n = n / 10;
	}
	if (n <= 9)
		count++;
	return (count);
}

static char	*ft_isnegative(int n)
{
	char	*str;
	int		count;

	n = -n;
	count = ft_countnbr(n);
	str = malloc((count + 2) * sizeof(char));
	if (!str)
		return (NULL);
	str[count + 1] = '\0';
	while (n > 9)
	{
		str[count] = n % 10 + '0';
		n = n / 10;
		count--;
	}
	if (n <= 9)
	{
		str[count] = n % 10 + '0';
		str[0] = '-';
	}
	return (str);
}

static char	*ft_intmin(void)
{
	char	*str;

	str = malloc(12);
	if (!str)
		return (NULL);
	ft_memcpy(str, "-2147483648", 12);
	return (str);
}

char	*ft_itoa(int n)
{
	int		count;
	char	*str;

	if (n == -2147483648)
		return (ft_intmin());
	if (n < 0)
		return (ft_isnegative(n));
	count = ft_countnbr(n);
	str = malloc((count + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[count] = '\0';
	while (n > 9)
	{
		str[count - 1] = n % 10 + '0';
		n = n / 10;
		count--;
	}
	if (n <= 9)
		str[0] = n % 10 + '0';
	return (str);
}

/*int	main(void)
{
	int	n = -2147483648;
	printf("Number: %s\n", ft_itoa(n));
	return (0);
}*/
