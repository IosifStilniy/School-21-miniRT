/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:39:57 by dcelsa            #+#    #+#             */
/*   Updated: 2022/05/13 23:23:55 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	quartrot(t_cart *pos, t_axis *axis)
{	
	double	mlts[3][3];
	t_cart	axsr;

	axsr.x = sinf(axis->ang / 2) * axis->vector.x;
	axsr.y = sinf(axis->ang / 2) * axis->vector.y;
	axsr.z = sinf(axis->ang / 2) * axis->vector.z;
	mlts[0][0] = (1 - 2 * (powf(axsr.y, 2) + powf(axsr.z, 2))) * pos->x;
	mlts[0][1] = (2 * (axsr.x * axsr.y - axsr.z * cosf(axis->ang / 2))) * pos->y;
	mlts[0][2] = (2 * (axsr.x * axsr.z + axsr.y * cosf(axis->ang / 2))) * pos->z;
	mlts[1][0] = (2 * (axsr.x * axsr.y + axsr.z * cosf(axis->ang / 2))) * pos->x;
	mlts[1][1] = (1 - 2 * (powf(axsr.x, 2) + powf(axsr.z, 2))) * pos->y;
	mlts[1][2] = (2 * (axsr.y * axsr.z - axsr.x * cosf(axis->ang / 2))) * pos->z;
	mlts[2][0] = (2 * (axsr.x * axsr.z - axsr.y * cosf(axis->ang / 2))) * pos->x;
	mlts[2][1] = (2 * (axsr.y * axsr.z + axsr.x * cosf(axis->ang / 2))) * pos->y;
	mlts[2][2] = (1 - 2 * (powf(axsr.x, 2) + powf(axsr.y, 2))) * pos->z;
	pos->x = mlts[0][0] + mlts[0][1] + mlts[0][2];
	pos->y = mlts[1][0] + mlts[1][1] + mlts[1][2];
	pos->z = mlts[2][0] + mlts[2][1] + mlts[2][2];
}

void	engine(t_dots *dots, t_polys *polys, t_crdstm *crdstm, t_rot *rot)
{
	int		i;

	wrldtoobjaxis(crdstm, rot);
	i = -1;
	while (++i < dots->dotsnum)
	{
		dots->pos[i].x = dots->dots[i].x * dots->scale;
		dots->pos[i].y = dots->dots[i].y * dots->scale;
		dots->pos[i].z = dots->dots[i].z * dots->scale;
		quartrot(&dots->pos[i], &rot->axis);
	}
	i = -1;
	while (++i < polys->polynum)
	{
		polys->poly[i].norm = polys->poly[i].srcnorm;
		quartrot(&polys->poly[i].norm, &rot->axis);
	}
}

void	flatanglehandler(t_rot *rot, t_cart *ref)
{
	float	ang;

	axisbuilder(rot->start, rot->end, &rot->axis);
	ang = rot->axis.ang;
	if (!comparef(rot->axis.length, 0, 0.0001) && !comparef(rot->axis.ang, M_PI, 0.001))
		;
	else if (!ref)
	{
		vectorbuilder(1, 0, 0, &rot->axis);
		if (!comparef(rot->end->x, 0, 0.001) || !comparef(rot->end->z, 0, 0.001))
			vectorbuilder(0, 1, 0, &rot->axis);
	}
	else if (ref != rot->start)
		axisbuilder(ref, rot->end, &rot->axis);
	rot->axis.ang = ang;
}

void	objtoobjaxis(t_crdstm *src, t_crdstm *dst, t_rot *rot)
{
	t_crdstm	world;
	t_cart		ref;
	t_cart		pos;
	
	vectorbuilder(1, 0, 0, &world.ox);
	vectorbuilder(0, 1, 0, &world.oy);
	vectorbuilder(0, 0, 1, &world.oz);
	if (!src)
		src = &world;
	if (!dst)
		dst = &world;
	cartbuilder(1, 1, 1, &pos);
	crdstmrot(src, rot, &src->oz, &dst->oz);
	quartrot(&pos, &rot->axis);
	crdstmrot(src, rot, &src->ox, &dst->ox);
	quartrot(&pos, &rot->axis);
	cartbuilder(1, 1, 1, &ref);
	axisbuilder(&ref, &pos, &rot->axis);
}

void	objtoobjpos(t_cart *center, t_cart *dot)
{
	dot->x -= center->x;
	dot->y -= center->y;
	dot->z -= center->z;
}
