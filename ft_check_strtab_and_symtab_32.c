/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_strtab_and_symtab_32.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <tariqelbouhali039@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 23:14:24 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/04/16 00:16:12 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

int	ft_get_symtab_dymsym_32(t_table *table, int *symtab_exist,
		int *dymsym_exist)
{
	int		i;
	ssize_t	rd;

	*symtab_exist = 0;
	*dymsym_exist = 0;
	i = 0;
	while (i < table->elf32.e_shnum)
	{
		rd = read(table->fd, &table->sections32[i], table->elf32.e_shentsize);
		if (rd == -1)
			return (ft_display_error("ft_nm: error reading file\n"));
		//ft_display_section_temp_32(table, i, rd);//temp
		if (table->sections32[i].sh_type == SHT_DYNSYM)
		{
			table->dymsym32 = table->sections32[i];
			*dymsym_exist = 1;
		}
		if (table->sections32[i].sh_type == 2)
		{
			table->symtab32 = table->sections32[i];
			*symtab_exist = 1;
		}
		i++;
	}
	return (0);
}

int	ft_get_strtab_dymstr_32(t_table *table, int symtab_exist, int dymsym_exist)
{
	if (symtab_exist == 0)
	{
		ft_display_error("ft_nm: ");
		ft_display_error(table->filename);
		ft_display_error(": no symbols\n");
		return (0);
	}
	if (symtab_exist == 1 && table->symtab32.sh_link == 0)
	{
		ft_display_error("ft_nm: error no strtab found\n");
		if (dymsym_exist == 0)
			return (1);
	}
	else if (symtab_exist)
		table->strtab32 = table->sections32[table->symtab32.sh_link];
	if (dymsym_exist == 1 && table->dymsym32.sh_link == 0)
	{
		ft_display_error("ft_nm: error no dymstr found\n");
		return (1);
	}
	else if (dymsym_exist)
	{
		table->dymstr32 = table->sections32[table->dymsym32.sh_link];
	}
	return (0);
}

int	ft_check_strtab_symtab_err_32(t_table *table)
{
	if (table->strtab32.sh_offset + table->strtab32.sh_size
		> (long unsigned int) table->st.st_size)
	{
		ft_display_error("ft_nm: erorr strtab not fit in file\n");
		return (1);
	}
	if (table->symtab32.sh_offset + table->symtab32.sh_size
		> (long unsigned int) table->st.st_size || table->symtab32.sh_entsize
		== 0 || table->symtab32.sh_size % table->symtab32.sh_entsize != 0)
	{
		ft_display_error("ft_nm: error on symtab data\n");
		return (1);
	}
	return (0);
}

int	ft_check_strtab_and_symtab_32(t_table *table)
{
	int		symtab_exist;
	int		dymsym_exist;

	table->sections32 = (t_elf32_shdr *)
		malloc(table->elf32.e_shentsize * table->elf32.e_shnum);
	lseek(table->fd, table->elf32.e_shoff, SEEK_SET);
	if (ft_get_symtab_dymsym_32(table, &symtab_exist, &dymsym_exist))
		return (1);
	if (ft_get_strtab_dymstr_32(table, symtab_exist, dymsym_exist))
		return (1);
	if (ft_check_strtab_symtab_err_32(table))
		return (1);
	close(table->fd);
	if (dymsym_exist)
	{
		ft_loop_over_dym_symbols_32(table);
	}
	return (0);
}
