/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 10:15:11 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 10:28:05 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

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

void	copy_flipped_pixel(t_img *screen, t_img *tile, t_drawpos pos)
{
	int				src_x;
	int				src_i;
	int				dst_i;
	unsigned int	pixel;

	src_x = tile->width - 1 - pos.x;
	src_i = pos.y * tile->line_len + src_x * (tile->bpp / 8);
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

void	render_enemies(t_game *game)
{
	int		i;
	int		ex;
	int		ey;
	t_img	*enemy_sprite;

	i = 0;
	while (i < game->num_enemies)
	{
		ex = game->enemies[i].x * TILE_SIZE;
		ey = game->enemies[i].y * TILE_SIZE;
		enemy_sprite = &game->enemy[game->enemies[i].anim_index];
		if (game->enemies[i].dir_x < 0)
			draw_tile_to_screen_flipped(&game->screen, enemy_sprite, ex, ey);
		else
			draw_tile_to_screen(&game->screen, enemy_sprite, ex, ey);
		i++;
	}
}

void	render_player(t_game *game)
{
	int		ppx;
	int		ppy;
	t_img	*player_sprite;

	ppx = game->player.x * TILE_SIZE;
	ppy = game->player.y * TILE_SIZE;
	draw_tile_to_screen(&game->screen, &game->floor, ppx, ppy);
	if (game->input.key_up || game->input.key_down
		|| game->input.key_left || game->input.key_right)
		player_sprite = &game->player.walk[game->player.anim_index % 2];
	else
		player_sprite = &game->player.idle[game->player.anim_index % 2];
	if (game->player.facing == -1)
		draw_tile_to_screen_flipped(&game->screen, player_sprite, ppx, ppy);
	else
		draw_tile_to_screen(&game->screen, player_sprite, ppx, ppy);
}
