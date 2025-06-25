/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:40:30 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 09:41:15 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	move_player(t_game *game, int dx, int dy)
{
	int		x;
	int		y;
	char	tile;

	x = game->player.x + dx;
	y = game->player.y + dy;
	if (check_collision(game, x, y))
		return ;
	tile = game->grid[y][x];
	if (tile == 'C')
		handle_collectible(game, x, y);
	if (tile == 'E' && handle_exit(game, x, y))
		return ;
	game->player.x = x;
	game->player.y = y;
	if (dx < 0)
		game->player.facing = -1;
	else if (dx > 0)
		game->player.facing = 1;
	game->player.is_moving = 1;
	game->moves++;
	printf("Moves: %d\n", game->moves);
	check_enemy_collision(game);
	game->player.anim_index ^= 1;
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

int	handle_continuous_input(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (game->input.cooldown > 0)
	{
		game->input.cooldown--;
		return (0);
	}
	if (game->input.key_up)
		process_input(game, 0, -1);
	else if (game->input.key_down)
		process_input(game, 0, 1);
	else if (game->input.key_left)
		process_input(game, -1, 0);
	else if (game->input.key_right)
		process_input(game, 1, 0);
	return (0);
}

int	close_game(t_game *game)
{
	if (game->enemies)
		free(game->enemies);
	mlx_destroy_image(game->mlx, game->floor.img_ptr);
	mlx_destroy_image(game->mlx, game->wall.img_ptr);
	mlx_destroy_image(game->mlx, game->exit.img_ptr);
	mlx_destroy_image(game->mlx, game->collectible->img_ptr);
	mlx_destroy_window(game->mlx, game->win);
	free_map(game);
	exit(0);
}
