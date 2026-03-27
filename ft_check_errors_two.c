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

int     ft_check_if_its_elf_file_and_hdrs_exist(char *filename, int *is_64)
{
        int             fd;
        int             rd_bytes;
        uint64_t        first8_ident;
        uint64_t        second8_ident;
	t_elf64_ehdr	elf64;
	t_elf32_ehdr	elf32;
        struct stat     st;


        fd = open(filename, O_RDONLY);
        if (fd < 3)
        {
                ft_display_error("ft_nm: ");
                perror(filename);
                return (1);
        }
        if (fstat(fd, &st) == -1)
        {
                ft_display_error("ft_nm: error getting file size\n");
                return (1);
        }
        first8_ident = 0;
        second8_ident = 0;
        //check elf header fit in file
        if (st.st_size < sizeof(elf64))
        {
                ft_display_error("ft_nm: error reading elf header\n");
                close(fd);
                return (1);
        }
        read(fd, &elf64, sizeof(t_elf64_ehdr));
	*is_64 = elf64.e_ident[4];
        if (ft_get_and_check_elf_header_magic_number(&first8_ident, &second8_ident, elf64))
        {
                ft_display_error("ft_nm: error, not an elf file\n");
                close(fd);
                return (1);
        }
	if (*is_64 == 1)
	{
		lseek(fd, 0, SEEK_SET);
        	read(fd, &elf64, sizeof(t_elf32_ehdr));
		if (ft_check_elf_headers_32(fd, elf32, st.st_size))
		{
			close(fd);
			return (1);
		}
		close(fd);
		return (0);
	}

        //check program header fit in file
        printf("program hrader %ld %d %d %ld\n", elf64.e_phoff, elf64.e_phnum, elf64.e_phentsize, st.st_size);
        if (elf64.e_phoff + (elf64.e_phnum * elf64.e_phentsize) > st.st_size)
        {
                if (elf64.e_phoff == 0)
                        ft_display_error("ft_nm: warning, missing program header\n");
        }
        //check sections header fit in file
        printf("section header : %ld %d %d %ld\n", elf64.e_shoff, elf64.e_shnum, elf64.e_shentsize, st.st_size);
        if (elf64.e_shoff + (elf64.e_shnum * elf64.e_shentsize) > st.st_size)
        {
                ft_display_error("ft_nm: error, section headers missing in file\n");
                close(fd);
                return (1);
        }
        //check sections header frames fit in file
        if (ft_check_sections_headers_frames(fd, elf64, st.st_size))
		return (1);
	if (ft_check_strtab_and_symtab(fd, elf64, st.st_size))
		return (1);
        //reach here
        close(fd);
        return (0);
}

int	ft_check_elf_headers_32(int fd, t_elf32_ehdr elf32, size_t file_size)
{

        //check program header fit in file
        printf("program hrader %d %d %d %ld\n", elf32.e_phoff, elf32.e_phnum, elf32.e_phentsize, file_size);
        if (elf32.e_phoff + (elf32.e_phnum * elf32.e_phentsize) > file_size)
        {
                if (elf32.e_phoff == 0)
                        ft_display_error("ft_nm: warning, missing program header\n");
        }
        //check sections header fit in file
        printf("section header : %d %d %d %ld\n", elf32.e_shoff, elf32.e_shnum, elf32.e_shentsize, file_size);
        if (elf32.e_shoff + (elf32.e_shnum * elf32.e_shentsize) > file_size)
        {
                ft_display_error("ft_nm: error, section headers missing in file\n");
                return (1);
        }
        //check sections header frames fit in file
        if (ft_check_sections_headers_frames_32(fd, elf32, file_size))
		return (1);
	if (ft_check_strtab_and_symtab_32(fd, elf32, file_size))
		return (1);
	return (0);
}
