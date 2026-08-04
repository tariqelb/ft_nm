/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_errors_two.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <tariqelbouhali039@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 00:19:25 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/04/15 02:20:00 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

int	ft_get_magin_helper(uint64_t *first8_ident, uint64_t *second8_ident,
		t_elf64_ehdr elf64, int i)
{
	const long unsigned int	magic_number = 2135247942;

	if (i < 8)
	{
		*first8_ident = *first8_ident | elf64.e_ident[i];
		if (i == 3)
		{
			if (magic_number != *first8_ident)
				return (1);
		}
		if (i < 7)
			*first8_ident = *first8_ident << 8;
	}
	if (i >= 8)
	{
		*second8_ident = *second8_ident | elf64.e_ident[i];
		if (i < 15)
			*second8_ident = *second8_ident << 8;
	}
	return (0);
}

int	ft_get_check_elf_header_magic_nbr(t_elf64_ehdr elf64)
{
	int			i;
	uint64_t	first8_ident;
	uint64_t	second8_ident;

	first8_ident = 0;
	second8_ident = 0;
	i = 0;
	while (i < 16)
	{
		if (ft_get_magin_helper(&first8_ident, &second8_ident, elf64, i))
			return (ft_display_error("ft_nm: error invalid magic nbr\n"));
		i++;
	}
	if (elf64.e_ident[4] != ELFCLASS32 && elf64.e_ident[4] != ELFCLASS64)
		return (1);
	if (elf64.e_ident[5] != ELFDATA2LSB && elf64.e_ident[5] != ELFDATA2MSB)
		return (1);
	if (elf64.e_ident[6] != EV_CURRENT)
		return (1);
	return (0);
}

int	ft_check_magic_number_and_ehdr_size(t_table *table)
{
	ssize_t		rd;

	if (fstat(table->fd, &table->st) == -1)
	{
		close(table->fd);
		return (ft_display_error("ft_nm: error getting file size\n"));
	}
	if (table->st.st_size < (long int) sizeof(t_elf64_ehdr))
	{
		close(table->fd);
		return (ft_display_error("ft_nm: error reading elf header\n"));
	}
	rd = read(table->fd, &table->elf64, sizeof(t_elf64_ehdr));
	if (rd != sizeof(t_elf64_ehdr))
	{
		close(table->fd);
		return (ft_display_error("ft_nm: error reading elf header\n"));
	}
	table->is_64 = table->elf64.e_ident[4];
	if (ft_get_check_elf_header_magic_nbr(table->elf64))
	{
		close(table->fd);
		return (ft_display_error("ft_nm: error, not an elf file\n"));
	}
	return (0);
}

int	ft_lseek_read_ehdr_32(t_table *table)
{
	ssize_t	rd;

	if (lseek(table->fd, 0, SEEK_SET) == -1)
	{
		close(table->fd);
		return (ft_display_error("ft_nm: lseek failed\n"));
	}
	rd = read(table->fd, &table->elf32, sizeof(t_elf32_ehdr));
	if (rd != sizeof(t_elf32_ehdr))
	{
		close(table->fd);
		return (ft_display_error("ft_nm: error reading elf header\n"));
	}
	return (0);
}

int	ft_check_if_its_elf_file_and_hdrs_exist(t_table *table)
{
	table->fd = open(table->filename, O_RDONLY);
	if (table->fd < 3)
	{
		ft_display_error("ft_nm: ");
		perror(table->filename);
		return (1);
	}
	if (ft_check_magic_number_and_ehdr_size(table))
		return (1);
	if (table->is_64 == 1)
	{
		if (ft_lseek_read_ehdr_32(table))
			return (1);
		if (ft_check_elf_headers_32(table))
			return (1);
	}
	else if (ft_check_elf_headers_64(table))
		return (1);
	close(table->fd);
	return (0);
}
