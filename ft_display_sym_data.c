/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display_sym_data.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <tariqelbouhali039@gmail.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 04:22:41 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/04/15 23:47:37 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_nm.h"

void	ft_display_section_temp_32(t_table *table, int i, int rd)
{
	if (table->sections32[i].sh_type == 2 || table->sections32[i].sh_type == 3
		|| table->sections32[i].sh_type == SHT_DYNSYM)
	{
		printf("index, readed bytes : %d %d\n", i, rd);
		printf("name      : %d\n", table->sections32[i].sh_name);
		printf("type      : [%d]\n", table->sections32[i].sh_type);
		printf("flags     : %d\n", table->sections32[i].sh_flags);
		printf("addr      : %d\n", table->sections32[i].sh_addr);
		printf("offset    : %d\n", table->sections32[i].sh_offset);
		printf("size      : %d\n", table->sections32[i].sh_size);
		printf("link      : %d\n", table->sections32[i].sh_link);
		printf("info      : %d\n", table->sections32[i].sh_info);
		printf("addralign : %d\n", table->sections32[i].sh_addralign);
		printf("entsize   : %d\n", table->sections32[i].sh_entsize);
		printf("------------------------------------------\n");
	}
}

void	ft_display_section_temp_64(t_table *table, int i, int rd)
{
	if (table->sections64[i].sh_type == 2 || table->sections64[i].sh_type == 3
		|| table->sections64[i].sh_type == SHT_DYNSYM)
	{
		printf("index, readed bytes : %d %d\n", i, rd);
		printf("name      : %d \n", table->sections64[i].sh_name);
		printf("type      : [%d]\n", table->sections64[i].sh_type);
		printf("flags     : %ld\n", table->sections64[i].sh_flags);
		printf("addr      : %ld\n", table->sections64[i].sh_addr);
		printf("offset    : %ld\n", table->sections64[i].sh_offset);
		printf("size      : %ld\n", table->sections64[i].sh_size);
		printf("link      : %d \n", table->sections64[i].sh_link);
		printf("info      : %d \n", table->sections64[i].sh_info);
		printf("addralign : %ld\n", table->sections64[i].sh_addralign);
		printf("entsize   : %ld\n", table->sections64[i].sh_entsize);
		printf("------------------------------------------\n");
	}
}
