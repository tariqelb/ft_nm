/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_header_fit_in_file.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <tariqelbouhali039@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 00:37:49 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/08/04 18:26:46 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

/*
	//check program header fit in file
	//if ....
	//check sections header fit in file
	//if ....
	//check sections header frames fit in file
	//if ....
*/

int	ft_check_elf_headers_32(t_table *table)
{
	int	err;

	err = 0;
	if (table->elf32.e_phoff + (table->elf32.e_phnum * table->elf32.e_phentsize)
		> (long unsigned int) table->st.st_size)
	{
		err = ft_display_error("ft_nm: error in program header size\n");
	}
	if (table->elf32.e_phoff == 0
		&& (table->elf32.e_type == ET_EXEC || table->elf32.e_type == ET_DYN))
		ft_display_error("ft_nm: warning, missing program header\n");
	if (table->elf32.e_shoff + (table->elf32.e_shnum * table->elf32.e_shentsize)
		> (long unsigned int) table->st.st_size)
	{
		err = ft_display_error("ft_nm: error, section headers missed\n");
	}
	if (err == 0 && ft_check_sections_headers_frames_32(table))
		err = 1;
	if (err == 0 && ft_check_strtab_and_symtab_32(table))
		err = 1;
	close (table->fd);
	if (err)
		return (1);
	return (0);
}

int	ft_check_elf_headers_64(t_table *table)
{
	int	err;

	err = 0;
	if (table->elf64.e_phoff + (table->elf64.e_phnum * table->elf64.e_phentsize)
		> (long unsigned int) table->st.st_size)
	{
		err = ft_display_error("ft_nm: error in program header size\n");
	}
	if (table->elf64.e_phoff == 0
		&& (table->elf64.e_type == ET_EXEC || table->elf64.e_type == ET_DYN))
		ft_display_error("ft_nm: warning, missing program header\n");
	if (table->elf64.e_shoff + (table->elf64.e_shnum * table->elf64.e_shentsize)
		> (long unsigned int) table->st.st_size)
	{
		err = ft_display_error("ft_nm: error, section headers missed\n");
	}
	if (err == 0 && ft_check_sections_headers_frames(table))
		err = 1;
	if (err == 0 && ft_check_strtab_and_symtab(table))
		err = 1;
	close(table->fd);
	if (err)
		return (1);
	return (0);
}
