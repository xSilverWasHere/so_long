/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:40:30 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/22 19:15:49 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	move_player(t_game *game, int dx, int dy)
{
	int	new_x = game->player.x + dx;
	int	new_y = game->player.y + dy;
	char	tile;

	// Bounds check
	if (new_x < 0 || new_x >= game->map_width || new_y < 0 || new_y >= game->map_height)
		return;

	tile = game->grid[new_y][new_x];

	// Wall collision
	if (tile == '1')
		return;

	// Collectible
	if (tile == 'C')
	{
		game->grid[new_y][new_x] = '0';  // Remove from map
		game->collected++;
		printf("Collected: %d/%d\n", game->collected, game->total_collectibles);
	}

	// Exit (only if all collectibles gathered)
	if (tile == 'E')
	{
		if (game->collected == game->total_collectibles)
		{
			printf("You win! Total moves: %d\n", game->moves + 1);
			close_game(game); // or exit(0);
			return;
		}
		else
		{
			printf("You must collect all items before exiting!\n");
			return;
		}
	}

	// Move player
	game->player.x = new_x;
	game->player.y = new_y;
	game->moves++;
	printf("Moves: %d\n", game->moves);
	draw(game);
}

int key_press(int keycode, void *param)
{
	t_game *game = (t_game *)param;

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
	t_game *game = (t_game *)param;

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

int handle_continuous_input(void *param)
{
	t_game *game = (t_game *)param;

	if (game->input.cooldown > 0)
	{
		game->input.cooldown--;
		return (0);
	}
	if (game->input.key_up)
	{
		move_player(game, 0, -1);
		game->input.cooldown = 10;
	}
	else if (game->input.key_down)
	{
		move_player(game, 0, 1);
		game->input.cooldown = 10;
	}
	else if (game->input.key_left)
	{
		move_player(game, -1, 0);
		game->input.cooldown = 10;
	}
	else if (game->input.key_right)
	{
		move_player(game, 1, 0);
		game->input.cooldown = 10;
	}
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