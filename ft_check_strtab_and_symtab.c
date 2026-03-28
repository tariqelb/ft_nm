#include "./ft_nm.h"

int ft_check_strtab_and_symtab(t_table *table)
{
	int		i;
	int     	rd;
	int		strtab_exist;
	int		symtab_exist;
	t_elf64_shdr	symtab;
	t_elf64_shdr	strtab;
	t_elf64_shdr	*sections;

	table->sections64 = (t_elf64_shdr *) malloc(table->elf64.e_shentsize * table->elf64.e_shnum);	
	strtab_exist = 0;
	symtab_exist = 0;
	lseek(fd, table->elf64.e_shoff, SEEK_SET);	
	i = 0;
	while (i < table->elf64.e_shnum)
	{
		rd = read(table->fd, &table->sections64[i], table->elf64.e_shentsize);
		if (table->sections64[i].sh_type == 2 || table->sections64[i].sh_type == 3)
		{
			printf("index, readed bytes : %d %d\n", i, rd);
			printf("name      : %d\n",   table->sections64[i].sh_name);
			printf("type      : [%d]\n", table->sections64[i].sh_type);
			printf("flags     : %ld\n",  table->sections64[i].sh_flags);
			printf("addr      : %ld\n",  table->sections64[i].sh_addr);
			printf("offset    : %ld\n",  table->sections64[i].sh_offset);
			printf("size      : %ld\n",  table->sections64[i].sh_size);
			printf("link      : %d\n",   table->sections64[i].sh_link);
			printf("info      : %d\n",   table->sections64[i].sh_info);
			printf("addralign : %ld\n",  table->sections64[i].sh_addralign);
			printf("entsize   : %ld\n",  table->sections64[i].sh_entsize);
			printf("------------------------------------------\n");
		}
		if (table->sections64[i].sh_type == 2)
		{
			table->symtab64 = table->sections64[i];
			symtab_exist++;
		}
		i++;
	}
	if (symtab_exist == 0)
	{
 		if (table->elf64.e_type == ET_EXEC || table->elf64.e_type == ET_DYN)
        		return (0);
		ft_display_error("ft_nm: ");
		ft_display_error(table->filename);
		ft_display_error(": no symbols\n");
		return (1);
	}
	if (table->symtab64.sh_link == 0)
	{
		ft_display_error("ft_nm: error no strtab found\n");
		return (1);
	}
	strtab_exist++;
	printf("Strtab symtab data:\n");
	printf("sym exist %d str exist %d, fd : %d, \n",
		symtab_exist, strtab_exist, table->fd);
	table->strtab64 = table->sections64[tablesymtab64.sh_link];
	//lseek(fd, elf64.e_shoff + (elf64.e_ehsize * symtab.sh_link), SEEK_SET);
	//rd = read(fd, &strtab, elf64.e_shentsize);
	printf("rd %d\n", rd);
	if (table->strtab64.sh_offset + table->strtab64.sh_size > table->st.st_size)
	{
		ft_display_error("ft_nm: erorr strtab not fit in file\n");
		return (1);
	}
	if (table->symtab64.sh_offset + table->symtab64.sh_size > table->st.st_size
		|| table->symtab64.sh_entsize == 0 || table->symtab64.sh_size % table->symtab64.sh_entsize != 0)
	{
		ft_display_error("ft_nm: error on symtab data\n");
		return (1);
	}
	close(table->fd);
	ft_loop_over_symbols(table);
	return (0);
}



int ft_check_strtab_and_symtab_32(t_table *table)
{
	int		i;
	int     	rd;
	int		strtab_exist;
	int		symtab_exist;
	t_elf32_shdr	symtab;
	t_elf32_shdr	strtab;
	t_elf32_shdr	*sections;

	table->sections32 = (t_elf32_shdr *) malloc(table->elf32.e_shentsize * table->elf32.e_shnum);
	strtab_exist = 0;
	symtab_exist = 0;
	lseek(table->fd, table->elf32.e_shoff, SEEK_SET);	
	i = 0;
	while (i < table->elf32.e_shnum)
	{
		rd = read(table->fd, &table->sections64[i], table->elf32.e_shentsize);
		if (table->sections32[i].sh_type == 2 || table->sections32[i].sh_type == 3)
		{
			printf("index, readed bytes : %d %d\n", i, rd);
			printf("name      : %d\n",	table->sections32[i].sh_name);
			printf("type      : [%d]\n",	table->sections32[i].sh_type);
			printf("flags     : %d\n",	table->sections32[i].sh_flags);
			printf("addr      : %d\n",	table->sections32[i].sh_addr);
			printf("offset    : %d\n",	table->sections32[i].sh_offset);
			printf("size      : %d\n",	table->sections32[i].sh_size);
			printf("link      : %d\n",	table->sections32[i].sh_link);
			printf("info      : %d\n",	table->sections32[i].sh_info);
			printf("addralign : %d\n",	table->sections32[i].sh_addralign);
			printf("entsize   : %d\n",	table->sections32[i].sh_entsize);
			printf("------------------------------------------\n");
		}
		if (table->sections32[i].sh_type == 2)
		{
			table->symtab32 = table->sections32[i];
			symtab_exist++;
		}
		i++;
	}
	if (symtab_exist == 0)
	{
		ft_display_error("ft_nm: ");
		ft_display_error(table->filename);
		ft_display_error(": no symbols\n");
		return (1);
	}
	if (table->symtab32.sh_link == 0)
	{
		ft_display_error("ft_nm: error no strtab found\n");
		return (1);
	}
	strtab_exist++;
	printf("Strtab symtab data:\n");
	printf("sym exist %d str exist %d, fd : %d, \n",
		symtab_exist, strtab_exist, table->fd);
	table->strtab = table->sections32[symtab32.sh_link];
	//lseek(fd, elf32.e_shoff + (elf64.e_ehsize * symtab.sh_link), SEEK_SET);
	//rd = read(fd, &strtab, elf32.e_shentsize);
	printf("rd %d\n", rd);
	if (table->strtab32.sh_offset + table->strtab32.sh_size > table->st.st_size)
	{
		ft_display_error("ft_nm: erorr strtab not fit in file\n");
		return (1);
	}
	if (table->symtab32.sh_offset + table->symtab32.sh_size > table->st.st_size
		|| table->symtab32.sh_entsize == 0 || table->symtab32.sh_size % table->symtab32.sh_entsize != 0)
	{
		ft_display_error("ft_nm: error on symtab data\n");
		return (1);
	}
	close(table->fd);
	ft_loop_over_symbols_32(table);
	return (0);
}
