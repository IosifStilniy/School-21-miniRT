/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/23 20:55:24 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	wininit(t_info *info, char *prog, char *file)
{
	char	*buf;

	buf = ft_strjoin(prog, ": ");
	info->win.header = ft_strjoin(buf, file);
	free(buf);
	info->win.res.x = RESX;
	info->win.res.y = RESY;
	info->win.win = mlx_new_window(info->mlx_ptr, info->win.res.x, info->win.res.y, info->win.header);
	info->win.cntr.x = info->win.res.x / 2;
	info->win.cntr.y = info->win.res.y / 2;
	info->data.res.x = RESX;
	info->data.res.y = RESY;
	info->data.img = mlx_new_image(info->mlx_ptr, info->data.res.x, info->data.res.y);
	info->data.cntr.x = info->data.res.x / 2;
	info->data.cntr.y = info->data.res.y / 2;
}

int	repairmouse(int x, int y, t_info *info)
{
	(void)x;
	mlx_mouse_move(info->win.win, 0, 0);
	if (!(y - 1 <= info->mouse.yshift && info->mouse.yshift <= y + 1))
	{
		info->mouse.yshift = y;
		return (0);
	}
	mlx_hook(info->win.win, 6, 1L << 6, &mousemove, info);
	return (0);
}

void	pudge(t_info *info)
{
	info->keybrd.render = FALSE;
	info->keybrd.interface = FALSE;
	info->mouse.yshift = 0;
	mlx_mouse_hide();
	mlx_hook(info->win.win, 2, 1L, &keydownhndlr, info);
	mlx_hook(info->win.win, 3, 1L << 1, &keyuphndlr, info);
	mlx_hook(info->win.win, 5, 1L << 3, &btnup, info);
	mlx_hook(info->win.win, 6, 1L << 6, &repairmouse, info);
}

void	initobjs(char *file, t_info *info)
{
	int		fd;

	fd = file_check(file, info->prog);
	info->a_light.determined = FALSE;
	info->lights.determined = FALSE;
	info->win.camera.determined = FALSE;
	ft_read_information(fd, info);
	createframerouts(info->objects);
	info->win.camera.objs = NULL;
	createcamobjs(&info->win.camera.objs, info->objects);
}

int	main(int argc, char **argv)
{
	t_info	info;

	info.prog = ft_strrchr(*argv++, '/') + 1;
	if (argc != 2)
		customerr(info.prog, INVINP, FALSE);
	info.mlx_ptr = mlx_init();
	wininit(&info, info.prog, *argv);
	initobjs(*argv, &info);
	initinterface(&info.interface, info.mlx_ptr, &info.win.res);
	initview(info.objects, &info.win.camera, &info.lights);
	pudge(&info);
	ft_draw_screen(&info);
	mlx_loop(info.mlx_ptr);
	return (0);
}
