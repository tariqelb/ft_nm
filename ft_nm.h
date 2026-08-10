/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 18:33:54 by tel-bouh          #+#    #+#             */
/*   Updated: 2026/08/10 22:20:38 by tariq            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <stddef.h>

# define SHT_PROGBITS   1
# define SHT_SYMTAB     2
# define SHT_STRTAB     3
# define SHT_RELA       4
# define SHT_HASH       5
# define SHT_DYNAMIC    6
# define SHT_NOTE       7
# define SHT_NOBITS     8
# define SHT_REL        9
# define SHT_DYNSYM     11

typedef struct s_out
{
	long int		addr;
	char			*name;
	char			type;
	struct s_out	*next;
}		t_output;

typedef struct s_elf32_ehdr
{
	unsigned char	e_ident[EI_NIDENT];
	uint16_t		e_type;
	uint16_t		e_machine;
	uint32_t		e_version;
	uint32_t		e_entry;
	uint32_t		e_phoff;
	uint32_t		e_shoff;
	uint32_t		e_flags;
	uint16_t		e_ehsize;
	uint16_t		e_phentsize;
	uint16_t		e_phnum;
	uint16_t		e_shentsize;
	uint16_t		e_shnum;
	uint16_t		e_shstrndx;
}		t_elf32_ehdr;

typedef struct s_elf64_ehdr
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

typedef struct s_elf64_shdr
{
	uint32_t	sh_name;
	uint32_t	sh_type;
	uint64_t	sh_flags;
	uint64_t	sh_addr;
	uint64_t	sh_offset;
	uint64_t	sh_size;
	uint32_t	sh_link;
	uint32_t	sh_info;
	uint64_t	sh_addralign;
	uint64_t	sh_entsize;
}		t_elf64_shdr;

typedef struct s_elf32_shdr
{
	uint32_t	sh_name;
	uint32_t	sh_type;
	uint32_t	sh_flags;
	uint32_t	sh_addr;
	uint32_t	sh_offset;
	uint32_t	sh_size;
	uint32_t	sh_link;
	uint32_t	sh_info;
	uint32_t	sh_addralign;
	uint32_t	sh_entsize;
}		t_elf32_shdr;

typedef struct s_elf64_sym
{
	uint32_t		st_name;
	unsigned char	st_info;
	unsigned char	st_other;
	uint16_t		st_shndx;
	uint64_t		st_value;
	uint64_t		st_size;
}		t_elf64_sym;

typedef struct s_elf32_sym
{
	uint32_t	st_name;
	uint32_t	st_value;
	uint32_t	st_size;
	uint8_t		st_info;
	uint8_t		st_other;
	uint16_t	st_shndx;
}		t_elf32_sym;

typedef struct s_table
{
	int				fd;
	int				is_64;
	char			*filename;
	char			*file;
	struct stat		st;
	t_elf32_shdr	symtab32;
	t_elf32_shdr	strtab32;
	t_elf32_shdr	dymsym32;
	t_elf32_shdr	dymstr32;
	t_elf32_shdr	*sections32;
	t_elf64_shdr	symtab64;
	t_elf64_shdr	strtab64;
	t_elf64_shdr	dymsym64;
	t_elf64_shdr	dymstr64;
	t_elf64_shdr	*sections64;
	t_elf32_ehdr	elf32;
	t_elf64_ehdr	elf64;
	t_output		*data;
}		t_table;

typedef struct s_data
{
	int				count;
	char			*file;
	char			*strtab_data;
	char			*name;
	char			type;
	t_elf64_sym		*syms64;
	t_elf32_sym		*syms32;
	t_elf64_shdr	sec64;
	t_elf32_shdr	sec32;
	unsigned char	sym_type;
	unsigned char	bind;
}		t_data;

void		ft_free(t_table *table);

//file :  ft_check_errors_one.c
int			ft_check_errors(int ac, char *av, t_table *table);
int			ft_check_file_exist_and_size(char *filename, t_table *table);

//file :  ft_check_errors_two.c
int			ft_get_magin_helper(uint64_t *f,
				uint64_t *s, t_elf64_ehdr elf64, int i);
int			ft_get_check_elf_header_magic_nbr(t_elf64_ehdr elf64);
int			ft_check_magic_number_and_ehdr_size(t_table *table);
int			ft_check_if_its_elf_file_and_hdrs_exist(t_table *table);
int			ft_lseek_read_ehdr_32(t_table *table);

//file : ft_check_headers_fit_in_file.c
int			ft_check_elf_headers_32(t_table *table);
int			ft_check_elf_headers_64(t_table *table);

//file : ft_check_sections_header_frames.c
int			ft_check_sections_headers_frames(t_table *table);
int			ft_check_sections_headers_frames_32(t_table *table);

//file : ft_check_strtab_and_symtab.c
int			ft_get_symtab_dymsym_64(t_table *table,
				int *symtab_exist, int *dymsym_exist);
int			ft_get_strtab_dymstr_64(t_table *table,
				int symtab_exist, int dymsym_exist);
int			ft_check_strtab_symtab_err_64(t_table *table);
int			ft_check_strtab_and_symtab(t_table *table);

//file : ft_check_strtab_and_symtab_32.c
int			ft_get_symtab_dymsym_32(t_table *table,
				int *symtab_exist, int *dymsym_exist);
int			ft_get_strtab_dymstr_32(t_table *table,
				int symtab_exist, int dymsym_exist);
int			ft_check_strtab_symtab_err_32(t_table *table);
int			ft_check_strtab_and_symtab_32(t_table *table);

//file :  ft_display_error.c
int			ft_display_error(char *error);

//file : ft_loop_over_symbols.c 
int			ft_loop_over_symbols_32(t_table *table);

//File : ft_loop_over_symbols_32_util.c
int			ft_loopover_first_check_32(t_table *table, t_data *data_set);
void		ft_sort_display_clean_data_32(t_table *table,
				t_output *data, t_data *data_set);

//File : ft_loop_over_symbols_64_util.c 
int			ft_loopover_first_check(t_table *table, t_data *data_set);
void		ft_sort_display_clean_data(t_table *table,
				t_output *data, t_data *data_set);

//file : ft_loop_over_symbols.c 
int			ft_loop_over_helper_1(t_table *table, t_data *data_set, int i);
void		ft_loop_over_helper_2(t_table *table, t_data *data_set, int i);
t_output	*ft_loop_over_helper_3(t_data *data_set, int i, t_output *data);
t_output        *ft_loop_over_helper_4_64(t_table *table,
				t_data data_set, t_output *data);
int			ft_loop_over_symbols_64(t_table *table);

//file : ft_loop_over_dym_symbols_32.c 
int			ft_loop_over_dym_symbols_32(t_table *table);

//file : ft_loop_over_dym_symbols.c 
int			ft_loop_over_dym_symbols_64(t_table *table);

//file : ft_sort_display_output.c
int			ft_compare_nm(char *s1, char *s2);
t_output	*ft_sort_output(t_output *data);
void		ft_display_output(t_output *data, int is_64_bit);
void		ft_clear_output(t_output **head);
t_output	*ft_new_elem(long int addr, char type, char *name);
void		ft_add_new_elem(t_output **head,
				long int addr, char type, char *name);

//file : ft_display_sym_data.c
void		ft_display_section_temp_32(t_table *table, int i, int rd);
void		ft_display_section_temp_64(t_table *table, int i, int rd);

//file : ft_memchr.c
void		*ft_memchr(const void *s, int c, size_t n);

//File : ft_loop_over_dym_symbols_32_util.c
char		ft_get_symbol_type_32(t_table *table, t_elf32_sym *sym);

//File : ft_loop_over_dym_symbols_64_util.c
char		ft_tolower(char c);
char		ft_get_symbol_type64(t_table *table, t_elf64_sym *sym);

/*
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
uint16_t      e_phnum;        // Number of entries in the program header table
uint16_t      e_shentsize;        // Size of a section header table entry
uint16_t      e_shnum;        // Number of entries in the section header table
uint16_t      e_shstrndx;         // Section header string table index
}		t_elf32_ehdr;

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
typedef struct  s_elf32_syn
{
uint32_t	st_name;   // 4 bytes
uint32_t	st_value;  // 4 bytes
uint32_t	st_size;   // 4 bytes
uint8_t		st_info;   // 1 byte
uint8_t		st_other;  // 1 byte
uint16_t	st_shndx;  // 2 bytes
}	t_elf32_sym;

# define SHT_PROGBITS   1   
// Regular data stored in file (e.g. .text, .data, .rodata)
# define SHT_SYMTAB     2   
// Full symbol table (.symtab) — contains all symbols 
// (used by nm, stripped away in stripped binaries)
# define SHT_STRTAB     3   
// String table — stores strings (symbol names, section names);
// symbols use offsets into this
# define SHT_RELA       4   
// Relocation entries with addends 
// (used for linking and dynamic relocation)
# define SHT_HASH       5   
// Symbol hash table (old SysV) 
// — used by dynamic linker for faster symbol lookup
# define SHT_DYNAMIC    6   
// Dynamic linking info (.dynamic) — contains DT_NEEDED, DT_STRTAB, etc.
# define SHT_NOTE       7   
// Notes/metadata (e.g. ABI info, build ID)
# define SHT_NOBITS     8  
// Section occupies memory but not file space
// (e.g. .bss → zero-initialized data)
# define SHT_REL        9   
// Relocation entries without addends (older format vs RELA)
# define SHT_DYNSYM     11  
// Dynamic symbol table (.dynsym) 
// — only exported/imported symbols used at runtime
*/
#endif
