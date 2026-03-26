#include "./ft_nm.h"


//temp fun

int	ft_loop_over_symbols(t_elf64_shdr symtab, t_elf64_shdr strtab, t_elf64_shdr *sections, char *av)
{
	char *file;
	struct stat st;
	int fd = open(av, O_RDONLY);
	t_output	*data;

	if (fd < 0)
		return (1);

	fstat(fd, &st);
	file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if (file == MAP_FAILED)
		return (1);

	// safety checks
	if (symtab.sh_entsize == 0 ||
		symtab.sh_offset + symtab.sh_size > st.st_size ||
		strtab.sh_offset + strtab.sh_size > st.st_size)
		return (1);

	int count = symtab.sh_size / symtab.sh_entsize;
	
	data = NULL;

	t_elf64_sym *syms = (t_elf64_sym *)(file + symtab.sh_offset);
	char *strtab_data = (char *)(file + strtab.sh_offset);

	for (int i = 0; i < count; i++)
	{
	    // validate name offset
		if (syms[i].st_name >= strtab.sh_size)
			continue;

		char *name = strtab_data + syms[i].st_name;

		// ensure null-terminated
		if (!memchr(name, '\0', strtab.sh_size - syms[i].st_name))
		    continue;

		if (name[0] == '\0')
		    continue;

		char type;

		// 🔥 UNDEFINED
		if (syms[i].st_shndx == SHN_UNDEF)
			type = 'U';

		// 🔥 INVALID index protection
		else if (syms[i].st_shndx >= SHN_LORESERVE)
			type = '?';

		else
		{
			 t_elf64_shdr sec = sections[syms[i].st_shndx];

			if (sec.sh_type == SHT_NOBITS)
				type = 'B';
			else if (sec.sh_flags & SHF_EXECINSTR)
				type = 'T';
			else if (sec.sh_flags & SHF_WRITE)
				type = 'D';
			else
				type = 'R';
		}
		ft_add_new_elem(&data, syms[i].st_value, type, name);
		printf("%016lx %c %s\n", syms[i].st_value, type, name);
	}
	data = ft_sort_output(data);
	ft_display_output(data);
	free(data);
	return (0);
}


int	main(int ac, char **av)
{
	int	err;
	
	err = ft_check_errors(ac, av);
	if (err)
		return (1);

}
