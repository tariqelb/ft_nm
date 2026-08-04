/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_loop_over_symbols_32.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <tariqelbouhali039@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 01:04:56 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/04/16 05:26:58 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

#include "./ft_nm.h"
//the file mapped should unmapped
int	ft_loopover_first_check_32(t_table *table, t_data *data_set)
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
	if (table->symtab32.sh_entsize == 0 ||
		table->symtab32.sh_offset + table->symtab32.sh_size
		> (long unsigned int) table->st.st_size ||
		table->strtab32.sh_offset + table->strtab32.sh_size
		> (long unsigned int) table->st.st_size)
	{
		munmap(data_set->file, table->st.st_size);
		return (ft_display_error("ft_nm: error on symtab data\n"));
	}
	return (0);
}

int	ft_loop_over_helper_1_32(t_table *table, t_data *data_set, int i)
{
	data_set->sym_type = ELF32_ST_TYPE(data_set->syms32[i].st_info);
	// validate name offset
	if (data_set->syms32[i].st_name >= table->strtab32.sh_size)
		return (1);
	data_set->name = data_set->strtab_data + data_set->syms32[i].st_name;
	// ensure null-terminated
	if (!ft_memchr(data_set->name, '\0', table->strtab32.sh_size - data_set->syms32[i].st_name))
		return (1);
	if (data_set->name[0] == '\0')
		return (1);
	//ignore STT_FILE
	if (ELF32_ST_TYPE(data_set->syms32[i].st_info) == STT_SECTION)
		return (1);
	//Also ignore STT_SECTION
	if (ELF32_ST_TYPE(data_set->syms32[i].st_info) == STT_FILE)
		return (1);
	data_set->bind = ELF32_ST_BIND(data_set->syms32[i].st_info);
	return (0);
}

void	ft_loop_over_helper_2_32(t_table *table, t_data *data_set, int i)
{
	if (data_set->syms32[i].st_shndx == SHN_UNDEF)// 🔥 UNDEFINED
	    data_set->type = 'U';
	else if (data_set->syms32[i].st_shndx == SHN_ABS)// 🔥 ABSOLUTE
	    data_set->type = 'A';
	else if (data_set->syms32[i].st_shndx == SHN_COMMON)// 🔥 COMMON
	    data_set->type = 'C';
	else if (data_set->sym_type == STT_GNU_IFUNC)// 🔥 GNU IFUNC (indirect function)
	    data_set->type = 'I';
	else if (data_set->syms32[i].st_shndx >= SHN_LORESERVE)// 🔥 INVALID / RESERVED
	    data_set->type = '?';
	else
	{
		data_set->sec32 = table->sections32[data_set->syms32[i].st_shndx];
		if (data_set->sec32.sh_type == SHT_NOBITS)
		    data_set->type = 'B';
		else if (data_set->sec32.sh_flags & SHF_EXECINSTR)
		    data_set->type = 'T';
		else if (data_set->sec32.sh_flags & SHF_WRITE)
		    data_set->type = 'D';
		else if (data_set->sec32.sh_flags & SHF_ALLOC)
		    data_set->type = 'R';
		else
			data_set->type = 'N'; // debug or non-alloc
	}
}

t_output	*ft_loop_over_helper_3_32(t_data *data_set, int i, t_output *data)
{
	// 🔥 WEAK symbols override (IMPORTANT: after base type)
	if (data_set->bind == STB_WEAK)
	{
		if (data_set->syms32[i].st_shndx == SHN_UNDEF)
			data_set->type = 'w';
		else if (data_set->sym_type == STT_OBJECT)
			data_set->type = 'V';
		else
			data_set->type = 'W';
	}
	// 🔥 LOCAL → lowercase (except special '?')
	if (data_set->bind == STB_LOCAL && data_set->type != '?')
		data_set->type = ft_tolower(data_set->type);
	ft_add_new_elem(&data, data_set->syms32[i].st_value, data_set->type, data_set->name);
	return (data);
}

void	ft_sort_display_clean_data_32(t_table *table, t_output *data, t_data *data_set)
{
	munmap(data_set->file, table->st.st_size);
	data = ft_sort_output(data);
	ft_display_output(data);
	ft_clear_output(&data);
}

t_output        *ft_loop_over_helper_4_32(t_table *table, t_data data_set, t_output *data) 
{
	int	i;

	i = 0;
	while (i < data_set.count)
	{
		if (ft_loop_over_helper_1_32(table, &data_set, i) == 0)
		{
			ft_loop_over_helper_2_32(table, &data_set, i);
			data = ft_loop_over_helper_3_32(&data_set, i, data);
		}
		i++;
	}
        return (data);
}

int	ft_loop_over_symbols_32(t_table *table)
{
	t_data		data_set;
	t_output	*data;

	data = NULL;
	data_set.count = 0;
	if (ft_loopover_first_check_32(table, &data_set))
		return (1);
	data_set.count = table->symtab32.sh_size / table->symtab32.sh_entsize;
	if (data_set.count == 0)
	{
		munmap(data_set.file, table->st.st_size);
		return (0);
	}
	data_set.syms32 = (t_elf32_sym *)(data_set.file + table->symtab32.sh_offset);
	data_set.strtab_data = (char *)(data_set.file + table->strtab32.sh_offset);
	ft_loop_over_helper_4_32(table, data_set, data);
	ft_sort_display_clean_data_32(table, data, &data_set);
	return (0);
}

/*
int	ft_loop_over_symbols_32(t_table *table)
{
	char		*file;
	t_output	*data;
	unsigned char   sym_type;

	table->fd = open(table->filename, O_RDONLY);
	if (table->fd < 0)
		return (1);

	fstat(table->fd, &table->st);
	file = mmap(NULL, table->st.st_size, PROT_READ, MAP_PRIVATE, table->fd, 0);

	if (file == MAP_FAILED)
		return (1);

	// safety checks
	if (table->symtab32.sh_entsize == 0 ||
		table->symtab32.sh_offset + table->symtab32.sh_size > (long unsigned int) table->st.st_size ||
		table->strtab32.sh_offset + table->strtab32.sh_size > (long unsigned int) table->st.st_size)
		return (1);

	int count = table->symtab32.sh_size / table->symtab32.sh_entsize;
	
	data = NULL;

	t_elf32_sym *syms = (t_elf32_sym *)(file + table->symtab32.sh_offset);
	char *strtab_data = (char *)(file + table->strtab32.sh_offset);

	for (int i = 0; i < count; i++)
	{
		sym_type = ELF32_ST_TYPE(syms[i].st_info);
	    // validate name offset
		if (syms[i].st_name >= table->strtab32.sh_size)
			continue;

		char *name = strtab_data + syms[i].st_name;

		// ensure null-terminated
		if (!memchr(name, '\0', table->strtab32.sh_size - syms[i].st_name))
		    continue;

		if (name[0] == '\0')
		    continue;

		char type;
		unsigned char bind = ELF32_ST_BIND(syms[i].st_info);

		// 🔥 UNDEFINED
		if (syms[i].st_shndx == SHN_UNDEF)
		    type = 'U';

		// 🔥 ABSOLUTE
		else if (syms[i].st_shndx == SHN_ABS)
		    type = 'A';

		// 🔥 COMMON
		else if (syms[i].st_shndx == SHN_COMMON)
		    type = 'C';

		// 🔥 GNU IFUNC (indirect function)
		else if (sym_type == STT_GNU_IFUNC)
		    type = 'I';

		// 🔥 INVALID / RESERVED
		else if (syms[i].st_shndx >= SHN_LORESERVE)
		    type = '?';

		else
		{
			t_elf32_shdr sec = table->sections32[syms[i].st_shndx];
			if (sec.sh_type == SHT_NOBITS)
			    type = 'B';
			else if (sec.sh_flags & SHF_EXECINSTR)
			    type = 'T';
			else if (sec.sh_flags & SHF_WRITE)
			    type = 'D';
			else if (sec.sh_flags & SHF_ALLOC)
			    type = 'R';
			else
				type = 'N'; // debug or non-alloc
		}

		// 🔥 WEAK symbols override (IMPORTANT: after base type)
		if (bind == STB_WEAK)
		{
			if (syms[i].st_shndx == SHN_UNDEF)
				type = 'w';
			else if (sym_type == STT_OBJECT)
				type = 'V';
			else
				type = 'W';
		}

		// 🔥 LOCAL → lowercase (except special '?')
		if (bind == STB_LOCAL && type != '?')
			type = ft_tolower(type);
		ft_add_new_elem(&data, syms[i].st_value, type, name);
		//printf("%016x %c %s\n", syms[i].st_value, type, name);
	}
	data = ft_sort_output(data);
	ft_display_output(data);
	ft_clear_output(&data);
	return (0);
}
*/
