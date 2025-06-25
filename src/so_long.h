/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:17:57 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 11:21:36 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "../lib/mlx/mlx.h"
# include "../lib/libft/libft.h"
# include <X11/keysym.h>
# include <X11/X.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>

# define KEY_ESC 65307
# define KEY_UP 65362
# define KEY_DOWN 65364
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define TILE_SIZE 64

typedef struct s_img
{
	void	*img_ptr;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}	t_map;

typedef struct s_input_state
{
	int		key_up;
	int		key_down;
	int		key_left;
	int		key_right;
	long	last_input_time;
	long	key_press_time;
	int		key_held;
	int		initial_delay;
	int		cooldown;
}	t_input_state;

typedef struct s_player
{
	int				x;
	int				y;
	t_img			img;
}	t_player;

typedef struct s_counts
{
	int	p;
	int	e;
	int	c;
}	t_counts;

typedef struct s_charinfo
{
	char	ch;
	int		x;
	int		y;
}	t_charinfo;

typedef struct s_drawpos
{
	int	x;
	int	y;
	int	x_offset;
	int	y_offset;
}	t_drawpos;

typedef struct s_game
{
	void			*mlx;
	void			*win;
	t_img			screen;
	t_img			floor;
	t_img			wall;
	t_img			collectible;
	t_img			exit;
	t_player		player;
	t_input_state	input;
	int				map_width;
	int				map_height;
	char			**grid;
	int				moves;
	int				total_collectibles;
	int				collected;
}	t_game;

// Functions

// events
void	move_player(t_game *game, int dx, int dy);
int		key_press(int keycode, void *param);
int		key_release(int keycode, void *param);
int		handle_continuous_input(void *param);
int		close_game(t_game *game);

//init
void	load_texture(void *mlx, t_img *img, char *path);
void	init_screen_buffer(t_game *game);
void	init_game(t_game *game);

//main
int		game_loop(void *param);

//map
int		map_line_length(char *line);
void	validate_chars(t_game *game, t_counts *counts);
void	validate_map(t_game *game);
void	handle_char(t_game *game, t_counts *counts, t_charinfo info);
char	**copy_map_grid(char **grid, int height);
void	free_map(t_game *game);
int		load_map(t_game *game, const char *filename);
void	error_exit(const char *msg, t_game *game);
char	*trim_newline(char *line);
char	**realloc_grid(char **old, size_t old_cap, size_t new_cap,
			t_game *game);
int		all_targets_reached(char **grid, int height);

//move_utils
void	process_input(t_game *game, int x, int y);
int		check_collision(t_game *game, int x, int y);
void	handle_collectible(t_game *game, int x, int y);
int		handle_exit(t_game *game, int x, int y);

//read_utils
void	check_pathfinding(t_game *game);
char	**read_map_lines(int fd, t_game *game, size_t *line_count);
void	check_rectangular_and_walls(t_game *game);

//render
void	draw(t_game *game);
void	draw_tile_to_screen(t_img *screen, t_img *tile,
			int x_offset, int y_offset);
void	flood_fill(char **grid, int y, int x);
void	copy_pixel_if_visible(t_img *screen, t_img *tile, t_drawpos pos);
void	render_map_tiles(t_game *game);
void	render_player(t_game *game);
void	draw_tile_at(t_game *game, int x, int y);

#endif
