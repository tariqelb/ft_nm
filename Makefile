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
	./ft_loop_over_dym_symbols_32.c \
	./ft_loop_over_dym_symbols_32_util.c \
	./ft_loop_over_dym_symbols_64_util.c \
	./ft_loop_over_symbols_32.c \
	./ft_loop_over_symbols_32_util.c \
	./ft_loop_over_symbols.c \
	./ft_loop_over_symbols_64_util.c \
	./ft_memchr.c \
	./ft_nm.c \
	./ft_sort_output.c \
	./ft_sort_display_output.c

OBJS = $(SRCS:.c=.o)

CC = gcc

CFLAGS = -Wall -Wextra -Werror

all: $(NAME) #clean

$(NAME): $(OBJS)
	$(CC) -o $(NAME) -I . $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

