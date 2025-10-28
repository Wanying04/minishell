#ifndef PARSER_H
# define PARSER_H

/* Basic verification functions */
int		ft_isspace(char c);
int		ft_isspecial(char c);

/* Quote handling functions */
int		ft_get_quoted_size(char *input, int *i);
char	*ft_get_quoted_token(char *input, int *i);
int		ft_skip_quoted_section(char *input, int *i);

/* Token extraction functions */
char	*ft_get_special_token(char *input, int *i);
char	*ft_get_normal_token(char *input, int *i);
int		ft_count_tokens(char *input);
char	**ft_split_tokens(char *input);

/* Error handling functions */
int		ft_check_quotes(char *input);        // Checks for unclosed quotes
int		ft_check_escape_chars(char *input);  // Validates escape sequences
void	ft_free_tokens(char **tokens);       // Frees token array memory

#endif