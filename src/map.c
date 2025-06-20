/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:34:04 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/20 21:07:07 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	validate_map(char **map, int *width, int height)
{
	if (!map || height <= 0)
		return (0);
	*width = ft_strlen(map[0]);
	for (int y = 0; y < height; y++)
	{
		if ((int)ft_strlen(map[y]) != *width)
			return (0);
	}
	return (1);
}
static int	count_lines(const char *filename)
{
	int		fd;
	int		count = 0;
	char	buffer[1];
	int		read_result;
	char	prev = '\n';

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		error_exit("Failed to open map file");

	while ((read_result = read(fd, buffer, 1)) > 0)
	{
		if (buffer[0] == '\n' && prev != '\n')
			count++;
		prev = buffer[0];
	}
	if (prev != '\n')
		count++;
	close(fd);
	return (count);
}

static char	*read_line(int fd)
{
	char	*line = NULL;
	char	buffer[1];
	int		read_result;
	int		i = 0;
	int		capacity = 128;

	line = malloc(capacity);
	if (!line)
		return (NULL);

	while ((read_result = read(fd, buffer, 1)) > 0 && buffer[0] != '\n')
	{
		if (i >= capacity - 1)
		{
			capacity *= 2;
			char *new_line = malloc(capacity);
			if (!new_line)
				return (NULL);
			for (int j = 0; j < i; j++)
				new_line[j] = line[j];
			free(line);
			line = new_line;
		}
		line[i++] = buffer[0];
	}
	if (read_result == 0 && i == 0)
	{
		free(line);
		return (NULL);
	}
	line[i] = '\0';
	return (line);
}

void find_and_place_player(t_game *game)
{
	for (int y = 0; y < game->height; y++)
	{
		for (int x = 0; x < game->width; x++)
		{
			if (game->map[y][x] == 'P')
			{
				game->vars.player_x = x;
				game->vars.player_y = y;
				return;
			}
		}
	}
	error_exit("Player start position not found.");
}

void find_and_place_enemies(t_game *game)
{
	for (int y = 0; y < game->height; y++)
	{
		for (int x = 0; x < game->width; x++)
		{
			if (game->map[y][x] == 'X')
			{
				t_enemy *new_enemy = malloc(sizeof(t_enemy));
				if (!new_enemy)
					error_exit("Failed to allocate enemy.");
				new_enemy->x = x;
				new_enemy->y = y;
				new_enemy->dir_x = 1;
				new_enemy->dir_y = 0;
				new_enemy->anim = game->vars.enemy_anim;
				new_enemy->next = game->enemies;
				game->enemies = new_enemy;
			}
		}
	}
}

char **read_map_file(const char *path, int *height)
{
	int		fd;
	int		lines;
	int		i;
	char	**map;

	lines = count_lines(path);
	if (lines == 0)
		error_exit("Empty map file");

	fd = open(path, O_RDONLY);
	if (fd < 0)
		error_exit("Could not reopen map file");

	map = malloc((lines + 1) * sizeof(char *));
	if (!map)
		error_exit("Memory allocation failed");

	i = 0;
	while (i < lines)
	{
		map[i] = read_line(fd);
		if (!map[i])
		{
			free_map(map);
			error_exit("Error reading map line");
		}
		char *newline = ft_strchr(map[i], '\n');
		if (newline)
			*newline = '\0';
		i++;
	}
	map[i] = NULL;
	*height = lines;
	close(fd);
	return (map);
}