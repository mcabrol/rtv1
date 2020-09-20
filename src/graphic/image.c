/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcabrol <mcabrol@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/31 17:28:42 by mcabrol           #+#    #+#             */
/*   Updated: 2020/09/20 15:04:11 by mcabrol          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static char		*init_win_name(int id_win, int id_render)
{
	char	*win_name;
	char	*string_id;
	char	*tmp;

	string_id = ft_itoa(id_win);
	win_name = ft_strjoin("render - win #", string_id);
	free(string_id);
	string_id = ft_itoa(id_render);
	tmp = win_name;
	win_name = ft_strjoin(win_name, " - render #");
	free(tmp);
	tmp = win_name;
	win_name = ft_strjoin(win_name, string_id);
	free(tmp);
	free(string_id);
	return (win_name);
}

int				image(t_rtv1 *rtv1)
{
	int		x;
	int		y;
	int		i;
	t_vec	*screen;
	t_win	*win;

	win = &rtv1->image[rtv1->id_win];
	screen = rtv1->screen;
	y = -1;
	while (++y <= rtv1->scene.height)
	{
		x = -1;
		while (++x <= rtv1->scene.width)
		{
			i = (rtv1->scene.height - 1 - y) * rtv1->scene.width + x;
			put_pixel_vector(rtv1, x, y, &screen[i]);
		}
	}
	mlx_put_image_to_window(rtv1->mlx_ptr, win->win_ptr, win->img_ptr, 0, 0);
	return (1);
}

static t_win	*init_win_array(void)
{
	int		i;
	t_win	*image;

	image = (t_win*)ft_memalloc(sizeof(t_win) * MAX_WIN);
	i = 0;
	while (i < MAX_WIN)
	{
		image[i].available = 1;
		image[i].id_window = i;
		i = i + 1;
	}
	return (image);
}

int				init_image(t_rtv1 *rtv1)
{
	char	*win_name;
	int		id;

	if (rtv1->id_win > MAX_WIN)
	{
		clean_image_array(rtv1);
		return (EXIT_FAILURE);
	}
	if (rtv1->id_win == 0)
		rtv1->image = init_win_array();
	if (rtv1->image)
	{
		id = -1;
		while (++id < MAX_WIN)
			if (rtv1->image[id].available == 1)
				break ;
		win_name = init_win_name(id, rtv1->id_render);
		rtv1->image[id] = window(rtv1->mlx_ptr, rtv1->scene.width,
								rtv1->scene.height, win_name);
		mlx_hook(rtv1->image[id].win_ptr, 17, (1L << 17), close_rcross, \
		&rtv1->image[id]);
		rtv1->image[id].id_window = id;
		rtv1->image[id].available = 0;
		rtv1->id_win = id;
		free(win_name);
	}
	return (EXIT_SUCCESS);
}

int				write_ppm(t_rtv1 *rtv1)
{
	int		x;
	int		y;
	int		i;
	int		fd;
	char	*name;

	y = -1;
	name = stamp();
	fd = open(name, O_WRONLY | O_APPEND | O_CREAT, 0644);
	ft_dprintf(fd, "P3\n%d %d\n%d\n", rtv1->scene.width, rtv1->scene.height,\
	255);
	while (++y < rtv1->scene.height)
	{
		x = -1;
		while (++x < rtv1->scene.width)
		{
			i = (rtv1->scene.height - 1 - y) * rtv1->scene.width + x;
			ft_dprintf(fd, "%d %d %d ", to_byte(rtv1->screen[i].x, GAMMA),
											to_byte(rtv1->screen[i].y, GAMMA),
											to_byte(rtv1->screen[i].z, GAMMA));
		}
	}
	close(fd);
	return (1);
}
