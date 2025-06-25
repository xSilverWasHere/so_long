/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:40:28 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 11:22:02 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	draw_tile_to_screen(t_img *screen, t_img *tile,
		int x_offset, int y_offset)
{
	int			y;
	int			x;
	t_drawpos	pos;

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
			pos.x = x;
			pos.y = y;
			pos.x_offset = x_offset;
			pos.y_offset = y_offset;
			copy_pixel_if_visible(screen, tile, pos);
			x++;
		}
		y++;
	}
}

void	draw(t_game *game)
{
	ft_memset(game->screen.addr, 0,
		game->screen.height * game->screen.line_len);
	render_map_tiles(game);
	render_player(game);
	mlx_put_image_to_window(game->mlx, game->win, game->screen.img_ptr, 0, 0);
}

void	flood_fill(char **grid, int y, int x)
{
	if (grid[y][x] == '1' || grid[y][x] == 'V')
		return ;
	grid[y][x] = 'V';
	flood_fill(grid, y + 1, x);
	flood_fill(grid, y - 1, x);
	flood_fill(grid, y, x + 1);
	flood_fill(grid, y, x - 1);
}
