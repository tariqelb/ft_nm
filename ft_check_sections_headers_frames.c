/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_sections_headers_frames.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <tariqelbouhali039@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 01:27:12 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/04/15 01:48:56 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

int	ft_check_sections_headers_frames(t_table *table)
{
	int				i;
	int				rd;
	t_elf64_shdr	section;

	i = 0;
	lseek(table->fd, table->elf64.e_shoff, SEEK_SET);
	i = 0;
	while (i < table->elf64.e_shnum)
	{
		rd = read(table->fd, &section, table->elf64.e_shentsize);
		if (rd != (int)table->elf64.e_shentsize)
			return (ft_display_error("ft_nm: error reading section header\n"));
		if (section.sh_offset > (long unsigned int) table->st.st_size
			|| section.sh_size > (long unsigned int) table->st.st_size
			|| section.sh_offset > table->st.st_size - section.sh_size)
		{
			return (ft_display_error("ft_nm: corrupted section\n"));
		}
		i++;
	}
	return (0);
}

int	ft_check_sections_headers_frames_32(t_table *table)
{
	int				i;
	int				rd;
	t_elf32_shdr	section;

	i = 0;
	lseek(table->fd, table->elf32.e_shoff, SEEK_SET);
	while (i < table->elf32.e_shnum)
	{
		rd = read(table->fd, &section, table->elf32.e_shentsize);
		if (rd != (int)table->elf32.e_shentsize)
			return (ft_display_error("ft_nm: error reading section header\n"));
		if (section.sh_offset > (long unsigned int) table->st.st_size
			|| section.sh_size > (long unsigned int) table->st.st_size
			|| section.sh_offset > table->st.st_size - section.sh_size)
		{
			return (ft_display_error("ft_nm: corrupted section\n"));
		}
		i++;
	}
	return (0);
}
