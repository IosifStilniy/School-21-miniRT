/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_intersections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 16:32:15 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/22 19:20:59 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	ft_intersect_triangle(t_vrtx p[3], t_cart phit,
		float dist[3], t_cart *closest_norm, t_cart norm)
{
	t_cart	edge[3];
	t_cart	vec_c[3];
	float	k[2];
	float	denom;

	edge[0] = ft_substract_vectors(p[1].dot, p[0].dot);
	edge[1] = ft_substract_vectors(p[2].dot, p[0].dot);
	edge[2] = ft_get_cross_product(edge[1], edge[0]);
	denom = ft_get_dot_product(edge[2], edge[2]);

	// TODO remove these calcs.
	
	edge[0] = ft_substract_vectors(p[1].dot, p[0].dot);
	edge[1] = ft_substract_vectors(p[2].dot, p[1].dot);
	edge[2] = ft_substract_vectors(p[0].dot, p[2].dot);
	vec_c[0] = ft_substract_vectors(phit, p[0].dot);
	vec_c[1] = ft_substract_vectors(phit, p[1].dot);
	vec_c[2] = ft_substract_vectors(phit, p[2].dot);
	if (ft_get_dot_product(norm, ft_get_cross_product(edge[0], vec_c[0])) < 0)
		return (0);
	k[0] = ft_get_dot_product(norm, ft_get_cross_product(edge[1], vec_c[1]));
	if (k[0] < 0)
		return (0);
	k[1] = ft_get_dot_product(norm, ft_get_cross_product(edge[2], vec_c[2]));
	if (k[1] < 0)
		return (0);
	dist[0] = dist[1];
	if (!closest_norm || dist[0] < dist[2])
		return (1);
	k[0] /= denom;
	k[1] /= denom;
	*closest_norm = ft_summ_vectors(ft_summ_vectors(ft_multiply_vector(p[1].norm, k[0]), ft_multiply_vector(p[2].norm, k[1])), ft_multiply_vector(p[0].norm, 1 - k[0] - k[1]));
	*closest_norm = ft_get_vector_norm(*closest_norm, ft_get_vector_length(*closest_norm));
	return (1);
}

void	ft_intersect_plane(t_ray ray, t_cart norm_vector,
			t_cart pos, float *closest_distance)
{
	float	ray_to_plane;
	float	division;
	t_cart	new_plane_pos;

	division = ft_get_dot_product(ray.dir, norm_vector);
	if (division < __FLT_EPSILON__ && division > -__FLT_EPSILON__)
	{
		*closest_distance = INFINITY;
		return ;
	}
	new_plane_pos = ft_substract_vectors(pos, ray.orig);
	ray_to_plane = ft_get_dot_product(new_plane_pos, norm_vector) / division;
	if (ray_to_plane < __FLT_EPSILON__)
	{
		*closest_distance = INFINITY;
		return ;
	}
	*closest_distance = ray_to_plane;
}

float	ft_intersect_sphere(t_ray ray, t_obj *sphere)
{
	float	radius_squared;
	float	ray_in_sphere_half;
	float	ray_to_intersect[2];
	float	center_to_ray_squared;
	float	ray_to_radius_perpendicular;

	ray.orig = ft_substract_vectors(sphere->crdstm.pos, ray.orig);
	ray_to_radius_perpendicular = ft_get_dot_product(ray.dir, ray.orig);
	if (ray_to_radius_perpendicular < 0)
		return (INFINITY);
	center_to_ray_squared = ft_get_dot_product(ray.orig, ray.orig)
		- ray_to_radius_perpendicular * ray_to_radius_perpendicular;
	radius_squared = sphere->outframe * sphere->outframe;
	if (center_to_ray_squared > radius_squared)
		return (INFINITY);
	ray_in_sphere_half = sqrtf(radius_squared - center_to_ray_squared);
	ray_to_intersect[0] = ray_to_radius_perpendicular - ray_in_sphere_half;
	ray_to_intersect[1] = ray_to_radius_perpendicular + ray_in_sphere_half;
	if (ray_to_intersect[0] > ray_to_intersect[1])
		ray_to_intersect[0] = ray_to_intersect[1];
	if (ray_to_intersect[0] < 0)
		return (INFINITY);
	return (ray_to_intersect[0]);
}

void	ft_intersect_polygon(t_ray ray, t_cart *closest_norm,
			t_obj *object, float *closest_distance)
{
	int		i;
	t_vrtx	p[3];
	t_polys	polys;
	t_vrtx	*dots;
	t_cart	phit;
	float	dist[2];

	i = -1;
	dist[0] = INFINITY;
	polys = object->polys;
	dots = object->dots.pos;
	while (++i < polys.polynum)
	{
		ft_intersect_plane(ray, polys.poly[i].norm,
			dots[polys.poly[i].dots[0]].dot, &dist[1]);
		if (dist[1] < dist[0])
		{
			p[0] = dots[polys.poly[i].dots[0]];
			p[1] = dots[polys.poly[i].dots[1]];
			p[2] = dots[polys.poly[i].dots[2]];
			phit = ft_summ_vectors(ft_multiply_vector(ray.dir, dist[1] + 0.0001f), ray.orig);
			ft_intersect_triangle(p, phit, dist, closest_norm, polys.poly[i].norm);
		}
	}
	*closest_distance = dist[0];
}
