#include "./ft_nm.h"

int ft_check_strtab_and_symtab(int fd, t_elf64_ehdr elf64, size_t file_size)
{
	int		i;
	int     	rd;
	int		strtab_exist;
	int		symtab_exist;
	t_elf64_shdr	symtab;
	t_elf64_shdr	strtab;
	t_elf64_shdr	*sections;

	sections = (t_elf64_shdr *) malloc(elf64.e_shentsize * elf64.e_shnum);
	strtab_exist = 0;
	symtab_exist = 0;
	lseek(fd, elf64.e_shoff, SEEK_SET);	
	i = 0;
	while (i < elf64.e_shnum)
	{
		rd = read(fd, &sections[i], elf64.e_shentsize);
		if (sections[i].sh_type == 2 || sections[i].sh_type == 3)
		{
			printf("index, readed bytes : %d %d\n", i, rd);
                         printf("name      : %d\n",   sections[i].sh_name);
                         printf("type      : [%d]\n", sections[i].sh_type);
                         printf("flags     : %ld\n",  sections[i].sh_flags);
                         printf("addr      : %ld\n",  sections[i].sh_addr);
                         printf("offset    : %ld\n",  sections[i].sh_offset);
                         printf("size      : %ld\n",  sections[i].sh_size);
                         printf("link      : %d\n",   sections[i].sh_link);
                         printf("info      : %d\n",   sections[i].sh_info);
                         printf("addralign : %ld\n",  sections[i].sh_addralign);
                         printf("entsize   : %ld\n",  sections[i].sh_entsize);
                         printf("------------------------------------------\n");
		}
		if (sections[i].sh_type == 2)
		{
			symtab = sections[i];
			symtab_exist++;
		}
		i++;
	}
	if (symtab_exist == 0)
	{
		ft_display_error("ft_nm: error no symtab found\n");
		return (1);
	}
	if (symtab.sh_link == 0)
	{
		ft_display_error("ft_nm: error no strtab found\n");
		return (1);
	}
	strtab_exist++;
	printf("Strtab symtab data:\n");
	printf("sym exist %d str exist %d, fd : %d, \n",
		symtab_exist, strtab_exist, fd);
	strtab = sections[symtab.sh_link];
	//lseek(fd, elf64.e_shoff + (elf64.e_ehsize * symtab.sh_link), SEEK_SET);
	//rd = read(fd, &strtab, elf64.e_shentsize);
	printf("rd %d\n", rd);
	if (strtab.sh_offset + strtab.sh_size > file_size)
	{
		ft_display_error("ft_nm: erorr strtab not fit in file\n");
		return (1);
	}
	if (symtab.sh_offset + symtab.sh_size > file_size
		|| symtab.sh_entsize == 0 || symtab.sh_size % symtab.sh_entsize != 0)
	{
		ft_display_error("ft_nm: error on symtab data\n");
		return (1);
	}
	close(fd);
	ft_loop_over_symbols(symtab, strtab, sections, "read_elf_header.o");
	return (0);
}
