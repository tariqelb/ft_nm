/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_loop_over_dym_symbols_32_util.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 20:24:28 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/08/04 20:50:29 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

static void	ft_get_util(t_elf32_shdr sec, char *type)
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

char	ft_get_symbol_type_32(t_table *table, t_elf32_sym *sym)
{
	char			type;
	unsigned char	sym_type;
	t_elf32_shdr	sec;

	type = '?';
	sym_type = ELF32_ST_TYPE(sym->st_info);
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
		sec = table->sections32[sym->st_shndx];
		ft_get_util(sec, &type);
	}
	return (type);
}
