/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 09:50:01 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/04 17:54:36 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

t_bool	get_new_area(int type)
{
	t_block	*new_memory;
	int	tiny_area;

	dprintf(1, "\n\nget new area called !!!!\n\n");
	if (type == TINY) {
		tiny_area = ((100 * (TINY + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
		new_memory = (t_block *)mmap(NULL, tiny_area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		new_memory->size = tiny_area - BLOCKSIZE;
		new_memory->free = TRUE;
		new_memory->next = g_mem.tiny->next;
		new_memory->prev = g_mem.tiny;
		g_mem.tiny->next->prev = new_memory;
		g_mem.tiny->next = new_memory;
	}
	return TRUE;
}

void	init_lst(int type)
{
	int	tiny_area;

	if (type == TINY) {
		tiny_area = ((100 * (TINY + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
		g_mem.tiny = mmap(NULL, tiny_area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		dprintf(1, "plage memoire de %p jusque %p\n", g_mem.tiny, g_mem.tiny + tiny_area);
		g_mem.tiny->size = tiny_area - BLOCKSIZE;
		g_mem.tiny->free = TRUE;
		g_mem.tiny->next = g_mem.tiny;
		g_mem.tiny->prev = g_mem.tiny;
		/*
	} else if (type == SMALL) {
		mmap(NULL, 9999, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	} else {
		mmap(NULL, 9999, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		*/
	}

}

void	*carve_block(t_block *cur, size_t size, int rest)
{
	t_block	*new;

	cur->free = FALSE;
	dprintf(1, "%lu == blocksize\n", BLOCKSIZE);
	new = (void*)cur + BLOCKSIZE + size;
	dprintf(1, "new is at %p, so %p - %p = %ld\n", new, new, cur, (new - cur));
	new->size = rest;
	new->free = TRUE;
	new->prev = cur;
	new->next = cur->next;
	cur->next->prev = new;
	cur->next = new;
	return (void *)cur + BLOCKSIZE;
}
