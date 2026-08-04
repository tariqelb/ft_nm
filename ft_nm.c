/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <tariqelbouhali039@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 00:24:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/04/16 02:52:38 by tel-bouh         ###   ########.fr       */
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

int	main(int ac, char **av)
{
	int		err;
	t_table	table;

	table.sections32 = NULL;
	table.sections64 = NULL;
	table.is_64 = 0;
	err = ft_check_errors(ac, av, &table);
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
	return (0);
}
