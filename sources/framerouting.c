#include "minirt.h"

t_bool	checkrout(int dot1, int dot2, t_list *routs)
{
	t_bool	exist;
	int		*rout;

	while (routs)
	{
		rout = routs->content;
		exist = (rout[0] == dot1 || rout[0] == dot2);
		exist += (rout[1] == dot1 || rout[1] == dot2);
		if (exist)
			return (TRUE);
	}
	return (FALSE);
}

void	createroutlstelem(int dot1, int dot2, t_list **routs)
{
	int		*rout;
	
	rout = malloc(sizeof(*rout) * 2);
	rout[0] = dot1;
	rout[1] = dot2;
	ft_lstadd_front(routs, ft_lstnew(rout));
}

void	createroutlist(t_poly *polys, int polynum, t_list **routs)
{
	int	*dots;
	int	i;

	i = -1;
	while (++i < polynum)
	{
		dots = polys[i].dots;
		if (!checkrout(dots[0], dots[1], *routs))
			createroutlstelem(dots[0], dots[1], routs);
		if (!checkrout(dots[1], dots[2], *routs))
			createroutlstelem(dots[1], dots[2], routs);
		if (!checkrout(dots[2], dots[0], *routs))
			createroutlstelem(dots[2], dots[0], routs);
	}
}

int	lsttointarr(t_list *routs, int **routarr)
{
	int	size;
	int	i;

	size = ft_lstsize(routs);
	*routarr = malloc(sizeof(**routarr) * size);
	i = -1;
	while (++i < size)
	{
		(*routarr)[i][0] = 1;
		(*routarr)[i][0] = routs->content[0];
		(*routarr)[i][1] = routs->content[1];
		routs = routs->content;
	}
	return (size);
}

void	createframerouts(t_list *objs)
{
	t_list	*routs;
	t_obj	*obj;

	while (objs)
	{
		obj = objs->content;
		routs = NULL;
		createroutlist(obj->polys.poly, obj->polys.polynum, &routs);
		obj->dots.routsize = lsttointarr(routs, &obj->dots.rout);
		ft_lstclear(&rout, &free);
		objs = objs->next;
	}
}