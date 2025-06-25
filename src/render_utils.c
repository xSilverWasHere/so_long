/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 10:15:11 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 11:21:11 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	copy_pixel_if_visible(t_img *screen, t_img *tile, t_drawpos pos)
{
	int				src_i;
	int				dst_i;
	unsigned int	pixel;

	src_i = pos.y * tile->line_len + pos.x * (tile->bpp / 8);
	dst_i = (pos.y_offset + pos.y) * screen->line_len
		+ (pos.x_offset + pos.x) * (screen->bpp / 8);
	ft_memcpy(&pixel, tile->addr + src_i, sizeof(unsigned int));
	if ((pixel >> 24) != 0xFF)
		ft_memcpy(screen->addr + dst_i, &pixel, sizeof(unsigned int));
}

void	render_map_tiles(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			draw_tile_at(game, x, y);
			x++;
		}
		y++;
	}
}

void	render_player(t_game *game)
{
	int		ppx;
	int		ppy;

	ppx = game->player.x * TILE_SIZE;
	ppy = game->player.y * TILE_SIZE;
	draw_tile_to_screen(&game->screen, &game->floor, ppx, ppy);
	draw_tile_to_screen(&game->screen, &game->player.img, ppx, ppy);
}
