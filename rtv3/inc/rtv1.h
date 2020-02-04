/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcabrol <mcabrol@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 18:42:53 by mcabrol           #+#    #+#             */
/*   Updated: 2020/01/17 22:22:41 by mcabrol          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include "../libft/inc/libft.h"
# include "../minilibx-linux/mlx.h"

# include <math.h>
# include <stdint.h>
# include <fcntl.h>

# define SAMPLES 				8

# define HEIGHT					650u
# define WIDTH					700u
# define ZERO					0.0, 0.0, 0.0
# define FALSE					0
# define TRUE					1

# define SPHERE					0
# define PLANE					1

# define DIFF					0
# define SPEC					1
# define REFR					2

# define F_GREY					0xd1d1d1
# define F_BLUE					0x0000FF
# define F_RED					0xFF0000
# define F_GREEN				0x00FF00
# define F_WHITE				0xFFFFFF
# define F_BLACK				0x000000

# define REFRACTIVE_INDEX_OUT	1.0
# define REFRACTIVE_INDEX_IN	1.5
# define PI 					3.14159265358979323846
# define EPSILON_SPHERE			1e-4
# define GAMMA 					2.2

# define RAND48_SEED_0			(0x330e)
# define RAND48_SEED_1			(0xabcd)
# define RAND48_SEED_2			(0x1234)
# define RAND48_MULT_0			(0xe66d)
# define RAND48_MULT_1			(0xdeec)
# define RAND48_MULT_2			(0x0005)
# define RAND48_ADD   			(0x000b)

typedef int 		BOOL;

# include "struct.h"


/*
**	rtv1.c
*/

void 			rtv1(t_win *win);
int				check(int ac, char **av);
int				error(char *strerror);


/*
**	vector.c
*/

void			vec(double a, double b, double c, t_vec *dest);
t_vec			vecp(double a, double b, double c);
void			veccp(t_vec *v, t_vec *dest);
t_vec			*norm(t_vec *v);
double 			norm_s(t_vec *v);

/*
**	ray.c
*/

void			ray(t_vec o, t_vec d, double tmin, double tmax, int depth,
				t_ray *dest);
void 			prepare_ray(t_render *rt, t_radiance *target, t_cam *cam);
void			init_cam(t_cam *cam);
void			eval(t_ray *r, double t, t_vec *dest);
void			printr(t_ray *r);
void			printv(t_vec *v);

/*
**	radiance.c
*/

void			radiance(t_scene *scene, t_ray *ray, t_render *rt);

/*
**	sphere.c
*/

t_sphere		sphere(int t, double r, t_vec p, t_vec d, t_vec e, t_vec f, int reflect);

/*
**	sphere.c
*/

void 			init_scene(t_scene *scene);

/*
**	intersect.c
*/

BOOL 			intersect(t_ray *ray, size_t *id, t_scene *scene);
BOOL 			intersect_sphere(t_sphere *sphere, t_ray *ray);
BOOL			intersect_plane(t_sphere *sphere, t_ray *ray);


/*
**	operator.c
*/

void			sum(t_vec *v1, t_vec *v2, t_vec *dest);
void			sub(t_vec *v1, t_vec *v2, t_vec *dest);
void			multiplication(t_vec *v1, t_vec *v2, t_vec *dest);
void			nmulti(t_vec *v1, double n, t_vec *dest);
void			divide(t_vec *v1, t_vec *v2, t_vec *dest);
void			ndivide(t_vec *v1, double n, t_vec *dest);
void			divide3(double a, t_vec *v, t_vec *dest);
void			minus(t_vec *v, t_vec *dest);
void			cross(t_vec *v1, t_vec *v2, t_vec *dest);
void			sum_(t_vec *v1, t_vec *v2);
void			multi_(t_vec *v1, t_vec *v2);
void			nmulti_(t_vec *v1, double n);
void			ndivide_(t_vec *v1, double n);

/*
**	calcul.c
*/

double			dot(t_vec *v1, t_vec *v2);
double			max(t_vec *v);
double			clamp(double x, double low, double high);
void			clamp3(t_vec *v, double low, double high, t_vec *dest);
uint8_t			to_byte(double x, double gamma);
double			reflectance(double n1, double n2);
double			schlick_reflectance(double n1, double n2, double c);

/*
**	sample.c
*/

void			cosine_weighted_sample(double u1, double u2, t_vec *dest);

/*
**	sample.c
*/

void			init_seed(t_render *rt);

/*
**	specular.c
*/

void			specular_reflect(t_vec *d, t_vec *n, t_vec *dest);
t_vec			specular_transmit(t_vec *d, t_vec *n, double n_out,
				double n_in, double *pr, unsigned short xseed[3]);

/*
**	mlx.c
*/

int				init_window(char **av, t_win *win);
void			erase(t_win *win);
int				expose_hook(t_win *win);
void			put_pixel(t_win *win, int x, int y, int color);
void			put_pixel_vector(t_win *win, int x, int y, t_vec *v);
int				key(int keycode, t_win *win);

/*
**	gui.c
*/

void 			bg_gradient(t_win *win);
void 			logo_center(t_win *win);

// void 			init_opencl(t_opencl *cl);

/*
**	debbug.c
*/

void 	 		loading_text(int samples, int y);


#endif