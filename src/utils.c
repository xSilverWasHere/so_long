/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:15:34 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/20 20:28:57 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int get_pixel(t_img *img, int x, int y)
{
	char *pixel = img->img_pixels_ptr + (y * img->line_len + x * (img->bits_per_pixel / 8));
	return *(int *)pixel;
}

void set_pixel(t_img *img, int x, int y, int color)
{
	char *pixel = img->img_pixels_ptr + (y * img->line_len + x * (img->bits_per_pixel / 8));
	*(int *)pixel = color;
}

void	error_exit(const char *msg)
{
	fprintf(stderr, "Error\n%s\n", msg);
	exit(EXIT_FAILURE);
}

void	free_map(char **map)
{
	int	i = 0;

	if (!map)
		return;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}

int count_collectibles(char **map, int height)
{
	int count = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; map[y][x]; x++)
		{
			if (map[y][x] == 'C')
				count++;
		}
	}
	return count;
}

void draw_moves(t_game *game)
{
	char *move_str;
	char *label;

	label = ft_strdup("Moves: ");
	move_str = ft_itoa(game->player_moves);

	char *text = ft_strjoin(label, move_str);
	free(label);
	free(move_str);

	mlx_string_put(game->vars.mlx, game->vars.win, 10, 10, 0xFFFFFF, text);
	free(text);
}

static void destroy_texture(t_img *img, void *mlx)
{
	if (img && img->img_ptr)
		mlx_destroy_image(mlx, img->img_ptr);
}

void	close_game(t_game *game)
{
	if (!game)
		exit(0);
	
	destroy_texture(&game->vars.floor, game->vars.mlx);
	destroy_texture(&game->vars.wall, game->vars.mlx);
	destroy_texture(&game->vars.player, game->vars.mlx);
	destroy_texture(&game->vars.collectible, game->vars.mlx);
	destroy_texture(&game->vars.exit, game->vars.mlx);
	destroy_texture(&game->vars.screen, game->vars.mlx);

	if (game->vars.win)
		mlx_destroy_window(game->vars.mlx, game->vars.win);
	if (game->vars.mlx)
	{
		mlx_destroy_display(game->vars.mlx);
		free(game->vars.mlx);
	}

	free_map(game->map);

	printf("Game closed.\n");
	exit(0);
}
