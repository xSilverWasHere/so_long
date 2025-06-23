/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 16:49:38 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/23 14:10:14 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	map_line_length(const char *line)
{
	int	len;

	len = 0;
	while (line[len])
		len++;
	return (len);
}

void	error_exit(const char *msg, t_game *game)
{
	if (game)
		free_map(game);
	ft_putendl_fd("Error", 2);
	ft_putendl_fd((char *)msg, 2);
	exit(EXIT_FAILURE);
}

static void	validate_tile(t_game *game, t_tileinfo tile, t_counter *cnt)
{
	if (tile.ch == 'P')
	{
		(*cnt->p)++;
		game->player.x = tile.x;
		game->player.y = tile.y;
	}
	else if (tile.ch == 'E')
		(*cnt->e)++;
	else if (tile.ch == 'C')
		(*cnt->c)++;
	else if (tile.ch != '1' && tile.ch != '0')
		error_exit("Invalid character in map", game);
}

static void	validate_chars(t_game *game, int *p, int *e, int *c)
{
	int			y;
	int			x;
	t_counter	cnt;
	t_tileinfo	tile;

	cnt.p = p;
	cnt.e = e;
	cnt.c = c;
	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			tile.ch = game->grid[y][x];
			tile.x = x;
			tile.y = y;
			validate_tile(game, tile, &cnt);
			x++;
		}
		y++;
	}
}

static void	validate_map(t_game *game)
{
	int		p;
	int		e;
	int		c;
	int		width;
	int		y;
	int		a;
	char	**grid_copy;
	int		i;
	int		f;

	p = 0;
	e = 0;
	c = 0;
	if (game->map_height == 0)
		error_exit("Map is empty", game);
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
	validate_chars(game, &p, &e, &c);
	game->total_collectibles = c;
	if (p != 1 || e != 1 || c < 1)
		error_exit("Map must have 1 'P', 1 'E', and at least 1 'C'", game);
	grid_copy = copy_map_grid(game->grid, game->map_height);
	if (!grid_copy)
		error_exit("Failed to copy map for path validation", game);
	flood_fill(grid_copy, game->player.y, game->player.x);
	if (!all_targets_reached(grid_copy, game->map_height))
	{
		i = 0;
		while (i < game->map_height)
		{
			free(grid_copy[i]);
			i++;
		}
		free(grid_copy);
		error_exit("Unreachable collectibles or exit", game);
	}
	f = 0;
	while (f < game->map_height)
	{
		free(grid_copy[f]);
		f++;
	}
	free(grid_copy);
}

int	load_map(t_game *game, const char *filename)
{
	int		fd;
	char	*line;
	size_t	cap;
	size_t	count;
	size_t	len;
	char	**tmp;
	size_t	i;

	game->total_collectibles = 0;
	game->collected = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		error_exit("Could not open map file", game);
	line = NULL;
	cap = 16;
	count = 0;
	game->grid = malloc(sizeof(char *) * cap);
	if (!game->grid)
		error_exit("Memory allocation failed", game);
	line = get_next_line(fd);
	while (line != NULL)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (count >= cap)
		{
			cap *= 2;
			tmp = malloc(sizeof(char *) * cap);
			if (!tmp)
				error_exit("Memory allocation failed", game);
			i = 0;
			while (i < count)
			{
				tmp[i] = game->grid[i];
				i++;
			}
			free(game->grid);
			game->grid = tmp;
		}
		game->grid[count++] = line;
		line = get_next_line(fd);
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

void	flood_fill(char **grid, int y, int x)
{
	if (grid[y][x] == '1' || grid[y][x] == 'V')
		return ;
	grid[y][x] = 'V';
	flood_fill(grid, y + 1, x);
	flood_fill(grid, y - 1, x);
	flood_fill(grid, y, x + 1);
	flood_fill(grid, y, x - 1);
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
