/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcabrol <mcabrol@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 18:43:37 by mcabrol           #+#    #+#             */
/*   Updated: 2020/09/20 13:43:09 by judrion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void	intersect_inner(t_obj *obj, double *distance, t_ray *ray)
{
	if (obj->type == SPHERE)
		*distance = intersect_sphere(obj, ray);
	else if (obj->type == PLANE)
		*distance = intersect_plane(obj, ray);
	else if (obj->type == CYLINDER)
		*distance = intersect_cylinder(obj, ray);
	else if (obj->type == CONE)
		*distance = intersect_cone(obj, ray);
	else if (obj->type == BOX)
		*distance = intersect_box(obj, ray);
}

BOOL		intersect(t_ray *ray, size_t *id, t_scene *scene)
{
	BOOL		hit;
	double		distance;
	int			i;

	hit = FALSE;
	i = -1;
	ray->distance = T_MAX;
	while (++i < scene->n)
	{
		intersect_inner(&scene->obj[i], &distance, ray);
		if (distance >= T_MIN && distance < ray->distance)
		{
			ray->distance = distance;
			*id = i;
			hit = TRUE;
		}
	}
	return (hit);
}
