/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:40:30 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/23 12:44:09 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	move_player(t_game *game, int dx, int dy)
{
	int		new_x;
	int		new_y;
	int		i;
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
	{
		game->grid[new_y][new_x] = '0';
		game->collected++;
		printf("Collected: %d/%d\n", game->collected, game->total_collectibles);
	}
	if (tile == 'E')
	{
		if (game->collected == game->total_collectibles)
		{
			printf("You win! Total moves: %d\n", game->moves + 1);
			close_game(game);
			return ;
		}
		else
		{
			printf("You must collect all items before exiting!\n");
			return ;
		}
	}
	game->player.x = new_x;
	game->player.y = new_y;
	if (dx < 0)
		game->player.facing = -1;
	else if (dx > 0)
		game->player.facing = 1;
	game->player.is_moving = 1;
	game->moves++;
	printf("Moves: %d\n", game->moves);
	i = 0;
	while (i < game->num_enemies)
	{
		if (game->player.x == game->enemies[i].x
			&& game->player.y == game->enemies[i].y)
		{
			error_exit("You ran into an enemy!", game);
		}
		i++;
	}
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
