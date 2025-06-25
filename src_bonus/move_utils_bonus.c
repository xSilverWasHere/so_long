/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 09:36:26 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/25 09:37:37 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	process_input(t_game *game, int x, int y)
{
	move_player(game, x, y);
	game->input.cooldown = 10;
}

int	check_collision(t_game *game, int x, int y)
{
	if (x < 0 || x >= game->map_width || y < 0 || y >= game->map_height)
		return (1);
	if (game->grid[y][x] == '1')
		return (1);
	return (0);
}

void	handle_collectible(t_game *game, int x, int y)
{
	game->grid[y][x] = '0';
	game->collected++;
	printf("Collected: %d/%d\n", game->collected, game->total_collectibles);
}

int	handle_exit(t_game *game, int x, int y)
{
	(void)x;
	(void)y;
	if (game->collected == game->total_collectibles)
	{
		printf("You win! Total moves: %d\n", game->moves + 1);
		close_game(game);
		return (1);
	}
	printf("You must collect all items before exiting!\n");
	return (1);
}

void	check_enemy_collision(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->num_enemies)
	{
		if (game->player.x == game->enemies[i].x
			&& game->player.y == game->enemies[i].y)
			error_exit("You ran into an enemy!", game);
		i++;
	}
}
