/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguidado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 18:48:19 by iguidado          #+#    #+#             */
/*   Updated: 2020/07/17 12:08:30 by iguidado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
**	char		*ft_reverse_row(char *row)
**	{
**		int		i;
**		int		strlen;
**		char	*rev_str;
**	
**		i = 0;
**		strlen = ft_strlen(row);
**		rev_str = malloc(sizeof(char) * (strlen + 1));
**		rev_str[strlen] = 0;
**		while (row[i])
**		{
**			rev_str[strlen - i - 1] = row[i];
**			i++;
**		}
**		return (rev_str);
**	}
*/

/*
**	int			ft_set_map(t_file_data *fdata, t_map *map)
**	{
**		map->map = ft_split(fdata->line, ' ');
**
**	}
*/

/*
**	Maybe to a manip malloc lib
**
**	  char		*ft_add_newline(char **str)
**	  {
**		char *new;
**		int strlen;
**
**		strlen = ft_strlen(*str);
**		if (!(new = (char *)malloc(sizeof(char) * (strlen + 2))))
**			return (NULL);
**		new[strlen + 1] = '\0';
**		new[strlen] = '\n';
**		while (strlen--)
**			new[strlen] = (*str)[strlen];
**		free(*str);
**		return (new);
**	  }
*/

/*
**	Don't enter bad value, it will crash
*/

void		ft_get_spawn(t_config *cfg, t_file_data *fdata, int x, int y)
{
	if (cfg->spwn.pos.x >= 0)
		ft_manage_parse_error(ERROR_MAP_PLAYER_SPAWN, cfg, fdata);
	else
	{
		cfg->spwn.pos.y = y;
		cfg->spwn.pos.x = x;
		if (cfg->map.data[y][x] == 'N')
			cfg->spwn.angle = M_PI;
		if (cfg->map.data[y][x] == 'E')
			cfg->spwn.angle = M_PI / 2.0f;
		if (cfg->map.data[y][x] == 'S')
			cfg->spwn.angle = 0.0f;
		if (cfg->map.data[y][x] == 'W')
			cfg->spwn.angle = M_PI * (3.0f / 2.0f);
	}
}

void		ft_add_obj(t_config *cfg, t_file_data *fdata, int x, int y)
{
	t_obj_spwn *new;

	if (!(new = (t_obj_spwn *)malloc(sizeof(t_obj_spwn))))
		ft_manage_parse_error(ERROR_SYSCALL, cfg, fdata);
	new->pos.x = x;
	new->pos.y = y;
	cfg->map.data[y][x] = '0';
	new->next = cfg->obj_list;
	cfg->obj_list = new;
}

/*
**	can replace  ft_isset, to see if it is more performant
**
**	char		ft_istile(char c)
**	{
**		if ((c >= '0' && c <= '2') || c == ' ')
**			return (1);
**		if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
**			return (2);
**		return (0);
**	}
*/

void		ft_equalize_row(t_map *map)
{
	int		y;
	int		x;
	char	*new;

	y = 0;
	while (map->data[y])
	{
		x = 0;
		new = malloc(sizeof(char) * (map->res.x + 1));
		new[map->res.x] = 0;
		while (map->data[y][x])
		{
			new[map->res.x - 1 - x] = map->data[y][x];
			x++;
		}
			while (map->res.x -x - 1 >= 0)
			{
				new[map->res.x - x - 1] = ' ';
				x++;
			}
		free(map->data[y]);
		map->data[y] = new;
		y++;
	}
}

void		ft_process_map(t_config *cfg, t_file_data *fdata)
{
	int		x;
	int		y;
	char	tile;

	ft_equalize_row(&cfg->map);
	y = 0;
	while (cfg->map.data[y])
	{ 
		x = 0;
		while ((tile = cfg->map.data[y][x]))
		{
			if (!ft_isset(TILES, tile))
				ft_manage_parse_error(ERROR_MAP_BAD_TILE, cfg, fdata);
			ft_check_tile_type(cfg, fdata, x, y);
			if (ft_isset(SPAWN, tile))
				ft_get_spawn(cfg, fdata, x, y);
			if (tile == '2')
				ft_add_obj(cfg, fdata, x, y);
			x++;
		}
		y++;
	}
	cfg->map.res.y = y;
	if (cfg->spwn.pos.x < 0)
		ft_manage_parse_error(ERROR_MAP_PLAYER_SPAWN, cfg, fdata);
	cfg->map.data[cfg->spwn.pos.y][cfg->spwn.pos.x] = '0';
}

int			ft_add_row(char ***ptr_map, char *row)
{
	int		i;
	int		ret;
	char	**new;

	if (!row)
		return (0);
	i = 0;
	if (*ptr_map)
	{
		while ((*ptr_map)[i])
			i++;
	}
	if (!(new = (char **)malloc(sizeof(char *) * (i + 2))))
		return (0);
	new[i + 1] = 0;
	new[i] = row;
	while (i--)
		new[i] = (*ptr_map)[i];
	if (*ptr_map)
		free(*ptr_map);
	*ptr_map = new;
	return (1);
}

void		ft_add_map(t_config *cfg, t_file_data *fdata)
{
	int			ret;
	int			map_rows;
	char		*current_line;
	char		*next_line;

	while (!ft_line_is_map(fdata->line))
	{
		free(fdata->line);
		if ((ret = get_next_line(fdata->fd, &fdata->line)) == -1)
			ft_manage_parse_error(ERROR_SYSCALL, cfg, fdata);
		else if (!ret)
			ft_manage_parse_error(ERROR_FILE_END, cfg, fdata);
		fdata->line_nb++;
	}
	ft_add_row(&cfg->map.data, fdata->line);
	cfg->map.res.x = ft_strlen(fdata->line);
	while (get_next_line(fdata->fd, &fdata->line))
	{
		ft_add_row(&cfg->map.data,fdata->line);
		if (ft_strlen(fdata->line) > cfg->map.res.x)
			cfg->map.res.x = ft_strlen(fdata->line);
	}
	ft_add_row(&cfg->map.data, fdata->line);
	ft_process_map(cfg, fdata);
}
