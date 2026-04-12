#include "./ft_nm.h"

char    ft_tolower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

int	ft_loop_over_dym_symbols_32(t_table *table)
{
	char		*file;
	struct stat	st;
	t_output	*data;
	unsigned char   sym_type;

	table->fd = open(table->filename, O_RDONLY);
	if (table->fd < 0)
		return (1);

	fstat(table->fd, &table->st);
	file = mmap(NULL, table->st.st_size, PROT_READ, MAP_PRIVATE, table->fd, 0);

	if (file == MAP_FAILED)
		return (1);

	// safety checks
	if (table->dymsym32.sh_entsize == 0 ||
		table->dymsym32.sh_offset + table->dymsym32.sh_size > table->st.st_size ||
		table->dymstr32.sh_offset + table->dymstr32.sh_size > table->st.st_size)
		return (1);

	int count = table->dymsym32.sh_size / table->dymsym32.sh_entsize;
	
	data = NULL;

	t_elf32_sym *syms = (t_elf32_sym *)(file + table->dymsym32.sh_offset);
	char *strtab_data = (char *)(file + table->dymstr32.sh_offset);

	for (int i = 0; i < count; i++)
	{
	    // validate name offset
		sym_type = ELF32_ST_TYPE(syms[i].st_info);
		if (syms[i].st_name >= table->dymstr32.sh_size)
			continue;

		char *name = strtab_data + syms[i].st_name;

		// ensure null-terminated
		if (!memchr(name, '\0', table->dymstr32.sh_size - syms[i].st_name))
		    continue;

		if (name[0] == '\0')
		    continue;

		char type = '?';
		unsigned char bind = ELF32_ST_BIND(syms[i].st_info);
		// 🔥 UNDEFINED
		if (syms[i].st_shndx == SHN_UNDEF)
		    type = 'U';

		// 🔥 ABSOLUTE
		else if (syms[i].st_shndx == SHN_ABS)
		    type = 'A';

		// 🔥 COMMON
		else if (syms[i].st_shndx == SHN_COMMON)
		    type = 'C';

		// 🔥 GNU IFUNC (indirect function)
		else if (sym_type == STT_GNU_IFUNC)
		    type = 'I';

		// 🔥 INVALID / RESERVED
		else if (syms[i].st_shndx >= SHN_LORESERVE)
		    type = '?';

		else
		{
			t_elf64_shdr sec = table->sections64[syms[i].st_shndx];
			if (sec.sh_type == SHT_NOBITS)
			    type = 'B';
			else if (sec.sh_flags & SHF_EXECINSTR)
			    type = 'T';
			else if (sec.sh_flags & SHF_WRITE)
			    type = 'D';
			else if (sec.sh_flags & SHF_ALLOC)
			    type = 'R';
			else
				type = 'N'; // debug or non-alloc
		}

		// 🔥 WEAK symbols override (IMPORTANT: after base type)
		if (bind == STB_WEAK)
		{
			if (syms[i].st_shndx == SHN_UNDEF)
				type = 'w';
			else if (sym_type == STT_OBJECT)
				type = 'V';
			else
				type = 'W';
		}

		// 🔥 LOCAL → lowercase (except special '?')
		if (bind == STB_LOCAL && type != '?')
			type = ft_tolower(type);


		ft_add_new_elem(&data, syms[i].st_value, type, name);
		//printf("%016x %c %s\n", syms[i].st_value, type, name);
	}
	data = ft_sort_output(data);
	ft_display_output(data);
	ft_clear_output(&data);
	return (0);
}

int ft_loop_over_dym_symbols_64(t_table *table)
{
	char            *file;
	t_output        *data;
	unsigned char   sym_type;

    table->fd = open(table->filename, O_RDONLY);
    if (table->fd < 0)
        return (1);

    if (fstat(table->fd, &table->st) < 0)
        return (1);

    file = mmap(NULL, table->st.st_size, PROT_READ, MAP_PRIVATE, table->fd, 0);
    if (file == MAP_FAILED)
        return (1);

    // safety checks
    if (table->dymsym64.sh_entsize == 0 ||
        table->dymsym64.sh_offset + table->dymsym64.sh_size > table->st.st_size ||
        table->dymstr64.sh_offset + table->dymstr64.sh_size > table->st.st_size)
        return (1);

    int count = table->dymsym64.sh_size / table->dymsym64.sh_entsize;

    t_elf64_sym *syms = (t_elf64_sym *)(file + table->dymsym64.sh_offset);
    char *strtab_data = (char *)(file + table->dymstr64.sh_offset);

    data = NULL;

    for (int i = 0; i < count; i++)
    {
        // validate name offset
	sym_type = ELF64_ST_TYPE(syms[i].st_info);
        if (syms[i].st_name >= table->dymstr64.sh_size)
            continue;

        char *name = strtab_data + syms[i].st_name;

        // ensure null-terminated
        if (!memchr(name, '\0', table->dymstr64.sh_size - syms[i].st_name))
            continue;

        if (name[0] == '\0')
            continue;

        // ignore unwanted types
        int st_type = ELF64_ST_TYPE(syms[i].st_info);
        if (st_type == STT_SECTION || st_type == STT_FILE)
            continue;

        char type = '?';
        unsigned char bind = ELF64_ST_BIND(syms[i].st_info);
		// 🔥 UNDEFINED
		if (syms[i].st_shndx == SHN_UNDEF)
		    type = 'U';

		// 🔥 ABSOLUTE
		else if (syms[i].st_shndx == SHN_ABS)
		    type = 'A';

		// 🔥 COMMON
		else if (syms[i].st_shndx == SHN_COMMON)
		    type = 'C';

		// 🔥 GNU IFUNC (indirect function)
		else if (sym_type == STT_GNU_IFUNC)
		    type = 'I';

		// 🔥 INVALID / RESERVED
		else if (syms[i].st_shndx >= SHN_LORESERVE)
		    type = '?';

		else
		{
			t_elf64_shdr sec = table->sections64[syms[i].st_shndx];
			if (sec.sh_type == SHT_NOBITS)
			    type = 'B';
			else if (sec.sh_flags & SHF_EXECINSTR)
			    type = 'T';
			else if (sec.sh_flags & SHF_WRITE)
			    type = 'D';
			else if (sec.sh_flags & SHF_ALLOC)
			    type = 'R';
			else
				type = 'N'; // debug or non-alloc
		}

		// 🔥 WEAK symbols override (IMPORTANT: after base type)
		if (bind == STB_WEAK)
		{
			if (syms[i].st_shndx == SHN_UNDEF)
				type = 'w';
			else if (sym_type == STT_OBJECT)
				type = 'V';
			else
				type = 'W';
		}

		// 🔥 LOCAL → lowercase (except special '?')
		if (bind == STB_LOCAL && type != '?')
			type = ft_tolower(type);

        ft_add_new_elem(&data, syms[i].st_value, type, name);
    }

    data = ft_sort_output(data);
    //ft_display_output(data);
    //ft_clear_output(&data);

    return (0);
}

/*
int	ft_loop_over_dym_symbols_64(t_table *table)
{
	char		*file;
	struct stat	st;
	t_output	*data;

	table->fd = open(table->filename, O_RDONLY);
	if (table->fd < 0)
		return (1);

	fstat(table->fd, &table->st);
	file = mmap(NULL, table->st.st_size, PROT_READ, MAP_PRIVATE, table->fd, 0);

	if (file == MAP_FAILED)
		return (1);

	// safety checks
	if (table->dymsym64.sh_entsize == 0 ||
		table->dymsym64.sh_offset + table->dymsym64.sh_size > table->st.st_size ||
		table->dymstr64.sh_offset + table->dymstr64.sh_size > table->st.st_size)
		return (1);

	int count = table->dymsym64.sh_size / table->dymsym64.sh_entsize;
	
	data = NULL;

	t_elf64_sym *syms = (t_elf64_sym *)(file + table->dymsym64.sh_offset);
	char *strtab_data = (char *)(file + table->dymstr64.sh_offset);

	for (int i = 0; i < count; i++)
	{
	    // validate name offset
		if (syms[i].st_name >= table->dymstr64.sh_size)
			continue;

		char *name = strtab_data + syms[i].st_name;

		// ensure null-terminated
		if (!memchr(name, '\0', table->dymstr64.sh_size - syms[i].st_name))
		    continue;

		if (name[0] == '\0')
		    continue;
		//ignore STT_FILE
		if (ELF64_ST_TYPE(syms[i].st_info) == STT_SECTION)
    			continue;
		//Also ignore STT_SECTION
		if (ELF64_ST_TYPE(syms[i].st_info) == STT_FILE)
			continue;
		
		unsigned char bind = ELF64_ST_BIND(syms[i].st_info);

		if (bind == STB_WEAK)
		{
			if (syms[i].st_shndx == SHN_UNDEF)
				type = 'w';
			else
				type = 'W';
		}

		char type;

		// 🔥 UNDEFINED
		if (syms[i].st_shndx == SHN_UNDEF)
			type = 'U';
		else
		{
			 t_elf64_shdr sec = table->sections64[syms[i].st_shndx];

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
		//printf("%016lx %c %s\n", syms[i].st_value, type, name);
	}
	data = ft_sort_output(data);
	ft_display_output(data);
	ft_clear_output(&data);
	return (0);
}*/
