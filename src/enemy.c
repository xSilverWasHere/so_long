/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:30:41 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/20 20:26:37 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void add_enemy(t_game *game, int x, int y)
{
	t_enemy *new_enemy;

	new_enemy = malloc(sizeof(t_enemy));
	if (!new_enemy)
		error_exit("Failed to allocate memory for enemy");

	new_enemy->x = x;
	new_enemy->y = y;
	new_enemy->dir_x = 1;
	new_enemy->dir_y = 0;
	new_enemy->anim = game->vars.enemy_anim;
	new_enemy->next = game->enemies;
	game->enemies = new_enemy;
}