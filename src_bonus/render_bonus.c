/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:40:28 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/22 19:23:17 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	draw_tile_to_screen(t_img *screen, t_img *tile, int x_offset, int y_offset)
{
	if (!screen->addr || !tile->addr)
	{
		ft_putendl_fd("ERROR: Null image address in draw_tile_to_screen", 2);
		exit(1);
	}
	
	for (int y = 0; y < tile->height; y++)
	{
		for (int x = 0; x < tile->width; x++)
		{
			int src_i = y * tile->line_len + x * (tile->bpp / 8);
			int dst_i = (y_offset + y) * screen->line_len + (x_offset + x) * (screen->bpp / 8);

			unsigned int pixel;
			ft_memcpy(&pixel, tile->addr + src_i, sizeof(unsigned int));
			if ((pixel >> 24) != 0xFF)
				ft_memcpy(screen->addr + dst_i, &pixel, sizeof(unsigned int));
		}
	}
}

void	draw_tile_to_screen_flipped(t_img *screen, t_img *tile, int x_offset, int y_offset)
{
	if (!screen->addr || !tile->addr)
	{
		ft_putendl_fd("ERROR: Null image address in draw_tile_to_screen_flipped", 2);
		exit(1);
	}

	for (int y = 0; y < tile->height; y++)
	{
		for (int x = 0; x < tile->width; x++)
		{
			int src_x = tile->width - 1 - x;
			int src_i = y * tile->line_len + src_x * (tile->bpp / 8);
			int dst_i = (y_offset + y) * screen->line_len + (x_offset + x) * (screen->bpp / 8);

			unsigned int pixel;
			ft_memcpy(&pixel, tile->addr + src_i, sizeof(unsigned int));
			if ((pixel >> 24) != 0xFF)
				ft_memcpy(screen->addr + dst_i, &pixel, sizeof(unsigned int));
		}
	}
}

void draw(t_game *game)
{
	ft_memset(game->screen.addr, 0, game->screen.height * game->screen.line_len);
	game->player.is_moving = 0;

	for (int y = 0; y < game->map_height; y++)
	{
		for (int x = 0; x < game->map_width; x++)
		{
			int px = x * TILE_SIZE;
			int py = y * TILE_SIZE;
			draw_tile_to_screen(&game->screen, &game->floor, px, py);

			char tile = game->grid[y][x];
			if (tile == '1')
				draw_tile_to_screen(&game->screen, &game->wall, px, py);
			else if (tile == 'C')
				draw_tile_to_screen(&game->screen, &game->collectible[game->collectible_anim_index % 2], px, py);
			else if (tile == 'E')
				draw_tile_to_screen(&game->screen, &game->exit, px, py);
		}
	}
	for (int i = 0; i < game->num_enemies; i++)
	{
		int ex = game->enemies[i].x * TILE_SIZE;
		int ey = game->enemies[i].y * TILE_SIZE;
		t_img *enemy_sprite = &game->enemy[game->enemies[i].anim_index];

		if (game->enemies[i].dir_x < 0)
			draw_tile_to_screen_flipped(&game->screen, enemy_sprite, ex, ey);
		else
			draw_tile_to_screen(&game->screen, enemy_sprite, ex, ey);
	}
	int ppx = game->player.x * TILE_SIZE;
	int ppy = game->player.y * TILE_SIZE;
	draw_tile_to_screen(&game->screen, &game->floor, ppx, ppy);

	t_img *player_sprite;
	if (game->input.key_up || game->input.key_down ||
		game->input.key_left || game->input.key_right)
		player_sprite = &game->player.walk[game->player.anim_index % 2];
	else
		player_sprite = &game->player.idle[game->player.anim_index % 2];

	if (game->player.facing == -1)
		draw_tile_to_screen_flipped(&game->screen, player_sprite, ppx, ppy);
	else
		draw_tile_to_screen(&game->screen, player_sprite, ppx, ppy);
	mlx_put_image_to_window(game->mlx, game->win, game->screen.img_ptr, 0, 0);
	draw_hud(game);
}

void draw_hud(t_game *game)
{
	char buffer[64];

	sprintf(buffer, "Moves: %d", game->moves);
	mlx_string_put(game->mlx, game->win, 10, 20, 0xFFFFFF, buffer);

	sprintf(buffer, "Collected: %d / %d", game->collected, game->total_collectibles);
	mlx_string_put(game->mlx, game->win, 10, 40, 0xFFFFFF, buffer);
}