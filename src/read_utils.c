/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 09:59:56 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 11:07:59 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	check_rectangular_and_walls(t_game *game)
{
	int	y;
	int	a;
	int	width;

	width = game->map_width;
	y = 0;
	while (y < game->map_height)
	{
		if (map_line_length(game->grid[y]) != width)
			error_exit("Map is not rectangular", game);
		if (y == 0 || y == game->map_height - 1)
		{
			a = 0;
			while (a < width)
			{
				if (game->grid[y][a] != '1')
					error_exit("Map not enclosed (top/bottom)", game);
				a++;
			}
		}
		if (game->grid[y][0] != '1' || game->grid[y][width - 1] != '1')
			error_exit("Map not enclosed (sides)", game);
		y++;
	}
}

char	**read_map_lines(int fd, t_game *game, size_t *line_count)
{
	char	**grid;
	char	*line;
	size_t	cap;
	size_t	count;

	cap = 16;
	count = 0;
	grid = malloc(sizeof(char *) * cap);
	if (!grid)
		error_exit("Memory allocation failed", game);
	line = get_next_line(fd);
	while (line)
	{
		line = trim_newline(line);
		if (count >= cap)
		{
			cap *= 2;
			grid = realloc_grid(grid, count, cap, game);
		}
		grid[count++] = line;
		line = get_next_line(fd);
	}
	*line_count = count;
	return (grid);
}

void	check_pathfinding(t_game *game)
{
	char	**copy;
	int		i;

	copy = copy_map_grid(game->grid, game->map_height);
	if (!copy)
		error_exit("Failed to copy map for path validation", game);
	flood_fill(copy, game->player.y, game->player.x);
	if (!all_targets_reached(copy, game->map_height))
	{
		i = 0;
		while (i < game->map_height)
			free(copy[i++]);
		free(copy);
		error_exit("Unreachable collectibles or exit", game);
	}
	i = 0;
	while (i < game->map_height)
		free(copy[i++]);
	free(copy);
}
