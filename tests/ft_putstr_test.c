#include <unistd.h>

void    ft_display_error(char *error)
{
        int     len;

        len = 0;
        while (error[len])
                len++;
        if (len)
                write(2, error, len);
}
