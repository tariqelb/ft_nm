/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_loop_over_dym_symbols_32.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 18:35:46 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/08/10 22:06:40 by tariq            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h" 

static int	ft_validate_dynsym32(t_table *table)
{
	if (table->dymsym32.sh_entsize == 0)
		return (1);
	if (table->dymsym32.sh_offset + table->dymsym32.sh_size
		> (unsigned long)table->st.st_size)
		return (1);
	if (table->dymstr32.sh_offset + table->dymstr32.sh_size
		> (unsigned long)table->st.st_size)
		return (1);
	return (0);
}

static char	ft_apply_bind_rules(t_elf32_sym *sym, char type)
{
	unsigned char	bind;
	unsigned char	sym_type;

	bind = ELF32_ST_BIND(sym->st_info);
	sym_type = ELF32_ST_TYPE(sym->st_info);
	if (bind == STB_WEAK)
	{
		if (sym->st_shndx == SHN_UNDEF)
			type = 'w';
		else if (sym_type == STT_OBJECT)
			type = 'V';
		else
			type = 'W';
	}
	if (bind == STB_LOCAL && type != '?')
		type = ft_tolower(type);
	return (type);
}

static int	ft_process_symbol32(t_table *table, t_elf32_sym *sym,
			char *strtab, t_output **data)
{
	char	*name;
	char	type;

	if (sym->st_name >= table->dymstr32.sh_size)
		return (0);
	name = strtab + sym->st_name;
	if (!memchr(name, '\0',
			table->dymstr32.sh_size - sym->st_name))
		return (0);
	if (name[0] == '\0')
		return (0);
	type = ft_get_symbol_type_32(table, sym);
	type = ft_apply_bind_rules(sym, type);
	ft_add_new_elem(data, sym->st_value, type, name);
	return (0);
}

static int	ft_iter_symbols32(t_table *table, char *file)
{
	t_elf32_sym	*syms;
	char		*strtab;
	t_output	*data;
	int			count;
	int			i;

	count = table->dymsym32.sh_size / table->dymsym32.sh_entsize;
	syms = (t_elf32_sym *)(file + table->dymsym32.sh_offset);
	strtab = (char *)(file + table->dymstr32.sh_offset);
	data = NULL;
	i = 0;
	while (i < count)
	{
		ft_process_symbol32(table, &syms[i], strtab, &data);
		i++;
	}
	data = ft_sort_output(data);
	return (0);
}

int	ft_loop_over_dym_symbols_32(t_table *table)
{
	char	*file;

	table->fd = open(table->filename, O_RDONLY);
	if (table->fd < 0)
		return (1);
	fstat(table->fd, &table->st);
	file = mmap(NULL, table->st.st_size, PROT_READ,
			MAP_PRIVATE, table->fd, 0);
	if (file == MAP_FAILED)
		return (1);
	if (ft_validate_dynsym32(table))
		return (1);
	return (ft_iter_symbols32(table, file));
}
