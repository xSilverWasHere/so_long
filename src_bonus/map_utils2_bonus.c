/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils2_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 10:08:55 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 10:21:27 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	free_map(t_game *game)
{
	int	i;

	if (!game->grid)
		return ;
	i = 0;
	while (i < game->map_height)
	{
		free(game->grid[i]);
		i++;
	}
	free(game->grid);
	game->grid = NULL;
}

int	load_map(t_game *game, const char *filename)
{
	int		fd;
	size_t	count;
	int		x_count;

	game->total_collectibles = 0;
	game->collected = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		error_exit("Could not open map file", game);
	game->grid = read_map_lines(fd, game, &count);
	close(fd);
	if (count == 0)
		error_exit("Empty map file", game);
	game->map_height = (int)count;
	game->map_width = map_line_length(game->grid[0]);
	game->num_enemies = count_enemies(game);
	game->enemies = malloc(sizeof(t_enemy) * game->num_enemies);
	if (!game->enemies)
		error_exit("Failed to allocate enemy array", game);
	x_count = 0;
	validate_map(game, &x_count);
	game->num_enemies = x_count;
	return (1);
}

void	error_exit(const char *msg, t_game *game)
{
	if (game)
		free_map(game);
	ft_putendl_fd("Error", 2);
	ft_putendl_fd((char *)msg, 2);
	exit(EXIT_FAILURE);
}

char	*trim_newline(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

char	**realloc_grid(char **old, size_t old_cap, size_t new_cap, t_game *game)
{
	char	**new;
	size_t	i;

	new = malloc(sizeof(char *) * new_cap);
	if (!new)
		error_exit("Memory allocation failed", game);
	i = 0;
	while (i < old_cap)
	{
		new[i] = old[i];
		i++;
	}
	free(old);
	return (new);
}
