#include "./ft_nm.h"

int     ft_get_and_check_elf_header_magic_number(uint64_t *first8_ident,
                uint64_t *second8_ident, t_elf64_ehdr elf64)
{
        int             i;
        const int       magic_number = 2135247942;

        i = 0;
        while (i < 16)
        {
                if (i < 8)
                {
                        *first8_ident = *first8_ident | elf64.e_ident[i];
                        if (i == 3)
                        {
                                if (magic_number != *first8_ident)
                                        return (1);
                                printf("Magic number exist\n");
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
                i++;
        }
//printf("Data [%d][%d][%d]\n", elf64.e_ident[4], elf64.e_ident[5], elf64.e_ident[6]);
        if (elf64.e_ident[4] != ELFCLASS32 && elf64.e_ident[4] != ELFCLASS64)
                return (1);
        if (elf64.e_ident[5] != ELFDATA2LSB && elf64.e_ident[5] != ELFDATA2MSB)
                return (1);
        if (elf64.e_ident[6] != EV_CURRENT)
                return (1);
        return (0);
}

int     ft_check_if_its_elf_file_and_hdrs_exist(t_table *table)
{
        int             rd_bytes;
        uint64_t        first8_ident;
        uint64_t        second8_ident;


        table->fd = open(table->filename, O_RDONLY);
        if (fd < 3)
        {
                ft_display_error("ft_nm: ");
                perror(table->filename);
                return (1);
        }
        if (fstat(table->fd, &table->st) == -1)
        {
                ft_display_error("ft_nm: error getting file size\n");
                return (1);
        }
        first8_ident = 0;
        second8_ident = 0;
        //check elf header fit in file
        if (table->st.st_size < sizeof(elf64))
        {
                ft_display_error("ft_nm: error reading elf header\n");
                close(table->fd);
                return (1);
        }
        read(table->fd, &table->elf64, sizeof(t_elf64_ehdr));
	table->is_64 = table->elf64.e_ident[4];
        if (ft_get_and_check_elf_header_magic_number(&first8_ident, &second8_ident, table->elf64))
        {
                ft_display_error("ft_nm: error, not an elf file\n");
                close(table->fd);
                return (1);
        }
	if (table->is_64 == 1)
	{
		lseek(table->fd, 0, SEEK_SET);
        	read(table->fd, &elf64, sizeof(t_elf32_ehdr));
		if (ft_check_elf_headers_32(table))
		{
			close(table->fd);
			return (1);
		}
		close(table->fd);
		return (0);
	}

        //check program header fit in file
        printf("program hrader %ld %d %d %ld\n", table->elf64.e_phoff, table->elf64.e_phnum, table->elf64.e_phentsize, table->st.st_size);
        if (table->elf64.e_phoff + (table->elf64.e_phnum * table->elf64.e_phentsize) > table->st.st_size)
        {
                if (table->elf64.e_phoff == 0)
                        ft_display_error("ft_nm: warning, missing program header\n");
        }
        //check sections header fit in file
        printf("section header : %ld %d %d %ld\n", table->elf64.e_shoff, table->elf64.e_shnum, table->elf64.e_shentsize, table->st.st_size);
        if (table->elf64.e_shoff + (table->elf64.e_shnum * table->elf64.e_shentsize) > table->st.st_size)
        {
                ft_display_error("ft_nm: error, section headers missing in file\n");
                close(table->fd);
                return (1);
        }
        //check sections header frames fit in file
        if (ft_check_sections_headers_frames(table))
		return (1);
	if (ft_check_strtab_and_symtab(table))
		return (1);
        //reach here
        close(fd);
        return (0);
}

int	ft_check_elf_headers_32(t_table *table)
{
        //check program header fit in file
        printf("program hrader %d %d %d %ld\n", table->elf32.e_phoff, table->elf32.e_phnum, table->elf32.e_phentsize, table->st.st_size);
        if (table->elf32.e_phoff + (table->elf32.e_phnum * table->elf32.e_phentsize) > table->st.st_size)
        {
                if (elf32.e_phoff == 0)
                        ft_display_error("ft_nm: warning, missing program header\n");
        }
        //check sections header fit in file
        printf("section header : %d %d %d %ld\n", table->elf32.e_shoff, table->elf32.e_shnum, table->elf32.e_shentsize, table->st.st_size);
        if (table->elf32.e_shoff + (table->elf32.e_shnum * table->elf32.e_shentsize) > table->st.st_size)
        {
                ft_display_error("ft_nm: error, section headers missing in file\n");
                return (1);
        }
        //check sections header frames fit in file
        if (ft_check_sections_headers_frames_32(table))
		return (1);
	if (ft_check_strtab_and_symtab_32(table))
		return (1);
	return (0);
}
