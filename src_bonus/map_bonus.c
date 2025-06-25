/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 16:49:38 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 10:11:58 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

int	map_line_length(char *line)
{
	int	len;

	len = 0;
	while (line[len])
		len++;
	return (len);
}

int	count_enemies(t_game *game)
{
	int	count;
	int	y;
	int	x;

	count = 0;
	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			if (game->grid[y][x] == 'X')
				count++;
			x++;
		}
		y++;
	}
	return (count);
}

void	validate_chars(t_game *game, t_counts *counts)
{
	int			y;
	int			x;
	char		ch;
	t_charinfo	info;

	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			ch = game->grid[y][x];
			info.ch = ch;
			info.x = x;
			info.y = y;
			handle_char(game, counts, info);
			x++;
		}
		y++;
	}
}

void	validate_map(t_game *game, int *num_enemies)
{
	t_counts	counts;

	if (game->map_height == 0)
		error_exit("Map is empty", game);
	check_rectangular_and_walls(game);
	counts.p = 0;
	counts.e = 0;
	counts.c = 0;
	counts.x = 0;
	validate_chars(game, &counts);
	if (counts.p != 1 || counts.e != 1 || counts.c < 1)
		error_exit("Map must have 1 'P', 1 'E', and at least 1 'C'", game);
	game->total_collectibles = counts.c;
	check_pathfinding(game);
	*num_enemies = counts.x;
}

int	all_targets_reached(char **grid, int height)
{
	int	y;
	int	x;

	y = 0;
	x = 0;
	while (y < height)
	{
		x = 0;
		while (grid[y][x])
		{
			if (grid[y][x] == 'C' || grid[y][x] == 'E')
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}
