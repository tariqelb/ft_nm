/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_loop_over_symbols_64_util.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 22:48:27 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/08/04 22:58:25 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

int	ft_loopover_first_check(t_table *table, t_data *data_set)
{
	close(table->fd);
	table->fd = open(table->filename, O_RDONLY);
	if (table->fd < 0)
		return (ft_display_error("ft_nm: error open file\n"));
	if (fstat(table->fd, &table->st) == -1)
		return (ft_display_error("ft_nm: error getting file size\n"));
	data_set->file = mmap(NULL, table->st.st_size,
			PROT_READ, MAP_PRIVATE, table->fd, 0);
	if (data_set->file == MAP_FAILED)
		return (ft_display_error("ft_nm: error, fail to map file\n"));
	if (table->symtab64.sh_entsize == 0
		|| table->symtab64.sh_offset + table->symtab64.sh_size
		> (long unsigned int) table->st.st_size
		|| table->strtab64.sh_offset + table->strtab64.sh_size
		> (long unsigned int) table->st.st_size)
	{
		munmap(data_set->file, table->st.st_size);
		return (ft_display_error("ft_nm: error on symtab data\n"));
	}
	return (0);
}

void	ft_sort_display_clean_data(t_table *table,
		t_output *data, t_data *data_set)
{
	munmap(data_set->file, table->st.st_size);
	data = ft_sort_output(data);
	ft_display_output(data);
	ft_clear_output(&data);
}
