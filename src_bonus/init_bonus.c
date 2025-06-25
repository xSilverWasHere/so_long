/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:40:32 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 09:42:15 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	load_texture(void *mlx, t_img *img, char *path)
{
	img->img_ptr = mlx_xpm_file_to_image(mlx, path, &img->width, &img->height);
	if (!img->img_ptr)
	{
		ft_putstr_fd("ERROR: Failed to load texture: ", 2);
		ft_putendl_fd(path, 2);
		exit(EXIT_FAILURE);
	}
	img->addr = mlx_get_data_addr(img->img_ptr, &img->bpp,
			&img->line_len, &img->endian);
	if (!img->addr)
	{
		ft_putstr_fd("ERROR: Null image address in draw_tile_to_screen\n", 2);
		exit(EXIT_FAILURE);
	}
}

void	init_screen_buffer(t_game *game)
{
	int	width;
	int	height;

	width = game->map_width * 64;
	height = game->map_height * 64;
	game->screen.img_ptr = mlx_new_image(game->mlx, width, height);
	if (!game->screen.img_ptr)
	{
		ft_putendl_fd("ERROR: Failed to create screen buffer.", 2);
		exit(EXIT_FAILURE);
	}
	game->screen.addr = mlx_get_data_addr(game->screen.img_ptr,
			&game->screen.bpp, &game->screen.line_len, &game->screen.endian);
	if (!game->screen.addr)
	{
		ft_putendl_fd("ERROR: Failed to get screen buffer address.", 2);
		exit(EXIT_FAILURE);
	}
	game->screen.width = width;
	game->screen.height = height;
}

void	init_enemy_at(t_game *game, int x, int y, int *count)
{
	game->enemies[*count].x = x;
	game->enemies[*count].y = y;
	game->enemies[*count].dir_x = 1;
	game->enemies[*count].dir_y = 0;
	(*count)++;
}

void	init_enemies(t_game *game)
{
	int	count;
	int	x;
	int	y;

	count = 0;
	game->enemies = malloc(sizeof(t_enemy) * game->num_enemies);
	if (!game->enemies)
		error_exit("Memory allocation failed", game);
	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			if (game->grid[y][x] == 'X')
				init_enemy_at(game, x, y, &count);
			x++;
		}
		y++;
	}
}

void	init_game(t_game *game)
{
	game->frame_count = 0;
	game->mlx = mlx_init();
	if (!game->mlx)
		exit(EXIT_FAILURE);
	init_screen_buffer(game);
	game->win = mlx_new_window(game->mlx,
			game->map_width * 64, game->map_height * 64, "so_long");
	if (!game->win)
		exit(EXIT_FAILURE);
	load_texture(game->mlx, &game->floor, "assets/floor.xpm");
	load_texture(game->mlx, &game->wall, "assets/wall.xpm");
	load_texture(game->mlx, &game->exit, "assets/exit.xpm");
	load_texture(game->mlx, &game->collectible[0], "assets/collectible1.xpm");
	load_texture(game->mlx, &game->collectible[1], "assets/collectible2.xpm");
	load_texture(game->mlx, &game->enemy[0], "assets/enemy2.xpm");
	load_texture(game->mlx, &game->enemy[1], "assets/enemy1.xpm");
	load_texture(game->mlx, &game->player.walk[0], "assets/player_walking.xpm");
	load_texture(game->mlx,
		&game->player.walk[1], "assets/player_walking2.xpm");
	load_texture(game->mlx, &game->player.idle[0], "assets/player_idle1.xpm");
	load_texture(game->mlx, &game->player.idle[1], "assets/player_idle2.xpm");
	game->player.anim_index = 0;
	game->player.idle_index = 0;
	game->moves = 0;
}
