#include "./ft_nm.h"



int	main(int ac, char **av)
{
	int	err;
	t_table	table;

	table.is_64 = 0;	
	err = ft_check_errors(ac, av, &table);
	if (err)
		return (1);

}
