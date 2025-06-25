/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 10:27:41 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 11:21:08 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	draw_tile_at(t_game *game, int x, int y)
{
	int			px;
	int			py;
	char		tile;

	px = x * TILE_SIZE;
	py = y * TILE_SIZE;
	draw_tile_to_screen(&game->screen, &game->floor, px, py);
	tile = game->grid[y][x];
	if (tile == '1')
		draw_tile_to_screen(&game->screen, &game->wall, px, py);
	else if (tile == 'C')
		draw_tile_to_screen(&game->screen,
			&game->collectible, px, py);
	else if (tile == 'E')
		draw_tile_to_screen(&game->screen, &game->exit, px, py);
}
