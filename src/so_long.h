/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:17:57 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/20 20:58:48 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SO_LONG_H
#define SO_LONG_H
# define TILE_SIZE 64
# define WIDTH 640
# define HEIGHT 480

#include "../lib/mlx/mlx.h"
#include "../lib/libft/libft.h"
#include <../X11/keysym.h>
#include <../X11/X.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_img
{
	void	*img_ptr;
	char	*img_pixels_ptr;
	int		bits_per_pixel;
	int		endian;
	int		width;
	int		height;
	int		line_len;
}				t_img;

typedef struct s_anim 
{
	t_img frames[2];
	int current_frame;
	int frame_count;
	int frame_delay;
	int frame_timer;
}	t_anim;

typedef struct	s_var
{
	void	*mlx;
	void	*win;
	t_img	screen;
	t_img	floor;
	t_img	wall;
	t_img	collectible;
	t_img	exit;
	t_img	enemy;
	t_img	player;
	t_img	player_dead;
	int		player_x;
	int		player_y;
	int		facing_right;
	t_anim	player_anim;
	t_anim	collectible_anim;
	t_anim	enemy_anim;

}	t_var;

typedef struct s_enemy 
{
	int x, y;
	int dir_x, dir_y;
	t_anim anim;
	struct s_enemy *next;
}				t_enemy;

typedef struct s_game
{
	t_var	vars;
	t_enemy *enemies;
	int 	frame_counter;
	char	**map;
	int		width;
	int		height;
	int		collectibles;
	int		total_collectibles;
	int		collected;
	int		player_moves;
	int 	is_dead;
	int		death_anim_done;
}				t_game;


// utils.c
void	error_exit(const char *msg);
void	free_map(char **map);
int		count_collectibles(char **map, int height);
void	close_game(t_game *game);
void	add_enemy(t_game *game, int x, int y);
int		get_pixel(t_img *src, int x, int y);
void	set_pixel(t_img *dst, int x, int y, int color);
void	draw_moves(t_game *game);

// main.c
int game_loop(void *param);

// map.c
char	**read_map_file(const char *path, int *height);
int		validate_map(char **map, int *width, int height);
void	find_and_place_player(t_game *game);
void	find_and_place_enemies(t_game *game);

// render.c
void	load_texture(t_img *img, void *mlx, char *path);
void	render_map(t_game *game);
void	draw_player(t_game *game);
void	draw_sprite(t_img *dst, t_img *src, int x, int y);
void	draw_sprite_flipped(t_img *dst, t_img *src, int x, int y);
void	update_enemies(t_game *game);
void 	update_animation(t_anim *anim);
void	load_player_anim(t_anim *anim, void *mlx);
void	load_collectible_anim(t_anim *anim, void *mlx);
void	load_enemy_anim(t_anim *anim, void *mlx);
void	load_player_death_anim(t_anim *anim, void *mlx);
void	render_enemies(t_game *game);

// player.c
int		handle_input(int keycode, t_game *game);

#endif