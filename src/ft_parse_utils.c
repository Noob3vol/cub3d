/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguidado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 18:30:31 by iguidado          #+#    #+#             */
/*   Updated: 2020/05/09 18:30:33 by iguidado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int			ft_line_is_empty(char *line)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t')
			return (0);
		line++;
	}
	return (1);
}

int			ft_line_is_map(char *line)
{
	int ret;

	ret = 0;
	while (*line)
	{
		if (*line != ' ' && !(*line >= '0' && *line <= '2'))
			return (0);
		if (*line >= '0' && *line <= '2')
			ret = 1;
		line++;
	}
	return (ret);
}

int			ft_count_param(char **param)
{
	int i;

	i = 0;
	while (param[i])
		i++;
	return (i);
}

int			ft_str_isdigit(char *str)
{
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int			ft_str_is_colorcode(char *str)
{
	int i_comma;
	int nbr;

	while (*str)
	{
		nbr = 0;
		while (*str >= '0' && *str <= '9')
		{
			nbr *= 10;
			nbr += (*str - '0');
			str++;
		}
		if (*str == ',')
		{
			str++;
			i_comma++;
		}
		if (nbr > 255)
			return (0);
	}
	if (i_comma == 2)
		return (1);
	return (0);
}
