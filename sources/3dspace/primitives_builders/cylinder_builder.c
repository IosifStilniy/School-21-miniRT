#include "minirt.h"

void	cylindersurfdefiner(t_poly *polys, t_cart *dots, void *txtr, int dotnum)
{
	int		i;
	int		dotindxs[3];

	i = 0;
	while (++i < RNDSGMNTS)
	{
		dotindxs[0] = i + 2;
		dotindxs[1] = RNDSGMNTS + i + 2;
		dotindxs[2] = RNDSGMNTS + i - 1 + 2;
		surfing(++polys, dotindxs, dots, txtr);
		dotindxs[1] = RNDSGMNTS + i - 1 + 2;
		dotindxs[2] = i - 1 + 2;
		surfing(++polys, dotindxs, dots, txtr);
	}
	dotindxs[0] = 2;
	dotindxs[1] = RNDSGMNTS + 2;
	dotindxs[2] = dotnum - 1;
	surfing(++polys, dotindxs, dots, txtr);
	dotindxs[1] = dotnum - 1;
	dotindxs[2] = RNDSGMNTS - 1 + 2;
	surfing(++polys, dotindxs, dots, txtr);
}

void	buttsurf(int central, int strnum, t_poly *polys, t_cart *dots)
{
	int		i;
	int		dotindxs[3];

	i = -1;
	while (++i < RNDSGMNTS - 1)
	{
		dotindxs[0] = central;
		dotindxs[1] = strnum + i + 1;
		dotindxs[2] = strnum + i;
		repairspherenormal(polys + i, dotindxs, dots, NULL);
	}
	dotindxs[0] = central;
	dotindxs[1] = strnum;
	dotindxs[2] = strnum + i;
	repairspherenormal(polys + i, dotindxs, dots, NULL);
}

float	cylinderbuilder(t_dots *dots, t_polys *polys, float radius, float height)
{
	int		i;

	dots->dotsnum = RNDSGMNTS * 2 + 2;
	polys->polynum = RNDSGMNTS * 4;
	dots->dots = malloc(sizeof(*dots->dots) * dots->dotsnum);
	cartbuilder(0, 0, -height / 2, &dots->dots[0]);
	cartbuilder(0, 0, height / 2, &dots->dots[1]);
	circledotsfiller(&dots->dots[2], radius, NULL, FALSE);
	circledotsfiller(&dots->dots[RNDSGMNTS + 2], radius, NULL, FALSE);
	i = -1;
	while (++i < RNDSGMNTS)
	{
		dots->dots[2 + i].z = -height / 2;
		dots->dots[2 + RNDSGMNTS + i].z = height / 2;
	}
	polys->poly = malloc(sizeof(*polys->poly) * polys->polynum);
	cylindersurfdefiner(polys->poly - 1, dots->dots, &polys->txtr, dots->dotsnum);
	buttsurf(0, 2, &polys->poly[2 * RNDSGMNTS], dots->dots);
	buttsurf(1, RNDSGMNTS + 2, &polys->poly[3 * RNDSGMNTS], dots->dots);
	definecylindervrtxs(dots->dots, polys->poly, polys->polynum, &polys->txtr);
	cylindermapping(dots->dots, polys->poly, polys->polynum);
	return (sqrtf(powf(height / 2, 2) + powf(radius, 2)));
}
