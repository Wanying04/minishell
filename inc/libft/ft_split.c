/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:33:14 by albarrei          #+#    #+#             */
/*   Updated: 2024/12/21 18:51:27 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_wordcount(const char *s, char c)
{
	size_t	i;
	size_t	count;
	size_t	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && !in_word)
		{
			count++;
			in_word = 1;
		}
		else if (s[i] == c)
			in_word = 0;
		i++;
	}
	return (count);
}

static size_t	ft_wordlen(const char *s, char c)
{
	int	len;

	len = 0;
	while (s[len] != '\0' && s[len] != c)
		len++;
	return (len);
}

static char	**ft_setfree(char **result, int i)
{
	while (i--)
		free(result[i]);
	free(result);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	size_t	i;
	size_t	words;
	size_t	len;
	char	**result;

	if (!s)
		return (NULL);
	words = ft_wordcount(s, c);
	result = (char **)malloc((words + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (i < words)
	{
		while (*s != '\0' && *s == c)
			s++;
		len = ft_wordlen(s, c);
		result[i] = ft_substr(s, 0, len);
		if (!result[i])
			return (ft_setfree(result, i));
		s += len;
		i++;
	}
	result[i] = NULL;
	return (result);
}

/*int	main(void)
{
	int	i = 0;
	char	**result = ft_split(",h,g,g,r", ',');
	while (result[i])
		printf("Substring: %s\n", result[i++]);
	return (0);
}*/
