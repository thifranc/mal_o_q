#include <stdio.h>

int main()
{
	int get_p_size = 4096;
	int t_size = 90;

	int min_page = (((100 * t_size) / get_p_size) + 1) * get_p_size;
	dprintf(1, "min_page_plus_1 %d\n", min_page);
	int rab = min_page - (100 * t_size);
	int bonus = rab / t_size;
	int perte = rab % t_size;
	dprintf(1, "rab %d, bonus = %d, perte = %d\n", rab, bonus, perte);
	return 0;
}
