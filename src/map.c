/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 16:49:38 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/22 20:58:38 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	map_line_length(const char *line)
{
	int len = 0;
	while (line[len])
		len++;
	return len;
}

void error_exit(const char *msg, t_game *game)
{
	if (game)
		free_map(game);
	ft_putendl_fd("Error", 2);
	ft_putendl_fd((char *)msg, 2);
	exit(EXIT_FAILURE);
}

static void validate_chars(t_game *game, int *p_count, int *x_count, int *e_count, int *c_count)
{
	int y, x;
	char ch;

	for (y = 0; y < game->map_height; y++)
	{
		for (x = 0; x < game->map_width; x++)
		{
			ch = game->grid[y][x];
			if (ch == 'P')
			{
				(*p_count)++;
				game->player.x = x;
				game->player.y = y;
				
			}
			else if (ch == 'E')
				(*e_count)++;
			else if (ch == 'C')
				(*c_count)++;
			else if (ch == 'X')
				(*x_count)++;
			else if (ch != '1' && ch != '0')
				error_exit("Invalid character in map", game);
		}
	}
}

static void validate_map(t_game *game)
{
	int p = 0, x = 0, e = 0, c = 0;

	if (game->map_height == 0)
		error_exit("Map is empty", game);

	int width = game->map_width;
	for (int y = 0; y < game->map_height; y++)
	{
		if (map_line_length(game->grid[y]) != width)
			error_exit("Map is not rectangular", game);

		if (y == 0 || y == game->map_height - 1)
			for (int x = 0; x < width; x++)
				if (game->grid[y][x] != '1')
					error_exit("Map not enclosed (top/bottom)", game);

		if (game->grid[y][0] != '1' || game->grid[y][width - 1] != '1')
			error_exit("Map not enclosed (sides)", game);
	}

	validate_chars(game, &p, &x, &e, &c);

	char **grid_copy = copy_map_grid(game->grid, game->map_height);
	if (!grid_copy)
		error_exit("Failed to copy map for path validation", game);

	flood_fill(grid_copy, game->player.y, game->player.x);

	if (!all_targets_reached(grid_copy, game->map_height))
	{
		for (int i = 0; i < game->map_height; i++)
			free(grid_copy[i]);
		free(grid_copy);
		error_exit("Unreachable collectibles or exit", game);
	}

	for (int i = 0; i < game->map_height; i++)
		free(grid_copy[i]);
	free(grid_copy);

	game->total_collectibles = c;
	
	if (p != 1 || e != 1 || c < 1)
		error_exit("Map must have 1 'P', 1 'E', and at least 1 'C'", game);
}

int load_map(t_game *game, const char *filename)
{
	game->total_collectibles = 0;
	game->collected = 0;

	int fd = open(filename, O_RDONLY);
	if (fd < 0)
		error_exit("Could not open map file", game);

	char *line = NULL;
	size_t cap = 16;
	size_t count = 0;
	game->grid = malloc(sizeof(char *) * cap);
	if (!game->grid)
		error_exit("Memory allocation failed", game);

	while ((line = get_next_line(fd)))
	{
		size_t len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';

		if (count >= cap)
		{
			cap *= 2;
			char **tmp = malloc(sizeof(char *) * cap);
			if (!tmp)
				error_exit("Memory allocation failed", game);
			for (size_t i = 0; i < count; i++)
				tmp[i] = game->grid[i];
			free(game->grid);
			game->grid = tmp;
		}
		game->grid[count++] = line;
	}
	close(fd);

	game->map_height = (int)count;
	if (count > 0)
		game->map_width = map_line_length(game->grid[0]);
	else
		error_exit("Empty map file", game);

	validate_map(game);
	return (1);
}

void	free_map(t_game *game)
{
	if (!game->grid)
		return;
	for (int i = 0; i < game->map_height; i++)
		free(game->grid[i]);
	free(game->grid);
	game->grid = NULL;
}

char **copy_map_grid(char **grid, int height)
{
	char **copy = malloc(sizeof(char *) * (height + 1));
	if (!copy)
		return (NULL);

	for (int i = 0; i < height; i++)
		copy[i] = ft_strdup(grid[i]);
	copy[height] = NULL;
	return (copy);
}

void flood_fill(char **grid, int y, int x)
{
	if (grid[y][x] == '1' || grid[y][x] == 'V')
		return;

	grid[y][x] = 'V'; // Mark visited

	flood_fill(grid, y + 1, x);
	flood_fill(grid, y - 1, x);
	flood_fill(grid, y, x + 1);
	flood_fill(grid, y, x - 1);
}

int all_targets_reached(char **grid, int height)
{
	for (int y = 0; y < height; y++)
		for (int x = 0; grid[y][x]; x++)
			if (grid[y][x] == 'C' || grid[y][x] == 'E')
				return (0); // Unreachable
	return (1);
}