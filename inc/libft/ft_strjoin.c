/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 23:16:11 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/12 14:52:46 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*str;

	str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (str == NULL)
		return (NULL);
	str[0] = '\0';
	ft_strlcat(str, s1, ft_strlen(s1) + ft_strlen(s2) + 1);
	ft_strlcat(str, s2, ft_strlen(s1) + ft_strlen(s2) + 1);
	str[ft_strlen(str)] = '\0';
	return (str);
}

/*int	main(void)
{
	char	*res = ft_strjoin("lorem ipsum", "dolor sit amet");
	printf("String final: %s\n", res);
	return (0);
}*/

/*int	main(void)
{
	char *s1 = "where is my ";
 	char *s2 = "malloc ???";
 	char *res = ft_strjoin(s1, s2);
	printf("String final: %s\n", res);
	return (0);
}*/

/*int	main(void)
{
	char *s1 = "my favorite animal is";
 	char *s2 = " ";
 	char *s3 = "the nyancat";
 	char *res = ft_strjoin(ft_strjoin(s1, s2), s3);
	printf("String final: %s\n", res);
	return (0);
}*/

/*int	main(void)
{
	char *s1 = "my favorite animal is";
 	char *s2 = " ";
 	char *s3 = "the nyancat";
 	char *tmp = ft_strjoin(s1, s2);
 	char *res = ft_strjoin(tmp, s3);
	printf("String final: %s\n", res);
	return (0);
}*/
