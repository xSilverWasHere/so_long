/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:40:30 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/23 14:03:04 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	handle_exit(t_game *game)
{
	if (game->collected == game->total_collectibles)
	{
		printf("You win! Total moves: %d\n", game->moves + 1);
		close_game(game);
		return (1);
	}
	printf("You must collect all items before exiting!\n");
	return (1);
}

static void	handle_collectible(t_game *game, int x, int y)
{
	game->grid[y][x] = '0';
	game->collected++;
	printf("Collected: %d/%d\n", game->collected, game->total_collectibles);
}

void	move_player(t_game *game, int dx, int dy)
{
	int		new_x;
	int		new_y;
	char	tile;

	new_x = game->player.x + dx;
	new_y = game->player.y + dy;
	if (new_x < 0 || new_x >= game->map_width || new_y < 0
		|| new_y >= game->map_height)
		return ;
	tile = game->grid[new_y][new_x];
	if (tile == '1')
		return ;
	if (tile == 'C')
		handle_collectible(game, new_x, new_y);
	if (tile == 'E' && handle_exit(game))
		return ;
	game->player.x = new_x;
	game->player.y = new_y;
	game->moves++;
	printf("Moves: %d\n", game->moves);
	draw(game);
}

int	key_press(int keycode, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keycode == KEY_ESC)
		close_game(game);
	else if (keycode == KEY_W || keycode == KEY_UP)
		game->input.key_up = 1;
	else if (keycode == KEY_S || keycode == KEY_DOWN)
		game->input.key_down = 1;
	else if (keycode == KEY_A || keycode == KEY_LEFT)
		game->input.key_left = 1;
	else if (keycode == KEY_D || keycode == KEY_RIGHT)
		game->input.key_right = 1;
	return (0);
}

int	key_release(int keycode, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keycode == KEY_W || keycode == KEY_UP)
		game->input.key_up = 0;
	else if (keycode == KEY_S || keycode == KEY_DOWN)
		game->input.key_down = 0;
	else if (keycode == KEY_A || keycode == KEY_LEFT)
		game->input.key_left = 0;
	else if (keycode == KEY_D || keycode == KEY_RIGHT)
		game->input.key_right = 0;
	return (0);
}

static void	handle_direction(t_game *g, int dx, int dy)
{
	move_player(g, dx, dy);
	g->input.cooldown = 10;
}

int	handle_continuous_input(void *param)
{
	t_game	*g;

	g = (t_game *)param;
	if (g->input.cooldown > 0)
	{
		g->input.cooldown--;
		return (0);
	}
	if (g->input.key_up)
		handle_direction(g, 0, -1);
	else if (g->input.key_down)
		handle_direction(g, 0, 1);
	else if (g->input.key_left)
		handle_direction(g, -1, 0);
	else if (g->input.key_right)
		handle_direction(g, 1, 0);
	return (0);
}

int	close_game(t_game *game)
{
	mlx_destroy_image(game->mlx, game->floor.img_ptr);
	mlx_destroy_image(game->mlx, game->wall.img_ptr);
	mlx_destroy_image(game->mlx, game->exit.img_ptr);
	mlx_destroy_image(game->mlx, game->collectible.img_ptr);
	mlx_destroy_window(game->mlx, game->win);
	free_map(game);
	exit(0);
}
