#ifndef FT_NM_H
# define FT_NM_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <string.h>
# include <elf.h>
# include <ctype.h>

# define SHT_NULL	0
# define SHT_PROGBITS	1
# define SHT_SYMTAB	2   // full symbol table (.symtab)
# define SHT_STRTAB	3
# define SHT_RELA	4
# define SHT_HASH	5
# define SHT_DYNAMIC	6
# define SHT_NOTE	7
# define SHT_NOBITS	8
# define SHT_REL	9
# define SHT_DYNSYM	11   // dynamic symbol table (.dynsym)

typedef struct	s_elf32_ehdr
{
	unsigned char e_ident[EI_NIDENT]; // Magic number and file interpretation data
	uint16_t      e_type;             // Object file type
	uint16_t      e_machine;          // Target architecture
	uint32_t      e_version;          // Object file version
	uint32_t      e_entry;            // Entry point virtual address
	uint32_t      e_phoff;            // Program header table file offset
	uint32_t      e_shoff;            // Section header table file offset
	uint32_t      e_flags;            // Processor-specific flags
	uint16_t      e_ehsize;           // ELF header size in bytes
	uint16_t      e_phentsize;        // Size of a program header table entry
	uint16_t      e_phnum;            // Number of entries in the program header table
	uint16_t      e_shentsize;        // Size of a section header table entry
	uint16_t      e_shnum;            // Number of entries in the section header table
	uint16_t      e_shstrndx;         // Section header string table index
}		t_elf32_ehdr;

typedef struct	s_elf64_ehdr
{
	unsigned char	e_ident[EI_NIDENT];
	uint16_t		e_type;
	uint16_t		e_machine;
	uint32_t		e_version;
	uint64_t		e_entry;
	uint64_t		e_phoff;
	uint64_t		e_shoff;
	uint32_t		e_flags;
	uint16_t		e_ehsize;
	uint16_t		e_phentsize;
	uint16_t		e_phnum;
	uint16_t		e_shentsize;
	uint16_t		e_shnum;
	uint16_t		e_shstrndx; 
}		t_elf64_ehdr;

typedef struct	s_elf64_shdr
{
	uint32_t	sh_name;	// offset into .shstrtab
	uint32_t	sh_type;	// section type
	uint64_t	sh_flags;
	uint64_t	sh_addr;
	uint64_t	sh_offset;	// file offset of section data
	uint64_t	sh_size;	// size of section
	uint32_t	sh_link;	// link to another section
	uint32_t	sh_info;
	uint64_t	sh_addralign;
	uint64_t	sh_entsize;	// size of each entry (if table)
}		t_elf64_shdr;

typedef struct  s_elf32_shdr
{
	uint32_t	sh_name;       // offset into .shstrtab
	uint32_t	sh_type;       // section type
	uint32_t	sh_flags;      // flags (WRITE, ALLOC, EXEC...)
	uint32_t	sh_addr;       // virtual address in memory
	uint32_t	sh_offset;     // file offset of section data
	uint32_t	sh_size;       // size of section
	uint32_t	sh_link;       // link to another section
	uint32_t	sh_info;       // extra info
	uint32_t	sh_addralign;  // alignment constraint
	uint32_t	sh_entsize;    // size of each entry (if table)
}               t_elf32_shdr;

typedef struct s_elf64_sym
{
	uint32_t	st_name;	// offset into .strtab
	unsigned char	st_info;
	unsigned char	st_other;
	uint16_t	st_shndx;	// section index
	uint64_t	st_value;	// symbol value (address)
	uint64_t	st_size;
}	t_elf64_sym;

typedef struct s_elf32_sym
{
	uint32_t	st_name;	// offset into .strtab
	unsigned char	st_info;
	unsigned char	st_other;
	uint16_t	st_shndx;	// section index
	uint32_t	st_value;	// symbol value (address)
	uint32_t	st_size;
}	t_elf32_sym;

typedef struct	s_output
{
	long int	addr;
	char		*name;
	char		type;
	struct s_output	*next;
}		t_output;

typedef struct	s_table
{
	int		fd;
	int		is_64;
	char		*filename;
	struct stat 	st;
	t_elf32_shdr    symtab32;
	t_elf32_shdr    strtab32;
	t_elf32_shdr    dymsym32;
	t_elf32_shdr    dymstr32;
	t_elf32_shdr    *sections32;
	t_elf64_shdr    symtab64;
	t_elf64_shdr    strtab64;
	t_elf64_shdr    dymsym64;
	t_elf64_shdr    dymstr64;
	t_elf64_shdr    *sections64;
	t_elf32_ehdr	elf32;
	t_elf64_ehdr	elf64;
}		t_table;


//file :  ft_check_errors_one.c
//int     ft_check_errors(int ac, char **av, int *is_64);
//int     ft_check_file_exist_and_size(char *filename);
int	ft_check_errors(int ac, char **av, t_table *table);
int	ft_check_file_exist_and_size(char *filename, t_table *table);

//file :  ft_check_errors_two.c
//int     ft_check_elf_headers_32(int fd, t_elf32_ehdr elf32, size_t file_size, char *filename);
//int     ft_check_if_its_elf_file_and_hdrs_exist(char *filename, int *is_64);
int	ft_get_and_check_elf_header_magic_number(uint64_t *first8_ident,
		uint64_t *second8_ident, t_elf64_ehdr elf64);
int	ft_check_elf_headers_32(t_table *table);
int	ft_check_if_its_elf_file_and_hdrs_exist(t_table *table);

//file : ft_check_sections_header_frames.c
//int     ft_check_sections_headers_frames(int fd, t_elf64_ehdr elf64, size_t file_size);
//int     ft_check_sections_headers_frames_32(int fd, t_elf32_ehdr elf32, size_t file_size);
int	ft_check_sections_headers_frames(t_table *table);
int	ft_check_sections_headers_frames_32(t_table *table);

//file : ft_check_strtab_and_symtab.c
//int	ft_check_strtab_and_symtab(int fd, t_elf64_ehdr elf64, size_t size, char *filename);
//int	ft_check_strtab_and_symtab_32(int fd, t_elf32_ehdr elf32, size_t size, char *filename);
int	ft_check_strtab_and_symtab(t_table *table);
int	ft_check_strtab_and_symtab_32(t_table *table);

//file :  ft_display_error.c
void    ft_display_error(char *error);

//file : ft_loop_over_symbols.c 
//int     ft_loop_over_symbols(t_elf64_shdr symtab, t_elf64_shdr strtab, t_elf64_shdr *sections,char *av);
//int     ft_loop_over_symbols_32(t_elf32_shdr symtab, t_elf32_shdr strtab, t_elf32_shdr *sections,char *av);
int	ft_loop_over_symbols_32(t_table *table);
int	ft_loop_over_symbols_64(t_table *table);

//file : ft_loop_over_dym_symbols.c 
char    ft_tolower(char c);
int	ft_loop_over_dym_symbols_32(t_table *table);
int	ft_loop_over_dym_symbols_64(t_table *table);

//file : ft_sort_display_output.c
t_output	*ft_sort_output(t_output *data);
void		ft_display_output(t_output *data);
void		ft_clear_output(t_output **head);
t_output	*ft_new_elem(long int addr, char type, char *name);
void		ft_add_new_elem(t_output **head, long int addr, char type, char *name);

#endif
