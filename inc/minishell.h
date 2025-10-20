#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <ctype.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/history.h>
# include <readline/readline.h>

# include "parser.h"
//# include "executor.h"

// ========================= CONSTANTS =========================

# ifndef TILE_SIZE
#  define TILE_SIZE 128
# endif/*

// ========================= KEYBOARD INPUT =========================

# define KEY_W     87
# define KEY_A     65
# define KEY_S     83
# define KEY_D     68
# define KEY_ESC   256

// ========================= ENUMS =========================

typedef enum e_tile
{
	EMPTY = '0',
	WALL = '1',
	COLLECTIBLE = 'C',
	EXIT = 'E',
	PLAYER = 'P'
}		t_tile;

typedef enum e_direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
}		t_direction;

// ========================= STRUCTS =========================

// Position in the map
typedef struct s_pos
{
	int	x;
	int	y;
}		t_pos;

// Represent the map
typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
	int		collectibles;
	int		exits;
	int		players;
}		t_map;

// Assets pointers for each type of tile
typedef struct s_assets
{
	mlx_texture_t	*tx_wall;
	mlx_texture_t	*tx_floor;
	mlx_texture_t	*tx_collectible;
	mlx_texture_t	*tx_exit;
	mlx_texture_t	*tx_player_up;
	mlx_texture_t	*tx_player_down;
	mlx_texture_t	*tx_player_left;
	mlx_texture_t	*tx_player_right;
	mlx_image_t		*wall;
	mlx_image_t		*floor;
	mlx_image_t		*collectible;
	mlx_image_t		*exit;
	mlx_image_t		*player_up;
	mlx_image_t		*player_down;
	mlx_image_t		*player_left;
	mlx_image_t		*player_right;
}		t_assets;

// Represent the player
typedef struct s_player
{
	t_pos		pos;
	int			moves;
	int			collected;
	t_direction	dir;
}		t_player;

// Represent the game status
typedef struct s_game
{
	mlx_t		*mlx;
	t_map		map;
	t_player	player;
	t_assets	assets;
}		t_game;*/

// ========================= FUNCTIONS =========================

#endif
