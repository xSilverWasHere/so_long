/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:17:57 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/23 13:34:42 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_BONUS_H
# define SO_LONG_BONUS_H

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
# define MAX_ENEMIES 64

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

typedef struct s_enemy
{
	int		x;
	int		y;
	int		dir_x;
	int		dir_y;
	int		anim_index;
}	t_enemy;

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
	int				anim_index;
	int				idle_anim_timer;
	int				anim_counter;
	int				idle_index;
	int				is_moving;
	int				facing;
	int				x;
	int				y;
	t_img			idle[2];
	t_img			walk[2];
}	t_player;

typedef struct s_game
{
	void			*mlx;
	void			*win;
	t_img			screen;
	t_img			floor;
	t_img			wall;
	t_img			collectible[2];
	t_img			exit;
	t_player		player;
	t_img			enemy[2];
	t_enemy			*enemies;
	t_input_state	input;
	int				num_enemies;
	int				map_width;
	int				map_height;
	char			**grid;
	int				moves;
	int				collectible_anim_index;
	int				total_collectibles;
	int				collected;
	int				frame_count;
}	t_game;

// Functions
int		close_game(t_game *game);
int		key_press(int keycode, void *param);
int		key_release(int keycode, void *param);
int		handle_continuous_input(void *param);
void	init_screen_buffer(t_game *game);
void	init_game(t_game *game);
void	init_enemies(t_game *game);
void	draw(t_game *game);
void	draw_tile_to_screen(t_img *screen, t_img *tile,
			int x_offset, int y_offset);
void	draw_hud(t_game *game);
int		game_loop(void *param);
void	load_texture(void *mlx, t_img *img, char *path);
int		count_enemies(t_game *game);
void	update_enemies(t_game *game);
void	error_exit(const char *msg, t_game *game);
int		load_map(t_game *game, const char *filename);
void	free_map(t_game *game);
char	**copy_map_grid(char **grid, int height);
void	flood_fill(char **grid, int y, int x);
int		all_targets_reached(char **grid, int height);

#endif
