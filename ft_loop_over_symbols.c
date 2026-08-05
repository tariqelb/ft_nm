/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_loop_over_symbols.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <tariqelbouhali039@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:06:08 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/08/04 22:55:03 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

/*the file mapped should unmapped*/

int	ft_loop_over_helper_1(t_table *table, t_data *data_set, int i)
{
	data_set->sym_type = ELF64_ST_TYPE(data_set->syms64[i].st_info);
	if (data_set->syms64[i].st_name >= table->strtab64.sh_size)
		return (1);
	data_set->name = data_set->strtab_data + data_set->syms64[i].st_name;
	if (!ft_memchr(data_set->name, '\0',
			table->strtab64.sh_size - data_set->syms64[i].st_name))
		return (1);
	if (data_set->name[0] == '\0')
		return (1);
	if (ELF64_ST_TYPE(data_set->syms64[i].st_info) == STT_SECTION)
		return (1);
	if (ELF64_ST_TYPE(data_set->syms64[i].st_info) == STT_FILE)
		return (1);
	data_set->bind = ELF64_ST_BIND(data_set->syms64[i].st_info);
	return (0);
}

void	ft_loop_over_helper_2(t_table *table, t_data *data_set, int i)
{
	if (data_set->syms64[i].st_shndx == SHN_UNDEF)
		data_set->type = 'U';
	else if (data_set->syms64[i].st_shndx == SHN_ABS)
		data_set->type = 'A';
	else if (data_set->syms64[i].st_shndx == SHN_COMMON)
		data_set->type = 'C';
	else if (data_set->sym_type == STT_GNU_IFUNC)
		data_set->type = 'I';
	else if (data_set->syms64[i].st_shndx >= SHN_LORESERVE)
		data_set->type = '?';
	else
	{
		data_set->sec64 = table->sections64[data_set->syms64[i].st_shndx];
		if (data_set->sec64.sh_type == SHT_NOBITS)
			data_set->type = 'B';
		else if (data_set->sec64.sh_flags & SHF_EXECINSTR)
			data_set->type = 'T';
		else if (data_set->sec64.sh_flags & SHF_WRITE)
			data_set->type = 'D';
		else if (data_set->sec64.sh_flags & SHF_ALLOC)
			data_set->type = 'R';
		else
			data_set->type = 'N';
	}
}

t_output	*ft_loop_over_helper_3(t_data *data_set, int i, t_output *data)
{
	if (data_set->bind == STB_WEAK)
	{
		if (data_set->syms64[i].st_shndx == SHN_UNDEF)
			data_set->type = 'w';
		else if (data_set->sym_type == STT_OBJECT)
			data_set->type = 'V';
		else
			data_set->type = 'W';
	}
	if (data_set->bind == STB_LOCAL && data_set->type != '?')
		data_set->type = ft_tolower(data_set->type);
	ft_add_new_elem(&data, data_set->syms64[i].st_value,
		data_set->type, data_set->name);
	return (data);
}

t_output	*ft_loop_over_helper_4_64(t_table *table,
		t_data data_set, t_output *data)
{
	int	i;

	i = 0;
	while (i < data_set.count)
	{
		if (ft_loop_over_helper_1(table, &data_set, i) == 0)
		{
			ft_loop_over_helper_2(table, &data_set, i);
			data = ft_loop_over_helper_3(&data_set, i, data);
		}
		i++;
	}
	return (data);
}

int	ft_loop_over_symbols_64(t_table *table)
{
	t_data		data_set;
	t_output	*data;

	data = NULL;
	data_set.count = 0;
	if (ft_loopover_first_check(table, &data_set))
		return (1);
	data_set.count = table->symtab64.sh_size / table->symtab64.sh_entsize;
	if (data_set.count == 0)
	{
		munmap(data_set.file, table->st.st_size);
		return (0);
	}
	data_set.syms64 = (t_elf64_sym *)(data_set.file
			+ table->symtab64.sh_offset);
	data_set.strtab_data = (char *)(data_set.file + table->strtab64.sh_offset);
	data = ft_loop_over_helper_4_64(table, data_set, data);
	ft_sort_display_clean_data(table, data, &data_set);
	return (0);
}
