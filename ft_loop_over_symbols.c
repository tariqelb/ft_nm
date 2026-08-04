/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_loop_over_symbols.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <tariqelbouhali039@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:06:08 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/04/16 05:25:38 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

//the file mapped should unmapped

int	ft_loopover_first_check(t_table *table, t_data *data_set)
{
	close(table->fd);
	table->fd = open(table->filename, O_RDONLY);
	if (table->fd < 0)
		return (ft_display_error("ft_nm: error open file\n"));
	if (fstat(table->fd, &table->st) == -1)
		return (ft_display_error("ft_nm: error getting file size\n"));
	data_set->file = mmap(NULL, table->st.st_size, PROT_READ, MAP_PRIVATE, table->fd, 0);
	if (data_set->file == MAP_FAILED)
		return (ft_display_error("ft_nm: error, fail to map file\n"));
	// safety checks
	if (table->symtab64.sh_entsize == 0 ||
		table->symtab64.sh_offset + table->symtab64.sh_size
		> (long unsigned int) table->st.st_size ||
		table->strtab64.sh_offset + table->strtab64.sh_size
		> (long unsigned int) table->st.st_size)
	{
		munmap(data_set->file, table->st.st_size);
		return (ft_display_error("ft_nm: error on symtab data\n"));
	}
	return (0);
}

int	ft_loop_over_helper_1(t_table *table, t_data *data_set, int i)
{
	data_set->sym_type = ELF64_ST_TYPE(data_set->syms64[i].st_info);
	// validate name offset
	if (data_set->syms64[i].st_name >= table->strtab64.sh_size)
		return (1);
	data_set->name = data_set->strtab_data + data_set->syms64[i].st_name;
	// ensure null-terminated
	if (!ft_memchr(data_set->name, '\0', table->strtab64.sh_size - data_set->syms64[i].st_name))
		return (1);
	if (data_set->name[0] == '\0')
		return (1);
	//ignore STT_FILE
	if (ELF64_ST_TYPE(data_set->syms64[i].st_info) == STT_SECTION)
		return (1);
	//Also ignore STT_SECTION
	if (ELF64_ST_TYPE(data_set->syms64[i].st_info) == STT_FILE)
		return (1);
	data_set->bind = ELF64_ST_BIND(data_set->syms64[i].st_info);
	return (0);
}

void	ft_loop_over_helper_2(t_table *table, t_data *data_set, int i)
{
	if (data_set->syms64[i].st_shndx == SHN_UNDEF)// 🔥 UNDEFINED
	    data_set->type = 'U';
	else if (data_set->syms64[i].st_shndx == SHN_ABS)// 🔥 ABSOLUTE
	    data_set->type = 'A';
	else if (data_set->syms64[i].st_shndx == SHN_COMMON)// 🔥 COMMON
	    data_set->type = 'C';
	else if (data_set->sym_type == STT_GNU_IFUNC)// 🔥 GNU IFUNC (indirect function)
	    data_set->type = 'I';
	else if (data_set->syms64[i].st_shndx >= SHN_LORESERVE)// 🔥 INVALID / RESERVED
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
			data_set->type = 'N'; // debug or non-alloc
	}
}

t_output	*ft_loop_over_helper_3(t_data *data_set, int i, t_output *data)
{
	// 🔥 WEAK symbols override (IMPORTANT: after base type)
	if (data_set->bind == STB_WEAK)
	{
		if (data_set->syms64[i].st_shndx == SHN_UNDEF)
			data_set->type = 'w';
		else if (data_set->sym_type == STT_OBJECT)
			data_set->type = 'V';
		else
			data_set->type = 'W';
	}
	// 🔥 LOCAL → lowercase (except special '?')
	if (data_set->bind == STB_LOCAL && data_set->type != '?')
		data_set->type = ft_tolower(data_set->type);
	ft_add_new_elem(&data, data_set->syms64[i].st_value, data_set->type, data_set->name);
	return (data);
}

void	ft_sort_display_clean_data(t_table *table, t_output *data, t_data *data_set)
{
	munmap(data_set->file, table->st.st_size);
	data = ft_sort_output(data);
	ft_display_output(data);
	ft_clear_output(&data);
}

t_output	*ft_loop_over_helper_4_64(t_table *table, t_data data_set, t_output *data)
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
	data_set.syms64 = (t_elf64_sym *)(data_set.file + table->symtab64.sh_offset);
	data_set.strtab_data = (char *)(data_set.file + table->strtab64.sh_offset);
	data = ft_loop_over_helper_4_64(table, data_set, data);
	ft_sort_display_clean_data(table, data, &data_set);
	return (0);
}
