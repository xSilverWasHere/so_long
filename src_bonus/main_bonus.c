/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:37:23 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/22 19:23:18 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
	{
		ft_putendl_fd("Usage: ./so_long <map.ber>", 2);
		return (1);
	}

	ft_memset(&game, 0, sizeof(t_game));
	load_map(&game, argv[1]);
	init_enemies(&game);
	init_game(&game);
	draw(&game);
	mlx_hook(game.win, 2, 1L<<0, key_press, &game);
	mlx_hook(game.win, 3, 1L<<1, key_release, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
	return (0);
}

void update_enemies(t_game *game)
{
	for (int i = 0; i < game->num_enemies; i++)
	{
		t_enemy *e = &game->enemies[i];
		int next_x = e->x + e->dir_x;
		int next_y = e->y + e->dir_y;

		if (next_y >= 0 && next_y < game->map_height &&
			next_x >= 0 && next_x < game->map_width &&
			game->grid[next_y][next_x] != '1')
		{
			e->x = next_x;
			e->y = next_y;
			e->anim_index = (e->anim_index + 1) % 2;
		}
		else
		{
			e->dir_x *= -1;
			e->dir_y *= -1;
		}

		if (e->x == game->player.x && e->y == game->player.y)
			error_exit("You were caught by an enemy!", game);
	}
}

int game_loop(void *param)
{
    t_game *game = (t_game *)param;

    game->frame_count++;
    if (game->frame_count % 20 == 0)
	{
        game->player.anim_index++;
		game->collectible_anim_index = (game->collectible_anim_index + 1) % 2;
        update_enemies(game);
	}
    handle_continuous_input(game);
    draw(game);
    return (0);
}