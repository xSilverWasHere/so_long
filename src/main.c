/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:19:04 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/20 21:04:23 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	game_loop(void *param)
{
	t_game *game = (t_game *)param;

	update_enemies(game);
	update_animation(&game->vars.player_anim);
	update_animation(&game->vars.collectible_anim);

	render_map(game);

	mlx_put_image_to_window(game->vars.mlx, game->vars.win, game->vars.screen.img_ptr, 0, 0);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		error_exit("Usage: ./so_long <map.ber>");

	// Setup
	game.is_dead = 0;
	game.death_anim_done = 0;
	game.vars.facing_right = 1;
	game.enemies = NULL;

	game.map = read_map_file(argv[1], &game.height);
	if (!validate_map(game.map, &game.width, game.height))
		error_exit("Invalid map");
	game.collectibles = count_collectibles(game.map, game.height);

	// MLX and image
	game.vars.mlx = mlx_init();
	game.vars.win = mlx_new_window(game.vars.mlx, game.width * TILE_SIZE, game.height * TILE_SIZE, "so_long");

	int	screen_width = game.width * TILE_SIZE;
	int	screen_height = game.height * TILE_SIZE;

	game.vars.screen.img_ptr = mlx_new_image(game.vars.mlx, screen_width, screen_height);
	game.vars.screen.img_pixels_ptr = mlx_get_data_addr(
		game.vars.screen.img_ptr,
		&game.vars.screen.bits_per_pixel,
		&game.vars.screen.line_len,
		&game.vars.screen.endian
	);
	game.vars.screen.width = screen_width;
	game.vars.screen.height = screen_height;

	// Load textures
	load_texture(&game.vars.floor, game.vars.mlx, "textures/floor.xpm");
	load_texture(&game.vars.wall, game.vars.mlx, "textures/wall.xpm");
	load_texture(&game.vars.player, game.vars.mlx, "textures/player_idle1.xpm");
	load_texture(&game.vars.enemy, game.vars.mlx, "textures/enemy1.xpm");
	load_texture(&game.vars.collectible, game.vars.mlx, "textures/collectible1.xpm");
	load_texture(&game.vars.exit, game.vars.mlx, "textures/exit.xpm");
	load_texture(&game.vars.player_dead, game.vars.mlx, "textures/player_death1.xpm");

	// Load animations
	load_player_anim(&game.vars.player_anim, game.vars.mlx);
	load_collectible_anim(&game.vars.collectible_anim, game.vars.mlx);
	load_enemy_anim(&game.vars.enemy_anim, game.vars.mlx);

	find_and_place_player(&game);
	find_and_place_enemies(&game);

	// Game loop
	mlx_loop_hook(game.vars.mlx, game_loop, &game);
	mlx_key_hook(game.vars.win, handle_input, &game);
	mlx_loop(game.vars.mlx);

	return (0);
}