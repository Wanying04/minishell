#include "minishell.h"

// Function to expand ~
char *expand_tilde(char *path, t_env *env)
{
	char	*home;
	char	*expanded;
	size_t	len;

	if (!path || path[0] != '~')
		return (path);
	home = env_get(env, "HOME");
	if (!home)
		return (path);
	if (path[1] == '\0')
		return (ft_strdup(home));
	if (path[1] == '/')
	{
		len = ft_strlen(home) + ft_strlen(path + 1) + 1;
		expanded = malloc(len);
		if (!expanded)
			return (NULL);
		ft_strlcpy(expanded, home, len);
		ft_strlcat(expanded, path + 1, len);
		return (expanded);
	}
	return (path);
}
