/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 17:33:26 by thifranc          #+#    #+#             */
/*   Updated: 2017/08/30 17:26:21 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

struct	s_mem {
	t_block *tiny;
	t_block *small;
	t_block *large;
};

struct s_mem g_mem = {NULL, NULL, NULL};

void	*init_lst(int type)
{
	int	tiny_area;

	tiny_area = ((100 * (TINY + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
	if (type == TINY) {
		mmap(NULL, tiny_area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		/*
	} else if (type == SMALL) {
		mmap(NULL, 9999, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	} else {
		mmap(NULL, 9999, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		*/
	}

}

void	*t_malloc(size_t size)
{
	void	*mem;

	if (!g_mem.tiny)
	{
		mem = init_lst(TINY);
	} else {
	}

}

void	*malloc(size_t size)
{
	if (0 < size && size <= TINY)
	{
		return t_malloc(size);
	} else if (size < TINY && size <= SMALL) {
		return s_malloc(size);
	} else if (size > SMALL) {
		return l_malloc(size);
	}

}

void	free(void *ptr)
{
	ptr->free = 1;
	if ((ptr->next)->free || (ptr->prev)->free)
		fusion(*ptr);
}
