/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:40:28 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/22 19:18:14 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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

void draw(t_game *game)
{
	ft_memset(game->screen.addr, 0, game->screen.height * game->screen.line_len);

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
				draw_tile_to_screen(&game->screen, &game->collectible, px, py);
			else if (tile == 'E')
				draw_tile_to_screen(&game->screen, &game->exit, px, py);
		}
	}
	
	int ppx = game->player.x * TILE_SIZE;
	int ppy = game->player.y * TILE_SIZE;
	draw_tile_to_screen(&game->screen, &game->floor, ppx, ppy);
	draw_tile_to_screen(&game->screen, &game->player.img, ppx, ppy);
	mlx_put_image_to_window(game->mlx, game->win, game->screen.img_ptr, 0, 0);
}