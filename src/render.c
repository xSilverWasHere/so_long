/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:40:28 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/23 13:40:11 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	draw_tile_to_screen(t_img *screen, t_img *tile,
			int x_offset, int y_offset)
{
	int				y;
	int				x;
	int				src_i;
	int				dst_i;
	unsigned int	pixel;

	if (!screen->addr || !tile->addr)
	{
		ft_putendl_fd("ERROR: Null image address", 2);
		exit(1);
	}
	y = 0;
	while (y < tile->height)
	{
		x = 0;
		while (x < tile->width)
		{
			src_i = y * tile->line_len + x * (tile->bpp / 8);
			dst_i = (y_offset + y) * screen->line_len + (x_offset + x)
				* (screen->bpp / 8);
			ft_memcpy(&pixel, tile->addr + src_i, sizeof(unsigned int));
			if ((pixel >> 24) != 0xFF)
				ft_memcpy(screen->addr + dst_i, &pixel, sizeof(unsigned int));
			x++;
		}
		y++;
	}
}

void	draw(t_game *game)
{
	int		y;
	int		x;
	int		px;
	int		py;
	int		ppx;
	int		ppy;
	char	tile;

	ft_memset(game->screen.addr, 0, game->screen.height
		* game->screen.line_len);
	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			px = x * TILE_SIZE;
			py = y * TILE_SIZE;
			draw_tile_to_screen(&game->screen, &game->floor, px, py);
			tile = game->grid[y][x];
			if (tile == '1')
				draw_tile_to_screen(&game->screen, &game->wall, px, py);
			else if (tile == 'C')
				draw_tile_to_screen(&game->screen,
					&game->collectible,
					px, py);
			else if (tile == 'E')
				draw_tile_to_screen(&game->screen, &game->exit, px, py);
			x++;
		}
		y++;
	}
	ppx = game->player.x * TILE_SIZE;
	ppy = game->player.y * TILE_SIZE;
	draw_tile_to_screen(&game->screen, &game->floor, ppx, ppy);
	draw_tile_to_screen(&game->screen, &game->player.img, ppx, ppy);
	mlx_put_image_to_window(game->mlx, game->win, game->screen.img_ptr, 0, 0);
}
