#include "./ft_nm.h"

int	ft_check_sections_headers_frames(int fd, t_elf64_ehdr elf64, size_t file_size)
{
	printf("ft_check_sections_headers_frames\n");
	int		i;
	int		rd;
	t_elf64_shdr	section;

	i = 0;
	lseek(fd, elf64.e_shoff, SEEK_SET);
	while (i < elf64.e_shnum)
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		rd = read(fd, &section, elf64.e_shentsize);
		if (rd < sizeof(t_elf64_ehdr))
		{
			ft_display_error("ft_nm: error in sections header\n");
			return (1);
		}
		if (section.sh_offset + section.sh_size > file_size)
		{
			printf("ft_nm: error sections header size\n");
			return (1);
		}
		else
		{
		//	printf("Section data [%d] [%d] [%ld] [%ld]  \n",
		//		i, section.sh_type, section.sh_offset, section.sh_size);
		}
		i++;
	}
	return (0);
}
