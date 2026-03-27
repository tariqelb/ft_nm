#include "./ft_nm.h"

int     ft_check_errors(int ac, char **av, int *is_64)
{
        int     err;

        if (ac < 2)
        {
                ft_display_error("ft_nm : Error, messing file argument\n");
                return (1);
        }
        if (ft_check_file_exist_and_size(av[1]))
                return (1);
        if (ft_check_if_its_elf_file_and_hdrs_exist(av[1], is_64))
                return (1);

        return (0);
}

int     ft_check_file_exist_and_size(char *filename)
{
        int     fd;
        struct stat st;

        fd = open(filename, O_RDONLY);
        if (fd < 3)
        {
                ft_display_error("ft_nm: ");
                perror(filename);
                return (1);
        }
        fstat(fd, &st);
        if (st.st_size == 0)
        {
                ft_display_error("ft_nm: ");
                ft_display_error(filename);
                ft_display_error(": file is empty\n");
                close(fd);
                return (1);
        }
        close(fd);
        return (0);
}
