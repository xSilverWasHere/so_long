/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-g <jpedro-g@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:36:04 by jpedro-g          #+#    #+#             */
/*   Updated: 2025/06/20 21:12:01 by jpedro-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void draw_sprite(t_img *dst, t_img *src, int x_offset, int y_offset)
{
	int x, y;
	char *src_pixel;
	char *dst_pixel;

	for (y = 0; y < src->height; y++)
	{
		for (x = 0; x < src->width; x++)
		{
			if (x + x_offset >= dst->width || y + y_offset >= dst->height)
				continue;

			src_pixel = src->img_pixels_ptr + y * src->line_len + x * (src->bits_per_pixel / 8);
			dst_pixel = dst->img_pixels_ptr + (y + y_offset) * dst->line_len + (x + x_offset) * (dst->bits_per_pixel / 8);
			*(unsigned int *)dst_pixel = *(unsigned int *)src_pixel;
		}
	}
}

void draw_sprite_flipped(t_img *dst, t_img *src, int x_offset, int y_offset)
{
	int x, y;
	unsigned int color;
	char *src_pixel;
	char *dst_pixel;

	for (y = 0; y < src->height; y++)
	{
		for (x = 0; x < src->width; x++)
		{
			int flipped_x = src->width - 1 - x;

			if (x + x_offset >= dst->width || y + y_offset >= dst->height)
				continue;

			src_pixel = src->img_pixels_ptr
				+ y * src->line_len
				+ flipped_x * (src->bits_per_pixel / 8);
			color = *(unsigned int *)src_pixel;

			if ((color >> 24) == 0x00)
				continue;

			dst_pixel = dst->img_pixels_ptr
				+ (y + y_offset) * dst->line_len
				+ (x + x_offset) * (dst->bits_per_pixel / 8);
			*(unsigned int *)dst_pixel = color;
		}
	}
}

void update_enemies(t_game *game)
{
	t_enemy *e = game->enemies;

	while (e)
	{
		update_animation(&e->anim);

		int next_x = e->x + e->dir_x;
		int next_y = e->y + e->dir_y;

		if (game->map[next_y][next_x] == '1')
		{
			e->dir_x *= -1;
			e->dir_y *= -1;
		}
		else
		{
			e->x = next_x;
			e->y = next_y;
		}

		if (e->x == game->vars.player_x && e->y == game->vars.player_y)
		{
			game->is_dead = 1;
		}
	}
}

void 	update_animation(t_anim *anim)
{
	anim->frame_timer++;
	if (anim->frame_timer >= anim->frame_delay)
	{
		anim->frame_timer = 0;
		anim->current_frame = (anim->current_frame + 1) % anim->frame_count;
	}
}

void load_texture(t_img *img, void *mlx, char *path)
{
	img->img_ptr = mlx_xpm_file_to_image(mlx, path, &img->width, &img->height);
	if (!img->img_ptr)
		error_exit("Failed to load texture");

	img->img_pixels_ptr = mlx_get_data_addr(img->img_ptr, &img->bits_per_pixel, &img->line_len, &img->endian);
	if (!img->img_pixels_ptr)
		error_exit("Failed to get texture data address");
}

void	load_player_anim(t_anim *anim, void *mlx)
{
	load_texture(&anim->frames[0], mlx, "textures/player_idle1.xpm");
	load_texture(&anim->frames[1], mlx, "textures/player_idle2.xpm");
	anim->frame_count = 2;
	anim->frame_delay = 60;
	anim->current_frame = 0;
	anim->frame_timer = 0;
}

void	load_collectible_anim(t_anim *anim, void *mlx)
{
	load_texture(&anim->frames[0], mlx, "textures/collectible1.xpm");
	load_texture(&anim->frames[1], mlx, "textures/collectible2.xpm");
	anim->frame_count = 2;
	anim->frame_delay = 60;
	anim->current_frame = 0;
	anim->frame_timer = 0;
}

void	load_enemy_anim(t_anim *anim, void *mlx)
{
	load_texture(&anim->frames[0], mlx, "textures/enemy1.xpm");
	load_texture(&anim->frames[1], mlx, "textures/enemy2.xpm");
	anim->frame_count = 2;
	anim->frame_delay = 5;
	anim->current_frame = 0;
	anim->frame_timer = 0;
}

void render_enemies(t_game *game)
{
	t_enemy *enemy = game->enemies;

	while (enemy)
	{
		t_img *frame = &enemy->anim.frames[enemy->anim.current_frame];
		draw_sprite(&game->vars.screen, frame, enemy->x * TILE_SIZE, enemy->y * TILE_SIZE);
		enemy = enemy->next;
	}
}

void draw_player(t_game *game)
{
	t_img *frame;

	if (game->is_dead)
	{
		frame = &game->vars.player_dead;
	}
	else
	{
		frame = &game->vars.player_anim.frames[game->vars.player_anim.current_frame];
	}

	int x = game->vars.player_x * TILE_SIZE;
	int y = game->vars.player_y * TILE_SIZE;

	if (game->vars.facing_right)
		draw_sprite(&game->vars.screen, frame, x, y);
	else
		draw_sprite_flipped(&game->vars.screen, frame, x, y);
}

void	render_map(t_game *game)
{
	int x, y;
	char tile;

	for (y = 0; y < game->height; y++)
	{
		for (x = 0; x < game->width; x++)
		{
			tile = game->map[y][x];
			draw_sprite(&game->vars.screen, &game->vars.floor, x * TILE_SIZE, y * TILE_SIZE);

			if (tile == '1')
				draw_sprite(&game->vars.screen, &game->vars.wall, x * TILE_SIZE, y * TILE_SIZE);
			else if (tile == 'C')
			{
				t_img *frame = &game->vars.collectible_anim.frames[game->vars.collectible_anim.current_frame];
				draw_sprite(&game->vars.screen, frame, x * TILE_SIZE, y * TILE_SIZE);
			}
			else if (tile == 'E')
				draw_sprite(&game->vars.screen, &game->vars.exit, x * TILE_SIZE, y * TILE_SIZE);
		}
	}

	render_enemies(game);
	draw_player(game);
}

