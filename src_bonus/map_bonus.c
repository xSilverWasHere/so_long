/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 16:49:38 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/22 19:23:17 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

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

int count_enemies(t_game *game)
{
    int count = 0;
    for (int y = 0; y < game->map_height; y++)
        for (int x = 0; x < game->map_width; x++)
            if (game->grid[y][x] == 'X')
                count++;
    return count;
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
			{
				game->enemies[*x_count].x = x;
				game->enemies[*x_count].y = y;
				game->enemies[*x_count].dir_x = 1;
				game->enemies[*x_count].dir_y = 0;
				(*x_count)++;
			}
			else if (ch != '1' && ch != '0')
				error_exit("Invalid character in map", game);
		}
	}
}

static void validate_map(t_game *game, int *num_enemies)
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

	game->total_collectibles = c;
	
	if (p != 1 || e != 1 || c < 1)
		error_exit("Map must have 1 'P', 1 'E', and at least 1 'C'", game);

	*num_enemies = x;
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
	game->num_enemies = count_enemies(game);
	game->enemies = malloc(sizeof(t_enemy) * game->num_enemies);
	if (!game->enemies)
		error_exit("Failed to allocate enemy array", game);

	int x_count = 0;
	validate_map(game, &x_count);
	game->num_enemies = x_count;

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