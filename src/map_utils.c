/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 09:45:28 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 11:20:21 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

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
