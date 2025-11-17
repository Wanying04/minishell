#include "minishell.h"

int	ft_isspecial(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == ';');
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || 
			c == '\v' || c == '\f' || c == '\r');
}
