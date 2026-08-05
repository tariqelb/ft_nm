/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_loop_over_dym_symbols_64_util.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 21:07:25 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/08/04 21:12:11 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

char	ft_tolower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

static void	ft_get_util(t_elf64_shdr sec, char *type)
{
	if (sec.sh_type == SHT_NOBITS)
		*type = 'B';
	else if (sec.sh_flags & SHF_EXECINSTR)
		*type = 'T';
	else if (sec.sh_flags & SHF_WRITE)
		*type = 'D';
	else if (sec.sh_flags & SHF_ALLOC)
		*type = 'R';
	else
		*type = 'N';
}

char	ft_get_symbol_type64(t_table *table, t_elf64_sym *sym)
{
	char			type;
	unsigned char	sym_type;
	t_elf64_shdr	sec;

	type = '?';
	sym_type = ELF64_ST_TYPE(sym->st_info);
	if (sym->st_shndx == SHN_UNDEF)
		type = 'U';
	else if (sym->st_shndx == SHN_ABS)
		type = 'A';
	else if (sym->st_shndx == SHN_COMMON)
		type = 'C';
	else if (sym_type == STT_GNU_IFUNC)
		type = 'I';
	else if (sym->st_shndx >= SHN_LORESERVE)
		type = '?';
	else
	{
		sec = table->sections64[sym->st_shndx];
		ft_get_util(sec, &type);
	}
	return (type);
}
