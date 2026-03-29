#include "./ft_nm.h"

int	ft_check_sections_headers_frames(t_table *table)
{
	printf("ft_check_sections_headers_frames\n");
	int		i;
	int		rd;
	t_elf64_shdr	section;

	i = 0;
	lseek(table->fd, table->elf64.e_shoff, SEEK_SET);
	while (i < table->elf64.e_shnum)
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		rd = read(table->fd, &section, table->elf64.e_shentsize);
		if (rd < sizeof(t_elf64_ehdr))
		{
			ft_display_error("ft_nm: error in sections header\n");
			return (1);
		}
		if (section.sh_offset + section.sh_size > table->st.st_size)
		{
			if (table->elf64.e_type == ET_REL)
			{
				printf("ft_nm: error sections header size\n");
				return (1);
			}
			else if (table->elf64.e_type == ET_EXEC)
				return (0);
			else if (table->elf64.e_type == ET_DYN)
				return (0);

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

int	ft_check_sections_headers_frames_32(t_table *table)
{
	printf("ft_check_sections_headers_frames\n");
	int		i;
	int		rd;
	t_elf64_shdr	section;

	i = 0;
	lseek(table->fd, table->elf32.e_shoff, SEEK_SET);
	while (i < table->elf32.e_shnum)
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		rd = read(table->fd, &section, table->elf32.e_shentsize);
		if (rd < sizeof(t_elf32_ehdr))
		{
			ft_display_error("ft_nm: error in sections header\n");
			return (1);
		}
		if (section.sh_offset + section.sh_size > table->st.st_size)
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
