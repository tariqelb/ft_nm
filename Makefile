NAME= ft_nm.out

SRCS=	./ft_check_errors_one.c \
	./ft_check_errors_two.c \
	./ft_check_header_fit_in_file.c \
	./ft_check_sections_headers_frames.c \
        ./ft_check_strtab_and_symtab_32.c \
        ./ft_check_strtab_and_symtab.c \
	./ft_display_error.c \
	./ft_display_sym_data.c \
	./ft_loop_over_dym_symbols.c \
	./ft_loop_over_symbols_32.c \
	./ft_loop_over_symbols.c \
	./ft_memchr.c \
	./ft_nm.c \
	./ft_sort_display_output.c

FLAGS= -Werror -Wextra -Wall

all:
	gcc $(FLAGS) -o ${NAME} ${SRCS}

cmp_test:
	gcc -c read_elf_header.c

exec:
	./ft_nm.out read_elf_header.o

fclean:
	/bin/rm -rf ${NAME}
