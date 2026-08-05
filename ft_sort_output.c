/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_output.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 18:35:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/08/05 22:07:11 by tariq            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

void	ft_swap_data(t_output *i)
{
	long int	tmp_addr;
	char		*tmp_name;
	char		tmp_type;

	tmp_addr = i->addr;
	tmp_name = i->name;
	tmp_type = i->type;
	i->addr = i->next->addr;
	i->name = i->next->name;
	i->type = i->next->type;
	i->next->addr = tmp_addr;
	i->next->name = tmp_name;
	i->next->type = tmp_type;
}

t_output	*ft_sort_output(t_output *data)
{
	t_output	*i;
	int			swapped;

	swapped = 1;
	if (!data)
		return (NULL);
	while (swapped)
	{
		swapped = 0;
		i = data;
		while (i->next)
		{
			if (ft_compare_nm(i->name, i->next->name) > 0)
			{
				ft_swap_data(i);
				swapped = 1;
			}
			i = i->next;
		}
	}
	return (data);
}
