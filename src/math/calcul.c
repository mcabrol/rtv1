/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calcul.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcabrol <mcabrol@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 17:55:37 by mcabrol           #+#    #+#             */
/*   Updated: 2020/02/06 19:11:31 by mcabrol          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double		len(t_vec *v)
{
	return (sqrt(v->x * v->x + v->y * v->y + v->z * v->z));
}

double		dot(t_vec *v1, t_vec *v2)
{
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

double		max(t_vec *v)
{
	if (v->x > v->y && v->x > v->z)
		return (v->x);
	else
		if (v->y > v->z)
			return (v->y);
		else
			return (v->z);
}

double		clamp(double x, double low, double high)
{
	if (x < high)
		if (x > low)
			return (x);
		else
			return (low);
	else
		return (high);
}

void		clamp3(t_vec *v, double low, double high, t_vec *dest)
{
	vec(clamp(v->x, low, high),
		clamp(v->y, low, high),
		clamp(v->z, low, high),
		dest);
}

uint8_t		to_byte(double x, double gamma)
{
	return ((uint8_t)clamp(255.0 * pow(x, 1.0 / gamma), 0.0, 255.0));
}

double		reflectance(double n1, double n2)
{
	double sqrt_r;

	sqrt_r = (n1 - n2) / (n1 + n2);
	return (sqrt_r * sqrt_r);
}

double		schlick_reflectance(double n1, double n2, double c)
{
	double ro;
	ro = reflectance(n1, n2);
	return (ro + (1 - ro) * c * c * c * c * c);
}

double		quadratic(double k1, double k2, double k3)
{
	double	des;
	double	t1;
	double	t2;
	double	t_min;

	t_min = T_MAX;
	if ((des = k2 * k2 - 4 * k1 * k3) >= 0)
	{
		t1 = (-k2 + sqrt(des)) / (2 * k1);
		t2 = (-k2 - sqrt(des)) / (2 * k1);
		if (t1 >= T_MIN && t1 < T_MAX && t1 < t_min)
			t_min = t1;
		if (t2 >= T_MIN && t2 < T_MAX && t2 < t_min)
			t_min = t2;
	}
	return (t_min);
}

void    	quadratic_base(t_vec k, t_vec *t)
{
    double        des;

    if ((des = k.y * k.y - 4 * k.x * k.z) >= 0)
    {
        t->x = (-k.y + sqrt(des)) / (2 * k.x);
        t->y = (-k.y - sqrt(des)) / (2 * k.x);
    }
    else
    {
        t->x = T_MAX;
        t->y = T_MAX;
       	t->z = T_MAX;
    }
}
double		check_cut(double t_min, t_obj *obj, t_vec *p)
{
    t_vec		cut_plane_pos;
    t_vec		pc;

	sum(&obj->p, &obj->cut_shift, &cut_plane_pos);
	sub(p, &cut_plane_pos, &pc);
    if (dot(&obj->cut_dir, &pc) >= 0)
        t_min = T_MAX;
    return (t_min);
}

double		define_tmin(t_vec t)
{
	if (t.x >= T_MIN && t.x < T_MAX && t.x < t.z)
		t.z = t.x;
	if (t.y >= T_MIN && t.y < T_MAX && t.y < t.z)
		t.z = t.y;
	return (t.z);
}

double 		check_pnt(t_vec *k, t_vec *direction, t_vec *origin, t_obj *obj)
{
	t_vec 	t;
	t_vec 	ac;
	t_vec	a;
	t_vec	b;
	t_vec	o;
	t_vec	p;

	t_vec 	oc;
	double 	len_ac;
	double	m[2];

	t.z = T_MAX;
	sub(origin, &obj->p, &oc);
	quadratic_base(*k, &t);
	if (t.x == T_MAX && t.y == T_MAX && t.z == T_MAX)
        return (T_MAX);

	m[0] = (dot(direction, &obj->d) * t.x) + dot(&oc, &obj->d);
	nmulti(&obj->d, m[0], &a);
	sub(&obj->p, &obj->p, &b);
	sum(&a, &b, &ac);
	len_ac = len(&ac);
	if (obj->h != 0 && (len_ac > obj->h || dot(&ac, &obj->d) < 0))
        t.x = T_MAX;
	if (t.x != T_MAX && obj->cut != 0)
	{
		nmulti(direction, t.x, &o);
		sum(origin, &o, &p);
		t.x = check_cut(t.x, obj, &p);
	}

	m[1] = (dot(direction, &obj->d) * t.y) + dot(&oc, &obj->d);
	nmulti(&obj->d, m[1], &a);
	sub(&obj->p, &obj->p, &b);
	sum(&a, &b, &ac);
	len_ac = len(&ac);
	if (obj->h != 0 && (len_ac > obj->h || dot(&ac, &obj->d) < 0))
        t.y = T_MAX;
	if (t.y != T_MAX && obj->cut != 0)
	{
		nmulti(direction, t.y, &o);
		sum(origin, &o, &p);
		t.y = check_cut(t.y, obj, &p);
	}
	t.z = define_tmin(t);
	return (t.z);
}
