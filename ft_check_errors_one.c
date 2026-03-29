#include "./ft_nm.h"

int     ft_check_errors(int ac, char **av, t_table *table)
{
        if (ac < 2)
        {
                ft_display_error("ft_nm : Error, messing file argument\n");
                return (1);
        }
        if (ft_check_file_exist_and_size(av[1], table))
                return (1);
	table->filename = av[1];
        if (ft_check_if_its_elf_file_and_hdrs_exist(table))
                return (1);

        return (0);
}

int     ft_check_file_exist_and_size(char *filename, t_table *table)
{
        table->fd = open(filename, O_RDONLY);
        if (table->fd < 3)
        {
                ft_display_error("ft_nm: ");
                perror(filename);
                return (1);
        }
        fstat(table->fd, &table->st);
        if (table->st.st_size == 0)
        {
                ft_display_error("ft_nm: ");
                ft_display_error(filename);
                ft_display_error(": file is empty\n");
                close(table->fd);
                return (1);
        }
        close(table->fd);
        return (0);
}
