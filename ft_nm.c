#include "./ft_nm.h"



int	main(int ac, char **av)
{
	int	err;
	int	is_64;

	is_64 = 0;	
	err = ft_check_errors(ac, av, &is_64);
	if (err)
		return (1);

}
