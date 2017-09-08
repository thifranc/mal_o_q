/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 13:48:23 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/08 10:29:26 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BLOCKSIZE sizeof(t_block) 
#define TINY 2048//relatif au systeme normalement
#define SMALL 4096
#define LARGE 10000


typedef enum		e_bool
{
	FALSE,
	TRUE
}					t_bool;

typedef struct		s_block
{
	t_bool			free;
	size_t			size;
	struct s_block	*next;
	struct s_block	*prev;
}					t_block;

struct	s_mem {
	t_block *tiny;
	t_block *small;
	t_block *large;
	t_block *t_head;
	t_block *s_head;
	t_block *l_head;
};

extern struct s_mem g_mem;

size_t	ft_strlen(const char *c);
void	ft_putstr(char const *s);
void	ft_putchar(char c);
void	ft_putnb_base(long long n, char *base);
t_block	*sort_list(t_block *head);
void	show_alloc_mem();
void	*s_malloc(size_t size);
void	*l_malloc(size_t size);
void	free(void *ptr);
void	*t_malloc(size_t size);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
t_bool	get_new_area(int type);
void	init_lst(int type);
t_bool	size_available(size_t size, t_block **src);
void	*carve_block(t_block *cur, size_t size);
void	*realloc(void *ptr, size_t size);

#endif
