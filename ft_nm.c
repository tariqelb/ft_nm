/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <tariqelbouhali039@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 00:24:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/08/08 03:58:35 by tariq            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

void	ft_free(t_table table)
{
	if (table.sections32 != NULL)
		free(table.sections32);
	if (table.sections64 != NULL)
		free(table.sections32);
}

void	ft_print_new_line(int i, int ac, char **av)
{
	if (ac > 2 && i == 1)
		printf("\n");
	if (ac > 2)
		printf("%s:\n", av[i]);
}

void	ft_init(t_table *table)
{
	table->sections32 = NULL;
	table->sections64 = NULL;
	table->is_64 = 0;
}

int	main(int ac, char **av)
{
	int		err;
	int		i;
	t_table	table;

	ft_init(&table);
	i = 1;
	while (i < ac)
	{
		ft_print_new_line(i, ac, av);
		err = ft_check_errors(ac, av[i], &table);
		if (err)
		{
			ft_free(table);
			return (1);
		}
		if (table.is_64 == 1)
			ft_loop_over_symbols_32(&table);
		else
			ft_loop_over_symbols_64(&table);
		ft_free(table);
		i++;
		if (ac > 2 && i != ac)
			printf("\n");
	}
	return (0);
}
