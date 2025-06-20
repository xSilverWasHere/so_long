/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:40:11 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/20 20:27:06 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	handle_input(int keycode, t_game *game)
{
	int	new_x = game->vars.player_x;
	int	new_y = game->vars.player_y;
	
	if (game->is_dead)
		return (0);
	if (keycode == XK_Escape)
		error_exit("Exit requested");

	if (keycode == 'w' || keycode == XK_Up)
		new_y--;
	else if (keycode == 's' || keycode == XK_Down)
		new_y++;
	else if (keycode == 'a' || keycode == XK_Left)
	{
		new_x--;
		game->vars.facing_right = 0;
	}
	else if (keycode == 'd' || keycode == XK_Right)
	{
		new_x++;
		game->vars.facing_right = 1;
	}
	else
		return (0);

	// Collision check
	if (new_x < 0 || new_x >= game->width || new_y < 0 || new_y >= game->height)
		return (0);
	if (game->map[new_y][new_x] == '1')
		return (0);

	// Collectibles
	if (game->map[new_y][new_x] == 'C')
	{
		game->map[new_y][new_x] = '0'; // Mark the collectible as collected
		game->collectibles--;
		printf("Collected one item. Remaining: %d\n", game->collectibles);
	}

	if (game->map[new_y][new_x] == 'E')
	{
		if (game->collectibles == 0)
		{
			printf("You win in %d moves!\n", game->player_moves + 1);
			exit(0); // Or error_exit("You win!")
		}
		else
		{
			// Can't exit yet
			printf("You must collect all items before exiting!\n");
			return (0);
		}
	}
	// Move player
	game->vars.player_x = new_x;
	game->vars.player_y = new_y;
	game->player_moves++;
	printf("Move: %d\n", game->player_moves);

	render_map(game);
	return (0);
}