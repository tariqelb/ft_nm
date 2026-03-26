#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "./ft_nm.h"

int	ft_loop_over_symbols(t_elf64_shdr symtab, t_elf64_shdr strtab, char *av)
{
	char *file;
	struct stat st;
	int count = symtab.sh_size / symtab.sh_entsize;
	int fd = open(av, O_RDONLY);
	fstat(fd, &st);
	file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	t_elf64_sym *syms = (t_elf64_sym *)(file + symtab.sh_offset);
	char *strtab_data = (char *)(file + strtab.sh_offset);

	for (int i = 0; i < count; i++)
	{
    		char *name = strtab_data + syms[i].st_name;
		printf("name (%s)\n", name);
	}

	return (0);
}


int	ft_get_section_headers(t_elf64_ehdr elf64, char *av)
{
	t_elf64_shdr	sections;
	printf("ft_get_section_headers, e_shnum = %d %ld\n", elf64.e_shnum, sizeof(sections));
	int		fd;
	char		buffer[elf64.e_shoff];
	int		i;
	int		flag = 0;
	t_elf64_shdr	symtab;
	t_elf64_shdr	strtab;

	i = 0;
	fd = open(av, O_RDONLY);
	lseek(fd, elf64.e_shoff, SEEK_SET);
	while (i < elf64.e_shnum)
	{
		int	rd;
		rd = read(fd, &sections, elf64.e_shentsize);
		if (sections.sh_type == 2 || sections.sh_type == 3)
		{
			printf("index, readed bytes : %d %d\n", i, rd);
			printf("name      : %d\n", sections.sh_name);
			printf("type      : [%d]\n", sections.sh_type);
			printf("flags     : %ld\n", sections.sh_flags);
			printf("addr      : %ld\n", sections.sh_addr);
			printf("offset    : %ld\n", sections.sh_offset);
			printf("size      : %ld\n", sections.sh_size);
			printf("link      : %d\n", sections.sh_link);
			printf("info      : %d\n", sections.sh_info);
			printf("addralign : %ld\n", sections.sh_addralign);
			printf("entsize   : %ld\n", sections.sh_entsize);
			printf("------------------------------------------\n");
		}
		if (sections.sh_type == 2)
		{
			symtab = sections;
			flag++;
		}
		i++;
	}
	close(fd);
	if (flag == 0)
	{
		printf("No symtab found\n");
		return (0);
	}
	if (symtab.sh_link == 0)
	{
		printf("No strtab found\n");
		return (0);
	}
	fd = open(av, O_RDONLY);
	lseek(fd, elf64.e_shoff + (elf64.e_ehsize * symtab.sh_link), SEEK_SET);
	int rd = read(fd, &strtab, elf64.e_shentsize);
	if (rd > 0)
	{
		printf("index, readed bytes : %d %d\n", i, rd);
		printf("name      : %d\n",   strtab.sh_name);
		printf("type      : [%d]\n", strtab.sh_type);
		printf("flags     : %ld\n",  strtab.sh_flags);
		printf("addr      : %ld\n",  strtab.sh_addr);
		printf("offset    : %ld\n",  strtab.sh_offset);
		printf("size      : %ld\n",  strtab.sh_size);
		printf("link      : %d\n",   strtab.sh_link);
		printf("info      : %d\n",   strtab.sh_info);
		printf("addralign : %ld\n",  strtab.sh_addralign);
		printf("entsize   : %ld\n",  strtab.sh_entsize);
		printf("------------------------------------------\n");
	}
	close(fd);	
	ft_loop_over_symbols(symtab, strtab, av);
	return (0);	
}


int	main(int ac, char **av)
{
	if (ac != 2)
		return (printf("Messing object file name argument\n"));
	int fd = open(av[1], O_RDONLY);
	
	printf("size of vars : %lu %lu %lu (%lu)\n",
		sizeof(uint16_t), sizeof(uint32_t), sizeof(uint64_t), sizeof(__uint128_t));

	t_elf64_ehdr	elf64;

	if (fd > 2)
	{
		int		i = 0;
		uint64_t	first8_ident = 0;
		uint64_t	secound8_ident = 0;
	

		read(fd, &elf64, sizeof(elf64));
		printf("size of elf64 struct   =  (%d)\n", (int) sizeof(elf64));
		printf("size of the elf header =  [%d]\n", elf64.e_ehsize);

		while (i < 16)
		{
			printf("[%d/%x]", elf64.e_ident[i], elf64.e_ident[i]);
			if (i < 8)
			{
				first8_ident = first8_ident | elf64.e_ident[i];
				if (i < 7)
					first8_ident = first8_ident << 8;
			}
			if (i >= 8)
			{
				secound8_ident = secound8_ident | elf64.e_ident[i];
				if (i < 15)
					secound8_ident = secound8_ident << 8;
			}
			i++;
			if (i == 16)
			printf("\n");
		}	
		printf("e_ident   : 0x[%lx][%lx]\n", first8_ident, secound8_ident);
		printf("type      : %d\n", elf64.e_type);
		printf("machine   : %d\n", elf64.e_machine);
		printf("version   : %d\n", elf64.e_version);
		printf("entry     : %lu\n", elf64.e_entry);
		printf("phoff     : %lu\n", elf64.e_phoff);
		printf("shoff     : %lu\n", elf64.e_shoff);
		printf("flags     : %d\n", elf64.e_flags);
		printf("ehsize    : %d\n", elf64.e_ehsize);
		printf("phentsize : %d\n", elf64.e_phentsize);
		printf("phnum     : %d\n", elf64.e_phnum);
		printf("shentsize : %d\n", elf64.e_shentsize);
		printf("shnum     : %d\n", elf64.e_shnum);
		printf("shstrndx  : %d\n", elf64.e_shstrndx);
		close(fd);
	}
	ft_get_section_headers(elf64, av[1]);	
	return (0);
}
