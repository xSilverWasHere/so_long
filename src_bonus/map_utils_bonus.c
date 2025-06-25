/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 09:45:28 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 10:07:50 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	setup_enemy(t_game *game, int x, int y, int *x_count)
{
	game->enemies[*x_count].x = x;
	game->enemies[*x_count].y = y;
	game->enemies[*x_count].dir_x = 1;
	game->enemies[*x_count].dir_y = 0;
	(*x_count)++;
}

void	handle_char(t_game *game, t_counts *counts, t_charinfo info)
{
	if (info.ch == 'P')
	{
		counts->p++;
		game->player.x = info.x;
		game->player.y = info.y;
	}
	else if (info.ch == 'E')
		counts->e++;
	else if (info.ch == 'C')
		counts->c++;
	else if (info.ch == 'X')
		setup_enemy(game, info.x, info.y, &counts->x);
	else if (info.ch != '1' && info.ch != '0')
		error_exit("Invalid character in map", game);
}

char	**copy_map_grid(char **grid, int height)
{
	char	**copy;
	int		i;

	copy = malloc(sizeof(char *) * (height + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < height)
	{
		copy[i] = ft_strdup(grid[i]);
		i++;
	}
	copy[height] = NULL;
	return (copy);
}
